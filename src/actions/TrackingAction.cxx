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
        auto Manager = EventManager::GetEventManager();
        Manager->StartFunctionProfile();
        Manager->AddParticleMapsFromTrack(track);
        Manager->AddPrimaryInfoFromTrackBegin(track);
        Manager->AddParticleInfoFromTrack(track);
        Manager->EndFunctionProfile("PreUserTrackingAction");
    }

    void TrackingAction::PostUserTrackingAction(const G4Track* track)
    {
        auto Manager = EventManager::GetEventManager();
        EventManager::GetEventManager()->StartFunctionProfile();
        Manager->AddPrimaryInfoFromTrackEnd(track);
        Manager->AddOpticalPhotonInfoFromTrackEnd(track);
        Manager->AddThermalElectronInfoFromTrackEnd(track);
        Manager->EndFunctionProfile("PostUserTrackingAction");
    }

}