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
        mParticleName.clear();
        mParticlePDG.clear();
        mParticleParentTrackID.clear();
        mParticleAncestorTrackID.clear();
        mPrimaryData.clear();

        // generate list of primaries
        auto Manager = EventManager::GetEventManager();
        auto primaries = Manager->GeneratePrimaryList();

        auto RunManager = G4RunManager::GetRunManager();
        auto Generator = (PrimaryGeneratorAction*)RunManager->GetUserPrimaryGeneratorAction();
        Generator->SetPrimaries(primaries);
    }

    EventAction::~EventAction()
    {}

    PrimaryData& EventAction::GetPrimaryData(G4int track_id)
    {
        for(size_t ii = 0; ii < mPrimaryData.size(); ii++)
        {
            if(mPrimaryData[ii].track_id == track_id) {
                return mPrimaryData[ii];
            }
        }
        throw std::invalid_argument(
            "No PrimaryData object matches track_id: " + std::to_string(track_id)
            + ". Creating and returning empty PrimaryData."
        );
    }

    void EventAction::BeginOfEventAction(const G4Event*)
    {
        mParticleName.clear();
        mParticlePDG.clear();
        mParticleParentTrackID.clear();
        mParticleAncestorTrackID.clear();
        mPrimaryData.clear();

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
            for(size_t ii = 0; ii < mParticleName.size(); ii++)
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
        if(Manager->SavePrimaryInfo())
        {
            G4int index = Manager->GetIndex("Primaries");
            G4int EventID = event->GetEventID();
            for(size_t ii = 0; ii < mPrimaryData.size(); ii++)
            {
                AnalysisManager->FillNtupleIColumn(index, 0, EventID);
                AnalysisManager->FillNtupleIColumn(index, 1, mPrimaryData[ii].track_id);
                AnalysisManager->FillNtupleSColumn(index, 2, mPrimaryData[ii].name);
                AnalysisManager->FillNtupleIColumn(index, 3, mPrimaryData[ii].pdg);
                AnalysisManager->FillNtupleDColumn(index, 4, mPrimaryData[ii].init_t);
                AnalysisManager->FillNtupleDColumn(index, 5, mPrimaryData[ii].init_x);
                AnalysisManager->FillNtupleDColumn(index, 6, mPrimaryData[ii].init_y);
                AnalysisManager->FillNtupleDColumn(index, 7, mPrimaryData[ii].init_z);
                AnalysisManager->FillNtupleDColumn(index, 8, mPrimaryData[ii].init_energy);
                AnalysisManager->FillNtupleDColumn(index, 9, mPrimaryData[ii].final_t);
                AnalysisManager->FillNtupleDColumn(index, 10, mPrimaryData[ii].final_x);
                AnalysisManager->FillNtupleDColumn(index, 11, mPrimaryData[ii].final_y);
                AnalysisManager->FillNtupleDColumn(index, 12, mPrimaryData[ii].final_z);
                AnalysisManager->FillNtupleDColumn(index, 13, mPrimaryData[ii].final_energy);
                AnalysisManager->FillNtupleIColumn(index, 14, mPrimaryData[ii].number_of_daughters);
                AnalysisManager->FillNtupleDColumn(index, 15, mPrimaryData[ii].total_daughter_init_energy);
                AnalysisManager->FillNtupleDColumn(index, 16, mPrimaryData[ii].total_daughter_final_energy);
                AnalysisManager->FillNtupleDColumn(index, 17, mPrimaryData[ii].total_daughter_edep);
                AnalysisManager->FillNtupleIColumn(index, 18, mPrimaryData[ii].number_of_edeps);
                AnalysisManager->FillNtupleDColumn(index, 19, mPrimaryData[ii].total_edep);
                AnalysisManager->FillNtupleIColumn(index, 20, mPrimaryData[ii].number_of_photons);
                AnalysisManager->FillNtupleDColumn(index, 21, mPrimaryData[ii].total_optical_photon_init_energy);
                AnalysisManager->FillNtupleDColumn(index, 22, mPrimaryData[ii].total_optical_photon_final_energy);
                AnalysisManager->FillNtupleDColumn(index, 23, mPrimaryData[ii].total_optical_photon_edep);
                AnalysisManager->FillNtupleIColumn(index, 24, mPrimaryData[ii].number_of_electrons);
                AnalysisManager->FillNtupleDColumn(index, 25, mPrimaryData[ii].total_thermal_electron_init_energy);
                AnalysisManager->FillNtupleDColumn(index, 26, mPrimaryData[ii].total_thermal_electron_final_energy);
                AnalysisManager->FillNtupleDColumn(index, 27, mPrimaryData[ii].total_thermal_electron_edep);
                AnalysisManager->FillNtupleIColumn(index, 28, mPrimaryData[ii].number_of_hits);
                AnalysisManager->AddNtupleRow(index);
            }
        }
        mParticleName.clear();
        mParticlePDG.clear();
        mParticleParentTrackID.clear();
        mParticleAncestorTrackID.clear();
        mPrimaryData.clear();
    }
}