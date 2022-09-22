/**
 * @file LArGeantRunAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "LArGeantRunAction.hh"

namespace largeant
{
    LArGeantRunAction::LArGeantRunAction()
    : G4UserRunAction()
    {
        // mMessenger = new G4GenericMessenger(this,
		// 		      "/largeant/",
		// 		      "Control of n-tuple quantities");
        // mMessenger->DeclareMethod("electric_field", &LArGeantRunAction::SetElectricField);

        // auto analysisManager = G4AnalysisManager::Instance();
        // analysisManager->SetDefaultFileType("root");
        // analysisManager->SetVerboseLevel(0);
        // analysisManager->SetNtupleMerging(true);

        // analysisManager->CreateNtuple("NEST", "NEST");
        // analysisManager->CreateNtupleIColumn("event");
        // analysisManager->CreateNtupleSColumn("particle_name");
        // analysisManager->CreateNtupleIColumn("particle_track_id");
        // analysisManager->CreateNtupleSColumn("parent_name");
        // analysisManager->CreateNtupleIColumn("parent_track_id");
        // analysisManager->CreateNtupleSColumn("ancestor_name");
        // analysisManager->CreateNtupleIColumn("ancestor_track_id");
        // analysisManager->CreateNtupleDColumn("efield");
        // analysisManager->CreateNtupleDColumn("dE");
        // analysisManager->CreateNtupleIColumn("dQ");
        // analysisManager->CreateNtupleDColumn("dx");
        // analysisManager->CreateNtupleDColumn("dEdx");
        // analysisManager->CreateNtupleDColumn("x_i");
        // analysisManager->CreateNtupleDColumn("y_i");
        // analysisManager->CreateNtupleDColumn("z_i");
        // analysisManager->CreateNtupleDColumn("x_f");
        // analysisManager->CreateNtupleDColumn("y_f");
        // analysisManager->CreateNtupleDColumn("z_f");
        // analysisManager->CreateNtupleIColumn("LightYield");
        // analysisManager->CreateNtupleIColumn("ChargeYield");
        // analysisManager->FinishNtuple(0);

        // analysisManager->CreateNtuple("bottom_sipm", "bottom_sipm");
        // analysisManager->CreateNtupleIColumn("event");
        // analysisManager->CreateNtupleIColumn("track_id");
        // analysisManager->CreateNtupleDColumn("x_i");
        // analysisManager->CreateNtupleDColumn("y_i");
        // analysisManager->CreateNtupleDColumn("z_i");
        // analysisManager->CreateNtupleDColumn("energy");
        // analysisManager->CreateNtupleDColumn("wavelength");
        // analysisManager->CreateNtupleIColumn("sipm_channel");
        // analysisManager->CreateNtupleDColumn("sipm_x");
        // analysisManager->CreateNtupleDColumn("sipm_y");
        // analysisManager->CreateNtupleDColumn("sipm_z");
        // analysisManager->FinishNtuple(1);
    }

    LArGeantRunAction::~LArGeantRunAction()
    {}

    void LArGeantRunAction::BeginOfRunAction(const G4Run* run)
    {
        // auto analysisManager = G4AnalysisManager::Instance();

        // G4int runID = run->GetRunID();
        // std::stringstream strRunID;
        // strRunID << runID;

        // G4bool fileopen = analysisManager->OpenFile("output_"+strRunID.str() + ".root");
        // if (!fileopen)
        // {
        //      G4cout << "File - output_" + strRunID.str() + ".root - not opened!" << G4endl;
        // }
        // else
        // {
        //     G4cout << "File - output_" + strRunID.str() + ".root - opened successfully." << G4endl;
        // }
    }

    void LArGeantRunAction::EndOfRunAction(const G4Run*)
    {
        // auto analysisManager = G4AnalysisManager::Instance();
        // analysisManager->Write();
        // analysisManager->CloseFile();
    }
}