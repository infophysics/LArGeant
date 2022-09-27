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
        G4cout << step->GetTrack()->GetParticleDefinition()->GetParticleName() << G4endl;

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

        auto AnalysisManager = G4AnalysisManager::Instance();
        AnalysisManager->FillNtupleIColumn(0, 0, event);
        AnalysisManager->FillNtupleIColumn(0, 1, copyNo);
        AnalysisManager->FillNtupleDColumn(0, 2, globalTime);
        AnalysisManager->FillNtupleIColumn(0, 3, trackID);
        AnalysisManager->FillNtupleDColumn(0, 4, localTime);
        AnalysisManager->FillNtupleDColumn(0, 5, particlePosition[0]);
        AnalysisManager->FillNtupleDColumn(0, 6, particlePosition[1]);
        AnalysisManager->FillNtupleDColumn(0, 7, particlePosition[2]);
        AnalysisManager->FillNtupleDColumn(0, 8, energy);
        AnalysisManager->FillNtupleDColumn(0, 9, particleMomentum[0]);
        AnalysisManager->FillNtupleDColumn(0, 10, particleMomentum[1]);
        AnalysisManager->FillNtupleDColumn(0, 11, particleMomentum[2]);
        AnalysisManager->AddNtupleRow(0);

        return true;
    }
}