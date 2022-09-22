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
        mPhysListFactory = std::make_shared<G4PhysListFactory>();
        mPhysicsLists = mPhysListFactory->AvailablePhysLists();
    }

    LArGeantPhysicsList::~LArGeantPhysicsList()
    {
    }

    void LArGeantPhysicsList::SetCuts()
    {
        SetCutValue(1 * mm, "proton");
        SetCutValue(1 * um, "gamma");
        SetCutValue(1 * um, "e-");
        SetCutValue(1 * um, "e+");
    }

    void LArGeantPhysicsList::PrintPhysicsLists()
    {
        G4cout << "Enabled Physics Lists:" << G4endl;
        for(size_t ii = 0; ii < mPhysicsLists.size(); ii++)
        {
            G4cout << "\t[" << ii << "]: " << mPhysicsLists[ii] << G4endl;
        }
    }
}