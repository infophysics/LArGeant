/**
 * @file SensitiveDetector.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "SensitiveDetector.hh"

namespace LArGeant
{
    SensitiveDetector::SensitiveDetector(G4String name)
    : G4VSensitiveDetector(name)
    , mName(name)
    {
    }

    SensitiveDetector::~SensitiveDetector()
    {
    }

    G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* history)
    { 
        const G4VTouchable* touchable = step->GetPreStepPoint()->GetTouchable();
        const G4Track* track = step->GetTrack();
        const G4StepPoint *preStepPoint = step->GetPreStepPoint();

        G4int           event = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
        G4int           copyNo = touchable->GetCopyNumber();
        G4double        globalTime = preStepPoint->GetGlobalTime();
        G4int           trackID = track->GetTrackID();
        G4double        localTime = preStepPoint->GetLocalTime();
        G4ThreeVector   particlePosition = preStepPoint->GetPosition();
        G4double        energy = preStepPoint->GetTotalEnergy();
        G4ThreeVector   particleMomentum = preStepPoint->GetMomentum();

        auto Manager = EventManager::GetEventManager();
        G4bool detected_hit = Manager->GetComponentFromCopyNumber(copyNo)->ProcessHits(step, history);
        if (Manager->SaveHits())
        {
            G4int index = Manager->GetIndex("Hits");
            auto AnalysisManager = G4AnalysisManager::Instance();
            AnalysisManager->FillNtupleIColumn(index, 0, event);
            AnalysisManager->FillNtupleIColumn(index, 1, copyNo);
            AnalysisManager->FillNtupleDColumn(index, 2, globalTime);
            AnalysisManager->FillNtupleIColumn(index, 3, trackID);
            AnalysisManager->FillNtupleDColumn(index, 4, localTime);
            AnalysisManager->FillNtupleDColumn(index, 5, particlePosition[0]);
            AnalysisManager->FillNtupleDColumn(index, 6, particlePosition[1]);
            AnalysisManager->FillNtupleDColumn(index, 7, particlePosition[2]);
            AnalysisManager->FillNtupleDColumn(index, 8, energy);
            AnalysisManager->FillNtupleDColumn(index, 9, particleMomentum[0]);
            AnalysisManager->FillNtupleDColumn(index, 10, particleMomentum[1]);
            AnalysisManager->FillNtupleDColumn(index, 11, particleMomentum[2]);
            AnalysisManager->FillNtupleIColumn(index, 12, detected_hit);
            AnalysisManager->AddNtupleRow(index);
        }
        return true;
    }
}