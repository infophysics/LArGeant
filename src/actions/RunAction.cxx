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
        auto AnalysisManager = G4AnalysisManager::Instance();
        AnalysisManager->SetDefaultFileType("root");
        AnalysisManager->SetVerboseLevel(0);
        AnalysisManager->SetNtupleMerging(true);
 
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
        AnalysisManager->FinishNtuple(0);

    }

    RunAction::~RunAction()
    {}

    void RunAction::BeginOfRunAction(const G4Run* run)
    {
        auto AnalysisManager = G4AnalysisManager::Instance();

        G4int runID = run->GetRunID();
        std::stringstream strRunID;
        strRunID << runID;

        G4bool fileopen = AnalysisManager->OpenFile("output_"+strRunID.str() + ".root");
        if (!fileopen)
        {
             G4cout << "File - output_" + strRunID.str() + ".root - not opened!" << G4endl;
        }
        else
        {
            G4cout << "File - output_" + strRunID.str() + ".root - opened successfully." << G4endl;
        }
    }

    void RunAction::EndOfRunAction(const G4Run*)
    {
        auto AnalysisManager = G4AnalysisManager::Instance();
        AnalysisManager->Write();
        AnalysisManager->CloseFile();
    }
}