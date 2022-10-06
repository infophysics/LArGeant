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
        LArDetector* detector = new LArDetector();
        mLArNEST = std::make_shared<NEST::LArNEST>(detector);
        mOpticalLengths = 0.0;
        mOpticalNum = 0;
    }

    SteppingAction::~SteppingAction()
    {}

    void SteppingAction::UserSteppingAction(const G4Step *step)
    {   
        G4VPhysicalVolume* physicalVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
        G4LogicalVolume *logicalVolume = physicalVolume->GetLogicalVolume();

        G4String particle_name = step->GetTrack()->GetParticleDefinition()->GetParticleName();
        G4int particle_pdg = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
        G4ThreeVector particleStartPosition = step->GetPreStepPoint()->GetPosition();

        G4StepPoint* preStepPoint = step->GetPreStepPoint();
        G4StepPoint* postStepPoint = step->GetPostStepPoint();
        const G4VProcess* preProcess = preStepPoint->GetProcessDefinedStep();
        const G4VProcess* postProcess = postStepPoint->GetProcessDefinedStep();

        auto Manager = EventManager::GetEventManager();
        auto AnalysisManager = G4AnalysisManager::Instance();
        if (particle_name == "opticalphoton" && int(step->GetTrack()->GetTrackStatus()) == 2)
        {
            if(Manager->SaveOpticalPhotons())
            {
                G4int index = Manager->GetIndex("OpticalPhotons");
                AnalysisManager->FillNtupleDColumn(index, 0, step->GetTrack()->GetTotalEnergy());
                AnalysisManager->FillNtupleDColumn(index, 1, step->GetTrack()->GetTrackLength()/cm);
                AnalysisManager->AddNtupleRow(index);
            }
        }
        if (particle_name == "thermalelectron" && int(step->GetTrack()->GetTrackStatus()) == 2)
        {
            if(Manager->SaveThermalElectrons())
            {
                G4int index = Manager->GetIndex("ThermalElectrons");
                AnalysisManager->FillNtupleDColumn(index, 0, step->GetTrack()->GetTotalEnergy());
                AnalysisManager->FillNtupleDColumn(index, 1, step->GetTrack()->GetTrackLength()/cm);
                AnalysisManager->AddNtupleRow(index);
            }
        }
        
        // Save Particle Info
        if(Manager->SaveParticleInfo())
        {
            G4Track* track = step->GetTrack();
            G4int track_id = track->GetTrackID(); 
            if (step->IsFirstStepInVolume())
            {
                G4int parent_track_id = track->GetParentID();
                G4String particle_name = track->GetParticleDefinition()->GetParticleName();

                mEventAction->SetParticleName(particle_name);
                mEventAction->SetParticlePDG(particle_pdg);
                mEventAction->SetParticleParentTrackID(parent_track_id);
                if (parent_track_id == 0) {
                    mEventAction->SetParticleAncestorTrackID(0);
                }
                else if (mEventAction->GetParticleParentTrackID(parent_track_id) == 0) {
                    mEventAction->SetParticleAncestorTrackID(parent_track_id);
                }
                else {
                    mEventAction->SetParticleAncestorTrackID(mEventAction->GetParticleAncestorTrackID(parent_track_id));
                }
            }
        }
    }
}