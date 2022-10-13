/**
 * @file SteppingAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "SteppingAction.hh"

namespace LArGeant
{
    SteppingAction::SteppingAction(std::shared_ptr<EventAction> eventAction)
    : G4UserSteppingAction()
    {
        mEventAction = eventAction;
    }

    SteppingAction::~SteppingAction()
    {}

    void SteppingAction::UserSteppingAction(const G4Step *step)
    {   
        if(step->GetTrack()->GetGlobalTime() >= EventManager::GetEventManager()->EventMaxTime()) {
            step->GetTrack()->SetTrackStatus(fStopAndKill);
        }
        G4VPhysicalVolume* physicalVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
        G4LogicalVolume *logicalVolume = physicalVolume->GetLogicalVolume();
        G4Track* track = step->GetTrack();
        G4StepPoint* pre_step_point = step->GetPreStepPoint();
        G4StepPoint* post_step_point = step->GetPostStepPoint();
        const G4VProcess* pre_process = pre_step_point->GetProcessDefinedStep();
        const G4VProcess* post_process = post_step_point->GetProcessDefinedStep();

        G4int           event = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
        G4String        particle_name = step->GetTrack()->GetParticleDefinition()->GetParticleName();
        G4int           particle_pdg = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
        G4ThreeVector   particleStartPosition = step->GetPreStepPoint()->GetPosition();
        G4double        local_time = track->GetLocalTime();
        G4double        global_time = track->GetGlobalTime();
        G4double        pre_step_energy = pre_step_point->GetKineticEnergy();
        G4double        post_step_energy = post_step_point->GetKineticEnergy();
        G4int           track_status = (int)track->GetTrackStatus();
        G4int           track_id = track->GetTrackID();
        G4int           parent_track_id = track->GetParentID();
        G4ThreeVector   pre_step_position = pre_step_point->GetPosition();
        G4ThreeVector   post_step_position = post_step_point->GetPosition();
        G4double        edep = step->GetTotalEnergyDeposit();

        auto Manager = EventManager::GetEventManager();
        auto AnalysisManager = G4AnalysisManager::Instance();
        if(
            Manager->SavePrimaryInfo() && 
            Manager->GetParticleParentTrackID(track_id) == 0 &&
            edep > 0
        )
        {
            if(parent_track_id == 0) {
                Manager->GetPrimaryData(track_id).number_of_edeps += 1;
                Manager->GetPrimaryData(track_id).total_edep += edep;
            }
            else if(particle_name == "opticalphoton") {
                Manager->GetPrimaryData(Manager->GetParticleAncestorTrackID(track_id)).total_optical_photon_edep += edep;
            }
            else if(particle_name == "thermalelectron") {
                Manager->GetPrimaryData(Manager->GetParticleAncestorTrackID(track_id)).total_thermal_electron_edep += edep;
            }
            else {
                Manager->GetPrimaryData(Manager->GetParticleAncestorTrackID(track_id)).total_daughter_edep += edep;
            }
        }
        if(
            Manager->SaveEnergyDepositions() && 
            edep > 0
        )
        {
            G4int index = Manager->GetIndex("EnergyDepositions");
            AnalysisManager->FillNtupleIColumn(index, 0, event);
            AnalysisManager->FillNtupleIColumn(index, 1, track_id);
            AnalysisManager->FillNtupleSColumn(index, 2, particle_name);
            AnalysisManager->FillNtupleIColumn(index, 3, particle_pdg);
            AnalysisManager->FillNtupleDColumn(index, 4, local_time);
            AnalysisManager->FillNtupleDColumn(index, 5, global_time);
            AnalysisManager->FillNtupleDColumn(index, 6, pre_step_position[0]);
            AnalysisManager->FillNtupleDColumn(index, 7, pre_step_position[1]);
            AnalysisManager->FillNtupleDColumn(index, 8, pre_step_position[2]);
            AnalysisManager->FillNtupleDColumn(index, 9, edep);
            AnalysisManager->AddNtupleRow(index);
        }
    }
}