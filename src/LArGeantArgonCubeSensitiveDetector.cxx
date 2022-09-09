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
        track->SetTrackStatus(fStopAndKill);
        G4StepPoint* preStepPoint = step->GetPreStepPoint();
        G4StepPoint* postStepPoint = step->GetPostStepPoint();

        G4ThreeVector photonPosition = preStepPoint->GetPosition();
        // G4cout << "Position: " << photonPosition << std::endl;

        const G4VTouchable* touchable = step->GetPreStepPoint()->GetTouchable();
        G4int copyNo = touchable->GetCopyNumber();
        // G4cout << "Detector: " << copyNo << std::endl;

        G4VPhysicalVolume* physVol = touchable->GetVolume();
        G4ThreeVector posDetector = physVol->GetTranslation();

        // G4cout << "Detector pos: " << posDetector << std::endl;

        G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

        G4AnalysisManager *man = G4AnalysisManager::Instance();
        man->FillNtupleIColumn(0, evt);
        man->FillNtupleDColumn(1, photonPosition[0]);
        man->FillNtupleDColumn(2, photonPosition[1]);
        man->FillNtupleDColumn(3, photonPosition[2]);
        man->AddNtupleRow(0);
    }
}