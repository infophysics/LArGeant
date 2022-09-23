/**
 * @file LArNESTInterface.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
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
    struct Hit 
    {
    public:
        Hit(double _E, double _t, G4ThreeVector _xyz)
        : E(_E), t(_t), xyz(_xyz), result{0, 0, 0, 0} 
        {};
        double E;
        double t;
        G4ThreeVector xyz;
        NEST::LArYieldFluctuationResult result;
    };

    struct Lineage 
    {
    public:
        Lineage(NEST::LArInteraction _type) 
        : type(_type)
        {};

        NEST::LArInteraction type = NEST::LArInteraction::NoneType;
        std::vector<Hit> hits;
        double density = -1;
        NEST::LArNESTResult result;
        bool result_calculated = false;
    };

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
        LArNESTScintillationProcess(const G4String& processName, G4ProcessType type,
                LArDetector* detector);
        LArNESTScintillationProcess(const G4String& processName, G4ProcessType type,
                NEST::LArNEST* customcalc, LArDetector* detector);
        ~LArNESTScintillationProcess();

    public:
        G4bool IsApplicable(const G4ParticleDefinition& aParticleType);
        // Returns true -> 'is applicable', for any particle type except for an
        // 'opticalphoton' and for short-lived particles

        G4double GetMeanFreePath(const G4Track& aTrack, G4double, G4ForceCondition*);
        // Returns infinity; i. e. the process does not limit the step, but
        // sets the 'StronglyForced' condition for the DoIt to be invoked at
        // every step.

        G4double GetMeanLifeTime(const G4Track& aTrack, G4ForceCondition*);
        // Returns infinity; i. e. the process does not limit the time, but
        // sets the 'StronglyForced' condition for the DoIt to be invoked at
        // every step.

        // For in-flight particles losing energy (or those stopped)
        G4VParticleChange* PostStepDoIt(const G4Track& aTrack, const G4Step& aStep);
        G4VParticleChange* AtRestDoIt(const G4Track& aTrack, const G4Step& aStep);

        void TryPopLineages(const G4Track& aTrack, const G4Step& aStep);

        void SetScintillationYieldFactor(const G4double yieldfactor);
        // Called to set the scintillation quantum yield factor, useful for
        // shutting off scintillation entirely, or for producing a universal
        // re-scaling to for example represent detector effects. Internally is
        // used for Lindhard yield factor for NR. Default should be user-set
        // to be 1 (for ER) in your simulation -- see NEST readme

        G4double GetScintillationYieldFactor() const;
        // Returns the quantum (photon/electron) yield factor. See above.

        Lineage GetChildType(const G4Track* aTrack, const G4Track* sec) const;
        G4Track* MakePhoton(G4ThreeVector xyz, double t);
        G4Track* MakeElectron(
            G4ThreeVector xyz, double density, 
            double t,double kin_E
        );
        std::vector<Lineage> getLastLineages() const { return lineages_prevEvent; }

        void SetDetailedSecondaries(bool detailed)  { detailed_secondaries = detailed; }
        void SetStackElectrons(bool stack_e)        { stack_electrons = stack_e; }
        void SetStackPhotons(bool stack_ph)         { stack_photons = stack_ph; }
        void SetAnalysisTrigger(
            std::function<void(std::vector<Lineage>)> _analysisTrigger
        ) 
        {
            this->analysisTrigger = _analysisTrigger;
        }

        void SetGamma_break(double _gamma_break) { this->gamma_break = _gamma_break; }

        double GetGamma_break() const { return gamma_break; }

        void SetLArNEST(std::unique_ptr<NEST::LArNEST> newcalc) 
        {
            fLArNEST.reset(newcalc.release());
            LArDetector* detector;
            //fDetector.reset(newcalc->GetDetector());
            fDetector.reset(detector);
        }

    protected:
        // bools for tracking some special particle cases

        std::unique_ptr<NEST::LArNEST> fLArNEST = NULL;
        std::vector<Lineage> lineages;
        std::vector<Lineage> lineages_prevEvent;
        std::map<std::tuple<int, CLHEP::Hep3Vector, CLHEP::Hep3Vector>, uint64_t>
            track_lins;
        std::unique_ptr<LArDetector> fDetector;
        NoTimeParticleChange fParticleChange;

        G4double YieldFactor = 1;  // turns scint. on/off
        bool detailed_secondaries = true;
        bool stack_electrons = true;
        bool stack_photons = true;
        double gamma_break =
            9 * mm;  // Gammas will not pass on their lineage (if they have one, e.g.
                    // bremsstrahlung) if they are this far from their origin.
        int verbose = 0;

        std::function<void(std::vector<Lineage>)> analysisTrigger;
    };

    inline G4bool LArNESTScintillationProcess::IsApplicable(
        const G4ParticleDefinition& aParticleType
    ) 
    {
        if (aParticleType.GetParticleName() == "opticalphoton") return false;
        if (aParticleType.IsShortLived()) return false;
        if (aParticleType.GetParticleName() == "thermalelectron") return false;
        // if(abs(aParticleType.GetPDGEncoding())==2112 || //neutron (no E-dep.)
        return true;
    }

    inline void LArNESTScintillationProcess::SetScintillationYieldFactor(const G4double yieldfactor)
    {
        YieldFactor = yieldfactor;
    }

    inline G4double LArNESTScintillationProcess::GetScintillationYieldFactor() const 
    {
        return YieldFactor;
    }
}