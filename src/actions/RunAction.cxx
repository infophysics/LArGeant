/**
 * @file RunAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "RunAction.hh"

namespace LArGeant
{
    RunAction::RunAction()
    : G4UserRunAction()
    {
        auto Manager = EventManager::GetEventManager();
        auto AnalysisManager = G4AnalysisManager::Instance();
        AnalysisManager->SetDefaultFileType("root");
        AnalysisManager->SetVerboseLevel(0);
        AnalysisManager->SetNtupleMerging(true);

        if(Manager->SaveParticleInfo())
        {
            G4int index = Manager->GetIndex("Particle");
            AnalysisManager->CreateNtuple("Particle", "Particle");
            AnalysisManager->CreateNtupleIColumn("event");
            AnalysisManager->CreateNtupleIColumn("track_id");
            AnalysisManager->CreateNtupleSColumn("particle");
            AnalysisManager->CreateNtupleIColumn("pdg");
            AnalysisManager->CreateNtupleIColumn("parent_track_id");
            AnalysisManager->CreateNtupleIColumn("ancestor_track_id");
            AnalysisManager->FinishNtuple(index);
        }

        if(Manager->SaveHits())
        {
            G4int index = Manager->GetIndex("Hits");
            AnalysisManager->CreateNtuple("Hits", "Hits");
            AnalysisManager->CreateNtupleIColumn("event");
            AnalysisManager->CreateNtupleIColumn("copy_no");
            AnalysisManager->CreateNtupleDColumn("t_event");
            AnalysisManager->CreateNtupleIColumn("track_id");
            AnalysisManager->CreateNtupleDColumn("t_particle");
            AnalysisManager->CreateNtupleDColumn("x_particle");
            AnalysisManager->CreateNtupleDColumn("y_particle");
            AnalysisManager->CreateNtupleDColumn("z_particle");
            AnalysisManager->CreateNtupleDColumn("E_particle");
            AnalysisManager->CreateNtupleDColumn("px_particle");
            AnalysisManager->CreateNtupleDColumn("py_particle");
            AnalysisManager->CreateNtupleDColumn("pz_particle");
            AnalysisManager->FinishNtuple(index);
        }
        if(Manager->SaveOpticalPhotons())
        {
            G4int index = Manager->GetIndex("OpticalPhotons");
            AnalysisManager->CreateNtuple("OpticalPhotons", "OpticalPhotons");
            AnalysisManager->CreateNtupleDColumn("energy");
            AnalysisManager->CreateNtupleDColumn("track_length");
            AnalysisManager->FinishNtuple(index);
        }
        if(Manager->SaveThermalElectrons())
        {
            G4int index = Manager->GetIndex("ThermalElectrons");
            AnalysisManager->CreateNtuple("ThermalElectrons", "ThermalElectrons");
            AnalysisManager->CreateNtupleDColumn("energy");
            AnalysisManager->CreateNtupleDColumn("track_length");
            AnalysisManager->FinishNtuple(index);
        }
        if(Manager->SaveNEST())
        {
            G4int index = Manager->GetIndex("NEST");
            AnalysisManager->CreateNtuple("NEST", "NEST");
            AnalysisManager->CreateNtupleIColumn("number_of_photons");
            AnalysisManager->CreateNtupleIColumn("number_of_electrons");
            AnalysisManager->CreateNtupleSColumn("particle");
            AnalysisManager->CreateNtupleDColumn("init_x");
            AnalysisManager->CreateNtupleDColumn("init_y");
            AnalysisManager->CreateNtupleDColumn("init_z");
            AnalysisManager->CreateNtupleDColumn("final_x");
            AnalysisManager->CreateNtupleDColumn("final_y");
            AnalysisManager->CreateNtupleDColumn("final_z");
            AnalysisManager->CreateNtupleDColumn("init_t");
            AnalysisManager->CreateNtupleDColumn("energy");
            AnalysisManager->CreateNtupleDColumn("efield");
            AnalysisManager->CreateNtupleDColumn("density");
            AnalysisManager->CreateNtupleDColumn("electron_kinetic_energy");
            AnalysisManager->CreateNtupleDColumn("efield_x");
            AnalysisManager->CreateNtupleDColumn("efield_y");
            AnalysisManager->CreateNtupleDColumn("efield_z");
            AnalysisManager->FinishNtuple(index);
        }
    }

    RunAction::~RunAction()
    {}

    void RunAction::BeginOfRunAction(const G4Run* run)
    {
        auto Manager = EventManager::GetEventManager();
        auto AnalysisManager = G4AnalysisManager::Instance();
        G4String OutputFileName = Manager->OutputFileName();
        G4String RunID = std::to_string(run->GetRunID());
        G4String OutputFile = OutputFileName + "_" + RunID + ".root";
        G4bool fileopen = AnalysisManager->OpenFile(OutputFile);
        if (!fileopen) {
            G4cout << "File - " + OutputFile 
                << " - not opened!" << G4endl;
        }
        else {
            G4cout << "File - " + OutputFile
                << " - opened successfully." << G4endl;
        }
    }

    void RunAction::EndOfRunAction(const G4Run*)
    {
        auto AnalysisManager = G4AnalysisManager::Instance();
        AnalysisManager->Write();
        AnalysisManager->CloseFile();
    }
}