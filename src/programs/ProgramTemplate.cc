/**
 * @file Template.cc
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
#include "Xenon.hh"
#include "ActionInitialization.hh"
#include "ArgonCubeDetector.hh"
#include "DetectorConstruction.hh"
#include "EventManager.hh"
#include "NESTInterface.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"

int main(int argc, char** argv)
{
    //-------- create the ui executive --------//
    G4UIExecutive* UIExecutive = 0;
    

    //-------- create the run manager --------//
    /**
     * The run manager is created with a certain
     * number of threads.
     */
    G4int NumberOfThreads = 10;
#ifdef LARGEANT_MULTITHREADED
    G4MTRunManager* RunManager = new G4MTRunManager();
    RunManager->SetNumberOfThreads(NumberOfThreads);
#else
    G4RunManager* RunManager = new G4RunManager();
#endif


    //-------- create the physics list --------//
    /**
     * The default physics list contains all of the 
     * Scintillation physics and the NEST interface.
     */
    auto PhysicsList = new LArGeant::PhysicsList();
    RunManager->SetUserInitialization(PhysicsList);


    //-------- create the detector --------//
    /**
     * The detector must be instantiated with an instance of
     * a LArGeant::Detector, which itself contains the various
     * components that make up the detector.  
     * 
     * The experimental hall is a cube with sides defaulted
     * to 100cm each.
     */
    auto detector = nullptr;
    G4double ExperimentalHallX = 100 * cm;
    G4double ExperimentalHallY = 100 * cm;
    G4double ExperimentalHallZ = 100 * cm;
    auto detectorConstruction = new LArGeant::DetectorConstruction(
        ExperimentalHallX,
        ExperimentalHallY,
        ExperimentalHallZ,
        detector
    );
    RunManager->SetUserInitialization(detectorConstruction);


    //-------- create the action initialization --------//
    /**
     * The action initialization does not really need to be
     * edited, unless the user wants to do something different
     * than the default behavior.
     */
    auto ActionInitialization = new LArGeant::ActionInitialization(
        new LArGeant::PrimaryGeneratorAction()
    );
    RunManager->SetUserInitialization(ActionInitialization);


    //-------- start the session --------//
    if (argc == 1)
    {
        UIExecutive = new G4UIExecutive(argc, argv);
    }
#ifdef LARGEANT_USE_VIS
        auto VisManager = std::make_shared<G4VisExecutive>();
        VisManager->Initialize();
#endif
        RunManager->Initialize();
#ifdef LARGEANT_USE_UI
    G4UImanager* UIManager = G4UImanager::GetUIpointer();
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

#ifdef LARGEANT_USE_UI_TCSH
        auto UITerminal = std::make_shared<G4UIterminal>(new G4UItcsh);
#else   
        auto UITerminal = std::make_shared<G4UIterminal>();
#endif
        UITerminal->SessionStart();

    return 0;
}