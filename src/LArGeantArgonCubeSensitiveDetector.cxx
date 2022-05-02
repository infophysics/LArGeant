/**
 * @file LArGeantArgonCubeSensitiveDetector.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-05-01
 */
#include "LArGeantArgonCubeSensitiveDetector.hh"

namespace largeant
{
    LArGeantArgonCubeSensitiveDetector::LArGeantArgonCubeSensitiveDetector(G4String name)
    : G4VSensitiveDetector(name)
    {
    }

    LArGeantArgonCubeSensitiveDetector::~LArGeantArgonCubeSensitiveDetector()
    {
    }

    G4bool LArGeantArgonCubeSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* history)
    {
        G4Track* track = step->GetTrack();
        G4StepPoint* preStepPoint = step->GetPreStepPoint();
        G4StepPoint* postStepPoint = step->GetPostStepPoint();

        G4ThreeVector photonPosition = preStepPoint->GetPosition();
        G4cout << photonPosition << std::endl;
    }
}