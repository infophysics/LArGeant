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

    void EventAction::BeginOfEventAction(const G4Event* event)
    {
        mParticleName.clear();
        mParticlePDG.clear();
        mParticleParentTrackID.clear();
        mParticleAncestorTrackID.clear();

        // generate list of primaries
        auto Manager = EventManager::GetEventManager();
        auto primaries = Manager->GeneratePrimaryList();

        auto RunManager = G4RunManager::GetRunManager();
        auto Generator = (PrimaryGeneratorAction*)RunManager->GetUserPrimaryGeneratorAction();
        Generator->SetPrimaries(primaries);
    }

    void EventAction::EndOfEventAction(const G4Event* event)
    {
        auto Manager = EventManager::GetEventManager();
        auto AnalysisManager = G4AnalysisManager::Instance();
        if(Manager->SaveParticleInfo())
        {
            G4int index = Manager->GetIndex("Particle");
            G4int EventID = event->GetEventID();
            for(G4int ii = 0; ii < mParticleName.size(); ii++)
            {
                AnalysisManager->FillNtupleIColumn(index, 0, EventID);
                AnalysisManager->FillNtupleIColumn(index, 1, ii+1);
                AnalysisManager->FillNtupleSColumn(index, 2, mParticleName[ii]);
                AnalysisManager->FillNtupleIColumn(index, 3, mParticlePDG[ii]);
                AnalysisManager->FillNtupleIColumn(index, 4, mParticleParentTrackID[ii]);
                AnalysisManager->FillNtupleIColumn(index, 5, mParticleAncestorTrackID[ii]);
                AnalysisManager->AddNtupleRow(index);
            }
        }
        mParticleName.clear();
        mParticlePDG.clear();
        mParticleParentTrackID.clear();
        mParticleAncestorTrackID.clear();
    }
}