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

        // Get the list of physics lists
        fPhysListFactory = std::make_shared<G4PhysListFactory>();
        fPhysicsLists = fPhysListFactory->AvailablePhysLists();
    }

    LArGeantPhysicsList::~LArGeantPhysicsList()
    {
    }

    // void LArGeantPhysicsList::ConstructProcess()
    // { 
    //     LArDetector* detector;
    //     LArNESTScintillationProcess* scintillationProcess = new LArNESTScintillationProcess("S1", fElectromagnetic, detector);
    //     fPManager->AddProcess(scintillationProcess, ordDefault + 1, ordInActive, ordDefault + 1);
    //     std::cout << "HERE" << std::endl;
    // }

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
    //     LArNESTThermalElectron::Definition();
    // }

    void LArGeantPhysicsList::SetCuts()
    {
        SetCutValue(1 * mm, "proton");
        SetCutValue(1 * mm, "gamma");
        SetCutValue(1 * mm, "e-");
        SetCutValue(1 * mm, "e+");
    }

    void LArGeantPhysicsList::PrintPhysicsLists()
    {
        G4cout << "Enabled Physics Lists:" << G4endl;
        for(size_t ii = 0; ii < fPhysicsLists.size(); ii++)
        {
            G4cout << "\t[" << ii << "]: " << fPhysicsLists[ii] << G4endl;
        }
    }
}