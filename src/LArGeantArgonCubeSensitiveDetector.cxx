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

        // get the photon position
        G4ThreeVector photonPosition = preStepPoint->GetPosition();
        G4ThreeVector photonMomentum = preStepPoint->GetMomentum();
        G4double wavelength = (1.239841939 * eV / photonMomentum.mag()) * 1000;
        
        // get the detector position
        const G4VTouchable* touchable = step->GetPreStepPoint()->GetTouchable();
        G4int copyNo = touchable->GetCopyNumber();
        G4VPhysicalVolume* physVol = touchable->GetVolume();
        G4ThreeVector detectorPosition = physVol->GetTranslation();

        // get event number
        G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

        G4AnalysisManager *man = G4AnalysisManager::Instance();

        man->FillNtupleIColumn(0, 0, evt);
        man->FillNtupleDColumn(0, 1, photonPosition[0]);
        man->FillNtupleDColumn(0, 2, photonPosition[1]);
        man->FillNtupleDColumn(0, 3, photonPosition[2]);
        man->FillNtupleDColumn(0, 4, wavelength);
        man->AddNtupleRow(0);

        man->FillNtupleIColumn(1, 0, evt);
        man->FillNtupleDColumn(1, 1, detectorPosition[0]);
        man->FillNtupleDColumn(1, 2, detectorPosition[1]);
        man->FillNtupleDColumn(1, 3, detectorPosition[2]);
        man->AddNtupleRow(1);
        
        return true;
    }
}