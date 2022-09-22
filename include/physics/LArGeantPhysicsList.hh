/**
 * @file LArGeantPhysicsList.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include <vector>
#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4HadronElasticProcess.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"

#include "G4ParticleHPThermalScattering.hh"
#include "G4ParticleHPElasticData.hh"
#include "G4ParticleHPThermalScatteringData.hh"
#include "G4ParticleHPElastic.hh"
#include "G4ParticleHPInelasticData.hh"
#include "G4ParticleHPInelastic.hh"
#include "G4ParticleHPCaptureData.hh"
#include "G4ParticleHPCapture.hh"
#include "G4ParticleHPFissionData.hh"
#include "G4ParticleHPFission.hh"

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4ProcessTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysListFactory.hh"

#include "LArGeantNeutronHPPhysics.hh"
#include "LArDetector.hh"
#include "LArGeantLArNEST.hh"

namespace largeant
{
    class LArGeantPhysicsList : public G4VModularPhysicsList
    {
    public:
        LArGeantPhysicsList();
        ~LArGeantPhysicsList();

        //virtual void ConstructParticle();
        //virtual void ConstructProcess();
        virtual void SetCuts();

        void PrintPhysicsLists();

    private:
        std::shared_ptr<G4PhysListFactory> mPhysListFactory;
        std::vector<G4String> mPhysicsLists;

        std::shared_ptr<G4ProcessManager> mPManager = {nullptr};
        std::shared_ptr<G4ProcessVector> mProcesses = {nullptr};
    };
}