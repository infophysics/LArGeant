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
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"
#include "G4PhysListFactory.hh"
// Neutron HP
#include "G4ParticleHPManager.hh"
#include "G4Types.hh"

int main(int argc, char** argv)
{
    // create the run manager
    G4RunManager* runManager = new G4RunManager();
    runManager->Initialize();

    // UI manager and executive
    G4UImanager* uiManager = G4UImanager::GetUIpointer();
    G4UIExecutive* uiExecutive = new G4UIExecutive(argc, argv);

    // visualization manager
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    // start the session
    uiExecutive->SessionStart();

    return 0;
}