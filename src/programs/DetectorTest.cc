/**
 * @file ArgonSphere.cc
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief   
 * @version 0.0
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
#include "G4UIterminal.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"
#include "G4ParticleHPManager.hh"

#include "Argon.hh"
#include "ActionInitialization.hh"
#include "ArgonCubeDetector.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"

int main(int argc, char** argv)
{
    G4UIExecutive* UIExecutive = 0;
    

    // create the run manager
#ifdef G4MULTITHREADED
    G4MTRunManager* RunManager = new G4MTRunManager();
    RunManager->SetNumberOfThreads(8);
#else
    G4RunManager* RunManager = new G4RunManager();
#endif

    // create the physics list
    auto PhysicsList = new LArGeant::PhysicsList();
    RunManager->SetUserInitialization(PhysicsList);

    // create the argon cube detector
    auto detector = new LArGeant::ArgonCubeDetector(
        10 * cm, 10 * cm, 10 * cm,
        8, 8, 1 * mm
    );
    auto detectorConstruction = new LArGeant::DetectorConstruction(
        100 * cm, 100 * cm, 100 * cm, detector
    );
    RunManager->SetUserInitialization(detectorConstruction);

    // create the action initialization
    auto PrimaryGeneratorAction = new LArGeant::PrimaryGeneratorAction(
        1,      // number of particles to generate
        "mu-",  // type of particle to generate
        {0,0,0},// starting position
        {0,0,1},// starting momentum direction
        5       // starting momentum (MeV)
    );
    auto ActionInitialization = new LArGeant::ActionInitialization(PrimaryGeneratorAction);
    RunManager->SetUserInitialization(ActionInitialization);


    // start the session
    if (argc == 1)
    {
        UIExecutive = new G4UIExecutive(argc, argv);
    }
#ifdef LARTPC_USE_VIS
        auto VisManager = std::make_shared<G4VisExecutive>();
        VisManager->Initialize();
#endif
        RunManager->Initialize();
#ifdef LARTPC_USE_UI
    G4UImanager* UIManager = G4UImanager::GetUIpointer();

    // start the session
    if (argc == 1)
    {
        UIManager->ApplyCommand("/control/execute vis.mac");
        UIManager->ApplyCommand("/run/verbose 1");
        UIManager->ApplyCommand("/event/verbose 0");
        UIExecutive->SessionStart();
    }
    else
    {
        UIManager->ApplyCommand("/run/verbose 1");
        UIManager->ApplyCommand("/event/verbose 0");
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UIManager->ApplyCommand(command+fileName);
    }
#endif

#ifdef LARTPC_USE_UI_TCSH
        auto UITerminal = std::make_shared<G4UIterminal>(new G4UItcsh);
#else   
        auto UITerminal = std::make_shared<G4UIterminal>();
#endif
        UITerminal->SessionStart();

    return 0;
}