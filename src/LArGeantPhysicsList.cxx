/**
 * @file LArGeantPhysicsList.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "LArGeantPhysicsList.hh"

namespace largeant
{
    LArGeantPhysicsList::LArGeantPhysicsList()
    : G4VModularPhysicsList()
    {   
        // Standard EM Physics
        RegisterPhysics(new G4EmStandardPhysics());

        // Synchroton Radiation & GN Physics
        RegisterPhysics(new G4EmExtraPhysics());

        // Optical Physics
        RegisterPhysics(new G4OpticalPhysics());

        // Decay Physics
        RegisterPhysics(new G4DecayPhysics());
        RegisterPhysics(new G4RadioactiveDecayPhysics());

        // Stopping Physics
        RegisterPhysics(new G4StoppingPhysics());

        // Ion Physics
        RegisterPhysics(new G4IonPhysics());

        // Hadron Elastic scattering
        RegisterPhysics(new G4HadronElasticPhysicsHP());

        // Hadron Physics
        RegisterPhysics(new G4HadronPhysicsQGSP_BERT_HP());

        // Neutron Physics
        //RegisterPhysics(new LArGeantNeutronHPPhysics("neutronHP"));
    }

    LArGeantPhysicsList::~LArGeantPhysicsList()
    {
    }

    // void LArGeantPhysicsList::ConstructParticle()
    // {
    //     G4BosonConstructor pBosonConstructor;
    //     pBosonConstructor.ConstructParticle();

    //     G4LeptonConstructor pLeptonConstructor;
    //     pLeptonConstructor.ConstructParticle();

    //     G4MesonConstructor pMesonConstructor;
    //     pMesonConstructor.ConstructParticle();

    //     G4BaryonConstructor pBaryonConstructor;
    //     pBaryonConstructor.ConstructParticle();

    //     G4IonConstructor pIonConstructor;
    //     pIonConstructor.ConstructParticle();

    //     G4ShortLivedConstructor pShortLivedConstructor;
    //     pShortLivedConstructor.ConstructParticle();
    // }

    void LArGeantPhysicsList::SetCuts()
    {
        SetCutValue(1 * mm, "proton");
        SetCutValue(1 * mm, "gamma");
        SetCutValue(1 * mm, "e-");
        SetCutValue(1 * mm, "e+");
    }
}