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
    G4UIExecutive* UIExecutive = 0;

    /// create argon cube object
    LArGeantArgon Argon(
        85.8 * kelvin,      /// temperature
        0.952 * atmosphere, /// pressure
        0.334,              /// ratio of Ar36
        0.063,              /// ratio of Ar38
        99.603              /// ratio of Ar40
    );
    G4double efield = 500.0;

//     // create the run manager
// #ifdef G4MULTITHREADED
//     G4MTRunManager* RunManager = new G4MTRunManager();
// #else
    G4RunManager* RunManager = new G4RunManager();
// #endif

    // create the argon cube detector
    RunManager->SetUserInitialization(
        new LArGeantArgonCubeDetector(
            Argon,  // Argon object from above
            0.1,  // length in X (m)
            0.1,  // length in Y (m)
            0.1,  // length in Z (m)
            8,    // number of pmts in X 
            8,    // number of pmts in Y 
            0.001 // thickness of pmts (m)
        )
    );
    // create the physics list
    RunManager->SetUserInitialization(new LArGeantPhysicsList());

    // create the action initialization
    LArGeantPrimaryGeneratorAction PrimaryGeneratorAction(
        1,      // number of particles to generate
        "mu-",  // type of particle to generate
        {0,0,0},// starting position
        {0,0,1},// starting momentum direction
        1  // starting momentum (MeV)
    );
    RunManager->SetUserInitialization(new LArGeantActionInitialization(PrimaryGeneratorAction, efield));

    // // Replaced HP environmental variables with C++ calls                                                                                     
    // G4ParticleHPManager::GetInstance()->SetSkipMissingIsotopes( true );
    // G4ParticleHPManager::GetInstance()->SetDoNotAdjustFinalState( true );
    // G4ParticleHPManager::GetInstance()->SetUseOnlyPhotoEvaporation( true );
    // G4ParticleHPManager::GetInstance()->SetNeglectDoppler( true );
    // G4ParticleHPManager::GetInstance()->SetProduceFissionFragments( true );
    // G4ParticleHPManager::GetInstance()->SetUseWendtFissionModel( true );
    // G4ParticleHPManager::GetInstance()->SetUseNRESP71Model( true );

    // start the session
    if (argc == 1)
    {
        UIExecutive = new G4UIExecutive(argc, argv);
    }
    // visualization manager
    G4VisManager* VisManager = new G4VisExecutive();
    VisManager->Initialize();
    
    // UI manager and executive
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

    

    return 0;
}