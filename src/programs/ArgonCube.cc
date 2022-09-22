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

#include "LArGeantArgon.hh"
#include "LArGeantActionInitialization.hh"
#include "LArGeantArgonCube.hh"
#include "LArGeantArgonCubeDetector.hh"
#include "LArGeantPhysicsList.hh"

using namespace largeant;

int main(int argc, char** argv)
{
    G4UIExecutive* UIExecutive = 0;

    /// create the argon object
    LArGeantArgon Argon(
        85.8 * kelvin,      /// temperature
        0.952 * atmosphere, /// pressure
        0.334,              /// ratio of Ar36
        0.063,              /// ratio of Ar38
        99.603              /// ratio of Ar40
    );

// create the run manager
#ifdef G4MULTITHREADED
    G4MTRunManager* RunManager = new G4MTRunManager();
    RunManager->SetNumberOfThreads(8);
#else
    G4RunManager* RunManager = new G4RunManager();
#endif

    // create the argon cube detector
    auto ArgonCubeDetector = new LArGeantArgonCubeDetector(
        Argon,  // Argon object from above
        0.1,  // length in X (m)
        0.1,  // length in Y (m)
        0.1,  // length in Z (m)
        8,    // number of pmts in X 
        8,    // number of pmts in Y 
        0.001 // thickness of pmts (m)
    );
    RunManager->SetUserInitialization(ArgonCubeDetector);

    // create the physics list
    auto PhysicsList = new LArGeantPhysicsList();
    RunManager->SetUserInitialization(PhysicsList);

    // create the action initialization
    auto PrimaryGeneratorAction = new LArGeantPrimaryGeneratorAction(
        1,      // number of particles to generate
        "mu-",  // type of particle to generate
        {0,0,0},// starting position
        {0,0,1},// starting momentum direction
        5  // starting momentum (MeV)
    );
    auto ActionInitialization = new LArGeantActionInitialization(PrimaryGeneratorAction);
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