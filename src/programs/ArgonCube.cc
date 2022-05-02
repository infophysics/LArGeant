/**
 * @file ArgonSphere.cc
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief Small program for simulating physics in a
 * sphere of natural argon.  
 * @version 0.1
 * @date 2022-04-27
 */
#include <iostream>
#include <vector>
#include <string>

// Geant4 includes 
#include "G4UIExecutive.hh"
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"
#include "G4PhysListFactory.hh"
#include "G4ParticleHPManager.hh"
#include "G4Types.hh"

#include "FTFP_BERT.hh"

#include "LArGeantArgon.hh"
#include "LArGeantActionInitialization.hh"
#include "LArGeantArgonCube.hh"
#include "LArGeantArgonCubeDetector.hh"
#include "LArGeantPhysicsList.hh"

using namespace largeant;

int main(int argc, char** argv)
{
    G4UIExecutive* uiExecutive = 0;

    // create argon cube object
    LArGeantArgon Argon;
    // create the run manager
    G4RunManager* runManager = new G4RunManager();
    runManager->SetUserInitialization(
        new LArGeantArgonCubeDetector(
            Argon,
            50000, 50000, 50000, 
            100, 100, 
            2000
        )
    );
    runManager->SetUserInitialization(new LArGeantPhysicsList());
    runManager->SetUserInitialization(new LArGeantActionInitialization());

    // // Replaced HP environmental variables with C++ calls                                                                                     
    // G4ParticleHPManager::GetInstance()->SetSkipMissingIsotopes( true );
    // G4ParticleHPManager::GetInstance()->SetDoNotAdjustFinalState( true );
    // G4ParticleHPManager::GetInstance()->SetUseOnlyPhotoEvaporation( true );
    // G4ParticleHPManager::GetInstance()->SetNeglectDoppler( true );
    // G4ParticleHPManager::GetInstance()->SetProduceFissionFragments( true );
    // G4ParticleHPManager::GetInstance()->SetUseWendtFissionModel( true );
    // G4ParticleHPManager::GetInstance()->SetUseNRESP71Model( true );
    
    runManager->Initialize();
    
    // print out available physics lists
    G4PhysListFactory *physListFactory = new G4PhysListFactory();
    const std::vector<G4String> physicsLists = physListFactory->AvailablePhysLists();
    std::cout << "Enabled Physics Lists:" << std::endl;
    for(size_t i = 0; i < physicsLists.size(); i++)
    {
        std::cout << "\t[" << i << "]: " << physicsLists[i] << std::endl;
    }
    // print out all processes for neutrons
    G4ParticleDefinition* neutron = G4Neutron::Neutron();
    G4ProcessManager* pManager = neutron->GetProcessManager();
    G4ProcessVector* processes = pManager->GetProcessList();
    std::cout << "Enabled Neutron HP Physics Processes:" << std::endl;
    for(size_t i = 0; i < processes->size(); i++)
    {
        std::cout << "\t[" << i << "]: " << (*processes)[i]->GetProcessName() << std::endl;
    }

    // start the session
    if (argc == 1)
    {
        uiExecutive = new G4UIExecutive(argc, argv);
    }
    // visualization manager
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();
    
    // UI manager and executive
    G4UImanager* uiManager = G4UImanager::GetUIpointer();

    // start the session
    if (argc == 1)
    {
        uiManager->ApplyCommand("/control/execute vis.mac");
        uiManager->ApplyCommand("/run/verbose 1");
        uiManager->ApplyCommand("/event/verbose 0");
        uiExecutive->SessionStart();
    }
    else
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        uiManager->ApplyCommand(command+fileName);
    }

    

    return 0;
}