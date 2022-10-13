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

        // generate list of primaries
        auto Manager = EventManager::GetEventManager();
        auto primaries = Manager->GeneratePrimaryList();
        Manager->ClearParticleInfo();

        auto RunManager = G4RunManager::GetRunManager();
        auto Generator = (PrimaryGeneratorAction*)RunManager->GetUserPrimaryGeneratorAction();
        Generator->SetPrimaries(primaries);
    }

    EventAction::~EventAction()
    {}

    void EventAction::BeginOfEventAction(const G4Event*)
    {
        // generate list of primaries
        auto Manager = EventManager::GetEventManager();
        auto primaries = Manager->GeneratePrimaryList();
        Manager->ClearParticleInfo();

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
            for(size_t ii = 0; ii < Manager->GetNumberOfParticles(); ii++)
            {
                AnalysisManager->FillNtupleIColumn(index, 0, EventID);
                AnalysisManager->FillNtupleIColumn(index, 1, ii+1);
                AnalysisManager->FillNtupleSColumn(index, 2, Manager->GetParticleName(ii));
                AnalysisManager->FillNtupleIColumn(index, 3, Manager->GetParticlePDG(ii));
                AnalysisManager->FillNtupleIColumn(index, 4, Manager->GetParticleParentTrackID(ii));
                AnalysisManager->FillNtupleIColumn(index, 5, Manager->GetParticleAncestorTrackID(ii));
                AnalysisManager->FillNtupleIColumn(index, 6, Manager->GetScintillationParentTrackID(ii));
                AnalysisManager->AddNtupleRow(index);
            }
        }
        if(Manager->SavePrimaryInfo())
        {
            G4int index = Manager->GetIndex("Primaries");
            G4int EventID = event->GetEventID();
            auto primaries = Manager->GetPrimaries();
            for(size_t ii = 0; ii < primaries.size(); ii++)
            {
                AnalysisManager->FillNtupleIColumn(index, 0, EventID);
                AnalysisManager->FillNtupleIColumn(index, 1, primaries[ii].track_id);
                AnalysisManager->FillNtupleSColumn(index, 2, primaries[ii].name);
                AnalysisManager->FillNtupleIColumn(index, 3, primaries[ii].pdg);
                AnalysisManager->FillNtupleDColumn(index, 4, primaries[ii].init_t);
                AnalysisManager->FillNtupleDColumn(index, 5, primaries[ii].init_x);
                AnalysisManager->FillNtupleDColumn(index, 6, primaries[ii].init_y);
                AnalysisManager->FillNtupleDColumn(index, 7, primaries[ii].init_z);
                AnalysisManager->FillNtupleDColumn(index, 8, primaries[ii].init_energy);
                AnalysisManager->FillNtupleDColumn(index, 9, primaries[ii].final_t);
                AnalysisManager->FillNtupleDColumn(index, 10, primaries[ii].final_x);
                AnalysisManager->FillNtupleDColumn(index, 11, primaries[ii].final_y);
                AnalysisManager->FillNtupleDColumn(index, 12, primaries[ii].final_z);
                AnalysisManager->FillNtupleDColumn(index, 13, primaries[ii].final_energy);
                AnalysisManager->FillNtupleIColumn(index, 14, primaries[ii].number_of_daughters);
                AnalysisManager->FillNtupleDColumn(index, 15, primaries[ii].total_daughter_init_energy);
                AnalysisManager->FillNtupleDColumn(index, 16, primaries[ii].total_daughter_final_energy);
                AnalysisManager->FillNtupleDColumn(index, 17, primaries[ii].total_daughter_edep);
                AnalysisManager->FillNtupleIColumn(index, 18, primaries[ii].number_of_edeps);
                AnalysisManager->FillNtupleDColumn(index, 19, primaries[ii].total_edep);
                AnalysisManager->FillNtupleIColumn(index, 20, primaries[ii].number_of_photons);
                AnalysisManager->FillNtupleDColumn(index, 21, primaries[ii].total_optical_photon_init_energy);
                AnalysisManager->FillNtupleDColumn(index, 22, primaries[ii].total_optical_photon_final_energy);
                AnalysisManager->FillNtupleDColumn(index, 23, primaries[ii].total_optical_photon_edep);
                AnalysisManager->FillNtupleIColumn(index, 24, primaries[ii].number_of_electrons);
                AnalysisManager->FillNtupleDColumn(index, 25, primaries[ii].total_thermal_electron_init_energy);
                AnalysisManager->FillNtupleDColumn(index, 26, primaries[ii].total_thermal_electron_final_energy);
                AnalysisManager->FillNtupleDColumn(index, 27, primaries[ii].total_thermal_electron_edep);
                AnalysisManager->FillNtupleIColumn(index, 28, primaries[ii].number_of_hits);
                AnalysisManager->AddNtupleRow(index);
            }
        }
        Manager->ClearParticleInfo();
    }
}