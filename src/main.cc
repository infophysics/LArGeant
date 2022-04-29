/**
 * @file main.cc
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
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

#include "LArGeantActionInitialization.hh"
#include "LArGeantDetectorConstruction.hh"
#include "LArGeantPhysicsList.hh"

using namespace largeant;

int main(int argc, char** argv)
{
    // create the run manager
    G4RunManager* runManager = new G4RunManager();
    runManager->SetUserInitialization(new LArGeantDetectorConstruction());
    runManager->SetUserInitialization(new LArGeantPhysicsList());
    runManager->SetUserInitialization(new LArGeantActionInitialization());
    runManager->Initialize();

    // UI manager and executive
    G4UImanager* uiManager = G4UImanager::GetUIpointer();
    G4UIExecutive* uiExecutive = 0;

    // visualization manager
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    // start the session
    if (argc == 1)
    {
        uiExecutive = new G4UIExecutive(argc, argv);
        uiManager->ApplyCommand("/vis/open OGL");
        uiManager->ApplyCommand("/vis/viewer/set/viewpointVector 1 1 1");
        uiManager->ApplyCommand("/vis/drawVolume");
        uiManager->ApplyCommand("/vis/viewer/set/autoRefresh true");
        uiManager->ApplyCommand("/vis/scene/add/tracjectories rich");
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