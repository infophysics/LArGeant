/**
 * @file LArGeantArgonCubeSensitiveDetector.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-05-01
 */
#pragma once
#include "G4SystemOfUnits.hh"
#include "G4VSensitiveDetector.hh"
#include "G4StepPoint.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

namespace largeant
{
    class LArGeantArgonCubeSensitiveDetector : public G4VSensitiveDetector
    {
    public:
        LArGeantArgonCubeSensitiveDetector(G4String name);
        ~LArGeantArgonCubeSensitiveDetector();

    private:
        virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*); 
    };
}