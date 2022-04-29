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
        RegisterPhysics(new G4EmStandardPhysics());
        RegisterPhysics(new G4OpticalPhysics());
        RegisterPhysics(new G4DecayPhysics());
        RegisterPhysics(new G4RadioactiveDecayPhysics());
        //RegisterPhysics(new G4NeutronHPPhysics("neutronHP"));
    }

    LArGeantPhysicsList::~LArGeantPhysicsList()
    {
    }

    void LArGeantPhysicsList::ConstructParticle()
    {
        G4BosonConstructor pBosonConstructor;
        pBosonConstructor.ConstructParticle();

        G4LeptonConstructor pLeptonConstructor;
        pLeptonConstructor.ConstructParticle();

        G4MesonConstructor pMesonConstructor;
        pMesonConstructor.ConstructParticle();

        G4BaryonConstructor pBaryonConstructor;
        pBaryonConstructor.ConstructParticle();

        G4IonConstructor pIonConstructor;
        pIonConstructor.ConstructParticle();

        G4ShortLivedConstructor pShortLivedConstructor;
        pShortLivedConstructor.ConstructParticle();
    }

    void LArGeantPhysicsList::SetCuts()
    {
        SetCutValue(0 * mm, "proton");
    }
}