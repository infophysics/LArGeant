/**
 * @file TrackingAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "TrackingAction.hh"

namespace LArGeant
{
    TrackingAction::TrackingAction(std::shared_ptr<EventAction> eventAction)
    : G4UserTrackingAction()
    {
        mEventAction = eventAction;
    }

    TrackingAction::~TrackingAction()
    {}

    void TrackingAction::PreUserTrackingAction(const G4Track* track)
    {
        EventManager::GetEventManager()->StartFunctionProfile();
        G4String particle_name = track->GetParticleDefinition()->GetParticleName();
        G4int particle_pdg = track->GetParticleDefinition()->GetPDGEncoding();
        G4ThreeVector particle_position = track->GetVertexPosition();

        G4double local_time = track->GetLocalTime();
        G4double global_time = track->GetGlobalTime();
        G4double kinetic_energy = track->GetKineticEnergy();
        G4int track_status = (int)track->GetTrackStatus();
        G4int track_id = track->GetTrackID();
        G4int parent_track_id = track->GetParentID();

        auto Manager = EventManager::GetEventManager();
        auto AnalysisManager = G4AnalysisManager::Instance();

        // Save Particle Info
        EventManager::GetEventManager()->StartFunctionProfile();
        mEventAction->AddParticleName(track_id, particle_name);
        mEventAction->AddParticlePDG(track_id, particle_pdg);
        mEventAction->AddParticleParentTrackID(track_id, parent_track_id);
        if (parent_track_id == 0) {
            mEventAction->AddParticleAncestorTrackID(track_id, 0);
        }
        else if (mEventAction->GetParticleParentTrackID(parent_track_id) == 0) {
            mEventAction->AddParticleAncestorTrackID(track_id, parent_track_id);
        }
        else {
            mEventAction->AddParticleAncestorTrackID(track_id, mEventAction->GetParticleAncestorTrackID(parent_track_id));
        }
        Manager->EndFunctionProfile("PreUserTrackingAction_SaveParticleInfo");

        if(Manager->SavePrimaryInfo())
        {
            EventManager::GetEventManager()->StartFunctionProfile();
            if(parent_track_id == 0)
            {
                mEventAction->AddPrimaryData(
                    PrimaryData(
                        particle_name, track_id, particle_pdg,
                        global_time, particle_position[0], particle_position[1],
                        particle_position[2], kinetic_energy 
                    )
                );
            }
            else
            {
                if(particle_name == "opticalphoton")
                {
                    mEventAction->GetPrimaryData(mEventAction->GetParticleAncestorTrackID(track_id)).number_of_photons += 1;
                    mEventAction->GetPrimaryData(mEventAction->GetParticleAncestorTrackID(track_id)).total_optical_photon_init_energy += kinetic_energy;
                }
                else if(particle_name == "thermalelectron")
                {
                    mEventAction->GetPrimaryData(mEventAction->GetParticleAncestorTrackID(track_id)).number_of_electrons += 1;
                    mEventAction->GetPrimaryData(mEventAction->GetParticleAncestorTrackID(track_id)).total_thermal_electron_init_energy += kinetic_energy;
                }
                else
                {
                    mEventAction->GetPrimaryData(mEventAction->GetParticleAncestorTrackID(track_id)).total_daughter_init_energy += kinetic_energy;
                    mEventAction->GetPrimaryData(mEventAction->GetParticleAncestorTrackID(track_id)).number_of_daughters += 1;
                }
            }
            Manager->EndFunctionProfile("PreUserTrackingAction_SavePrimaryInfo");
        }
        Manager->EndFunctionProfile("PreUserTrackingAction");
    }

    void TrackingAction::PostUserTrackingAction(const G4Track* track)
    {
        EventManager::GetEventManager()->StartFunctionProfile();
        G4String particle_name = track->GetParticleDefinition()->GetParticleName();
        G4ThreeVector particle_position = track->GetVertexPosition();
        G4double kinetic_energy = track->GetKineticEnergy();
        G4int track_status = (int)track->GetTrackStatus();
        G4int track_id = track->GetTrackID();
        G4double global_time = track->GetGlobalTime();

        auto Manager = EventManager::GetEventManager();
        auto AnalysisManager = G4AnalysisManager::Instance();
        if(Manager->SavePrimaryInfo())
        {
            EventManager::GetEventManager()->StartFunctionProfile();
            if(mEventAction->GetParticleParentTrackID(track_id) == 0)
            {
                mEventAction->GetPrimaryData(track_id).AddFinalTrackData(
                    global_time, particle_position[0], particle_position[1],
                    particle_position[2], kinetic_energy
                );
            }
            else if(particle_name == "opticalphoton") {
                mEventAction->GetPrimaryData(mEventAction->GetParticleAncestorTrackID(track_id)).total_optical_photon_final_energy += kinetic_energy;
            }
            else if (particle_name == "thermalelectron") {
                mEventAction->GetPrimaryData(mEventAction->GetParticleAncestorTrackID(track_id)).total_thermal_electron_final_energy += kinetic_energy;
            }
            else {
                mEventAction->GetPrimaryData(mEventAction->GetParticleAncestorTrackID(track_id)).total_daughter_final_energy += kinetic_energy;
            }
            Manager->EndFunctionProfile("PostUserTrackingAction_SavePrimaryInfo");
        }

        if (particle_name == "opticalphoton")
        {
            if(Manager->SaveOpticalPhotons())
            {
                EventManager::GetEventManager()->StartFunctionProfile();
                G4int index = Manager->GetIndex("OpticalPhotons");
                AnalysisManager->FillNtupleDColumn(index, 0, track->GetTotalEnergy());
                AnalysisManager->FillNtupleDColumn(index, 1, track->GetTrackLength()/cm);
                AnalysisManager->AddNtupleRow(index);
                Manager->EndFunctionProfile("PostUserTrackingAction_SaveOpticalPhotons");
            }
        }
        if (particle_name == "thermalelectron")
        {
            if(Manager->SaveThermalElectrons())
            {
                EventManager::GetEventManager()->StartFunctionProfile();
                G4int index = Manager->GetIndex("ThermalElectrons");
                AnalysisManager->FillNtupleDColumn(index, 0, track->GetTotalEnergy());
                AnalysisManager->FillNtupleDColumn(index, 1, track->GetTrackLength()/cm);
                AnalysisManager->AddNtupleRow(index);
                Manager->EndFunctionProfile("PostUserTrackingAction_SaveThermalElectrons");
            }
        }
        Manager->EndFunctionProfile("PostUserTrackingAction");
    }

}