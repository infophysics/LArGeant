/**
 * @file EventAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "EventAction.hh"

namespace LArGeant
{
    EventAction::EventAction(std::shared_ptr<RunAction> runAction)
    : G4UserEventAction()
    {
        mRunAction = runAction;
    }

    EventAction::~EventAction()
    {}

    void EventAction::BeginOfEventAction(const G4Event*)
    {
        mParticleName.clear();
        mParticleParentName.clear();
        mParticleParentTrackID.clear();
        mParticleAncestorName.clear();
        mParticleAncestorTrackID.clear();

        // generate list of primaries
        
    }

    void EventAction::EndOfEventAction(const G4Event*)
    {
        mParticleName.clear();
        mParticleParentName.clear();
        mParticleParentTrackID.clear();
        mParticleAncestorName.clear();
        mParticleAncestorTrackID.clear();
    }
}