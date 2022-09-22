/**
 * @file LArGeantEventAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "LArGeantEventAction.hh"

namespace largeant
{
    LArGeantEventAction::LArGeantEventAction(std::shared_ptr<LArGeantRunAction> runAction)
    : G4UserEventAction()
    {
        mRunAction = runAction;
    }

    LArGeantEventAction::~LArGeantEventAction()
    {}

    void LArGeantEventAction::BeginOfEventAction(const G4Event*)
    {
        mParticleName.clear();
        mParticleParentName.clear();
        mParticleParentTrackID.clear();
        mParticleAncestorName.clear();
        mParticleAncestorTrackID.clear();
    }

    void LArGeantEventAction::EndOfEventAction(const G4Event*)
    {
        mParticleName.clear();
        mParticleParentName.clear();
        mParticleParentTrackID.clear();
        mParticleAncestorName.clear();
        mParticleAncestorTrackID.clear();
    }
}