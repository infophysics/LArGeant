/**
 * @file LArNESTInterface.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @details Code adapted from Jason Brodsky's G4Integration code
 * in the NEST repository.
 * @date 2022-09-14
 */
#pragma once
#include <functional>
#include "G4Electron.hh"
#include "G4EmProcessSubType.hh"  
#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"     
#include "G4PhysicalConstants.hh"
#include "G4ProductionCuts.hh"
#include "G4RandomDirection.hh"
#include "G4SystemOfUnits.hh"
#include "G4UserLimits.hh"
#include "G4Version.hh"
#include "StackingAction.hh"
#include "G4Track.hh"
#include "G4DynamicParticle.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4OpticalPhoton.hh"
#include "G4ParticleMomentum.hh"
#include "G4PhysicsOrderedFreeVector.hh"
#include "G4Poisson.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4VRestDiscreteProcess.hh"
#include "Randomize.hh"
#include "globals.hh"
#include "templates.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4ProductionCuts.hh"
#include "G4SystemOfUnits.hh"
#include "LArNEST.hh"
#include "LArDetector.hh"

namespace LArGeant
{
    class NoTimeParticleChange : public G4ParticleChange 
    {
    public:
        NoTimeParticleChange() : G4ParticleChange() { debugFlag = false; }
    };

    class LArNESTThermalElectron : public G4ParticleDefinition 
    {
    public:
        static LArNESTThermalElectron* Definition();
        static LArNESTThermalElectron* ThermalElectronDefinition();
        static LArNESTThermalElectron* ThermalElectron();
    private:
        static LArNESTThermalElectron* theInstance;
        LArNESTThermalElectron() {}
        ~LArNESTThermalElectron() {}
    };

    class LArNESTScintillationProcess : public G4VRestDiscreteProcess 
    {
    public:
        LArNESTScintillationProcess(
            const G4String& processName, 
            G4ProcessType type,
            LArDetector* detector
        );
        LArNESTScintillationProcess(
            const G4String& processName, 
            G4ProcessType type,
            NEST::LArNEST* customcalc, 
            LArDetector* detector
        );
        ~LArNESTScintillationProcess();

    public:
        // Returns true -> 'is applicable', for any particle type except for an
        // 'opticalphoton' and for short-lived particles
        G4bool IsApplicable(const G4ParticleDefinition& aParticleType);
        
        // Returns infinity; i. e. the process does not limit the step, but
        // sets the 'StronglyForced' condition for the DoIt to be invoked at
        // every step.
        G4double GetMeanFreePath(const G4Track& aTrack, G4double, G4ForceCondition*);

        // Returns infinity; i. e. the process does not limit the time, but
        // sets the 'StronglyForced' condition for the DoIt to be invoked at
        // every step.
        G4double GetMeanLifeTime(const G4Track& aTrack, G4ForceCondition*);

        // For in-flight particles losing energy (or those stopped)
        G4VParticleChange* PostStepDoIt(const G4Track& aTrack, const G4Step& aStep);
        G4VParticleChange* AtRestDoIt(const G4Track& aTrack, const G4Step& aStep);

        // Called to set the scintillation quantum yield factor, useful for
        // shutting off scintillation entirely, or for producing a universal
        // re-scaling to (for example) represent detector effects. Internally is
        // used for Lindhard yield factor for NR. Default should be user-set
        // to be 1 (for ER) in your simulation -- see NEST readme
        void SetScintillationYieldFactor(const G4double yieldfactor);
        
        // Returns the quantum (photon/electron) yield factor. See above.
        G4double GetScintillationYieldFactor() const;
        
        G4Track* MakePhoton(G4ThreeVector xyz, double t);
        G4Track* MakeElectron(
            G4ThreeVector xyz, double density, 
            double t,double kin_E
        );

        void SetDetailedSecondaries(bool detailed)  { mDetailedSecondaries = detailed; }
        void SetStackElectrons(bool stack_e)        { mStackElectrons = stack_e; }
        void SetStackPhotons(bool stack_ph)         { mStackPhotons = stack_ph; }
        // void SetAnalysisTrigger(std::function<void(std::vector<Lineage>)> AnalysisTrigger) 
        // {
        //     this->mAnalysisTrigger = AnalysisTrigger;
        // }
        void SetLArNEST(std::unique_ptr<NEST::LArNEST> newcalc) 
        {
            mLArNEST.reset(newcalc.release());
            LArDetector* detector;
            //mDetector.reset(newcalc->GetDetector());
            mDetector.reset(detector);
        }
        void SetGammaBreak(double _gamma_break) { this->mGammaBreak = _gamma_break; }
        double GetGammaBreak() const { return mGammaBreak; }
        

    protected:
        std::unique_ptr<NEST::LArNEST> mLArNEST = {nullptr};
        std::map<std::tuple<int, CLHEP::Hep3Vector, CLHEP::Hep3Vector>, uint64_t>
            track_lins;
        std::unique_ptr<LArDetector> mDetector;
        NoTimeParticleChange mParticleChange;

        G4double mYieldFactor = 1;  // turns scint. on/off
        G4bool mDetailedSecondaries = true;
        G4bool mStackElectrons = true;
        G4bool mStackPhotons = true;

        // Gammas will not pass on their lineage (if they have one, e.g.
        // bremsstrahlung) if they are this far from their origin.
        G4double mGammaBreak = 9 * mm;  


    };

    inline G4bool LArNESTScintillationProcess::IsApplicable(
        const G4ParticleDefinition& aParticleType
    ) 
    {
        if (
            aParticleType.GetParticleName() == "opticalphoton" || 
            aParticleType.GetParticleName() == "thermalelectron"
        ) { 
            return false;
        }
        if (aParticleType.IsShortLived()) return false;
        return true;
    }

    inline void LArNESTScintillationProcess::SetScintillationYieldFactor(const G4double yieldfactor)
    {
        mYieldFactor = yieldfactor;
    }

    inline G4double LArNESTScintillationProcess::GetScintillationYieldFactor() const 
    {
        return mYieldFactor;
    }
}