/**
 * @file LArGeantSteppingAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "LArGeantSteppingAction.hh"

namespace largeant
{
    LArGeantSteppingAction::LArGeantSteppingAction(std::shared_ptr<LArGeantEventAction> eventAction)
    : G4UserSteppingAction()
    {
        mEventAction = eventAction;
        LArDetector* detector = new LArDetector();
        mLArNEST = std::make_shared<NEST::LArNEST>(detector);
    }

    LArGeantSteppingAction::~LArGeantSteppingAction()
    {}

    void LArGeantSteppingAction::UserSteppingAction(const G4Step *step)
    {   
        // if (step->GetPreStepPoint()->GetGlobalTime() >= mEventAction->GetStoppingTime())
        // {
        //     step->GetTrack()->SetTrackStatus(fStopAndKill);
        //     return;
        // }
        // const LArGeantArgonCubeDetector *detectorConstruction 
        //     = static_cast<const LArGeantArgonCubeDetector*> 
        //     (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

        // // get current volumes    
        // G4VPhysicalVolume* physicalVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
        // G4LogicalVolume *logicalVolume = physicalVolume->GetLogicalVolume();
        // G4LogicalVolume *scoringVolume = detectorConstruction->GetScoringVolume().get();

        // G4Track* track = step->GetTrack();
        // G4int track_id = track->GetTrackID();
        // // collect t=0 information mor all particles
        // if (step->IsFirstStepInVolume())
        // {
        //     G4int parent_track_id = track->GetParentID();
        //     G4String particle_name = track->GetParticleDefinition()->GetParticleName();

        //     mEventAction->SetParticleName(track_id, particle_name);
        //     mEventAction->SetParticleParentTrackID(track_id, parent_track_id);
        //     if (parent_track_id == 0)
        //     {
        //         mEventAction->SetParticleParentName(track_id, "none");
        //         mEventAction->SetParticleAncestorName(track_id, "none");
        //         mEventAction->SetParticleAncestorTrackID(track_id, 0);
        //     }
        //     else if (fEventAction->GetParticleParentTrackID(parent_track_id) == 0)
        //     {
        //         mEventAction->SetParticleParentName(track_id, mEventAction->GetParticleName(parent_track_id));
        //         mEventAction->SetParticleAncestorName(track_id, mEventAction->GetParticleName(parent_track_id));
        //         mEventAction->SetParticleAncestorTrackID(track_id, parent_track_id);
        //     }
        //     else
        //     {
        //         mEventAction->SetParticleParentName(track_id, mEventAction->GetParticleName(parent_track_id));
        //         mEventAction->SetParticleAncestorName(track_id, mEventAction->GetParticleAncestorName(parent_track_id));
        //         mEventAction->SetParticleAncestorTrackID(track_id, mEventAction->GetParticleAncestorTrackID(parent_track_id));
        //     }
        // }

        // // check that edep is in the active volume
        // if (logicalVolume != scoringVolume) {
        //     return;
        // }

        // // get edep value in keV (for NEST)
        // G4double edep = step->GetTotalEnergyDeposit()/keV;
        // G4double dx = step->GetStepLength()/mm;

        // // get efield value
        // G4double efield = mEventAction->GetElectricField();

        // // get particle info
        // G4AnalysisManager *man = G4AnalysisManager::Instance();
        // G4int event = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
        // G4String particle = step->GetTrack()->GetParticleDefinition()->GetParticleName();
        // G4StepPoint* preStepPoint = step->GetPreStepPoint();
        // G4StepPoint* postStepPoint = step->GetPostStepPoint();

        // // get the particle position
        // G4ThreeVector particleStartPosition = preStepPoint->GetPosition();
        // G4ThreeVector particleEndPosition = postStepPoint->GetPosition();
        // G4ThreeVector particleMomentum = preStepPoint->GetMomentum();

        // // run NEST
        // NEST::LArInteraction interaction = GetLArInteraction(particle);
        // NEST::LArNESTResult result = mLArNEST->FullCalculation(
        //     interaction,
        //     edep,
        //     efield,
        //     1.393,
        //     false
        // );

        // if (std::isnan(result.fluctuations.NphFluctuation) || std::isnan(result.fluctuations.NeFluctuation)) {
        //     return;
        // }

        // const std::vector<const G4Track*>* daughters = step->GetSecondaryInCurrentStep();
        // G4int dQ = 0;
        // for (size_t ii = 0; ii < daughters->size(); ii++)
        // {
        //     dQ += (*daughters)[ii]->GetParticleDefinition()->GetPDGCharge();
        // }

        // // save nest results
        // man->FillNtupleIColumn(0, 0, event);
        // man->FillNtupleSColumn(0, 1, particle);
        // man->FillNtupleIColumn(0, 2, track_id);
        // man->FillNtupleSColumn(0, 3, mEventAction->GetParticleParentName(track_id));
        // man->FillNtupleIColumn(0, 4, mEventAction->GetParticleParentTrackID(track_id));
        // man->FillNtupleSColumn(0, 5, mEventAction->GetParticleAncestorName(track_id));
        // man->FillNtupleIColumn(0, 6, mEventAction->GetParticleAncestorTrackID(track_id));
        // man->FillNtupleDColumn(0, 7, efield);
        // man->FillNtupleDColumn(0, 8, edep);
        // man->FillNtupleIColumn(0, 9, dQ);
        // man->FillNtupleDColumn(0, 10, dx);
        // if (!std::isnan(edep/dx)) {
        //     man->FillNtupleDColumn(0, 11, edep/dx);
        // }
        // else {
        //     man->FillNtupleDColumn(0, 11, 0);
        // }
        // man->FillNtupleDColumn(0, 12, particleStartPosition[0]);
        // man->FillNtupleDColumn(0, 13, particleStartPosition[1]);
        // man->FillNtupleDColumn(0, 14, particleStartPosition[2]);
        // man->FillNtupleDColumn(0, 15, particleEndPosition[0]);
        // man->FillNtupleDColumn(0, 16, particleEndPosition[1]);
        // man->FillNtupleDColumn(0, 17, particleEndPosition[2]);
        // man->FillNtupleIColumn(0, 18, int(result.fluctuations.NphFluctuation));
        // man->FillNtupleIColumn(0, 19, int(result.fluctuations.NeFluctuation));
        // man->AddNtupleRow(0);
    }
}