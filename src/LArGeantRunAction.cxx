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
    LArGeantRunAction::LArGeantRunAction(G4double efield)
    : G4UserRunAction()
    , fElectricField(efield)
    {
        fMessenger = new G4GenericMessenger(this,
				      "/largeant/",
				      "Control of n-tuple quantities");
        fMessenger->DeclareMethod("electric_field", &LArGeantRunAction::SetElectricField);
        ConstructRootTrees();
    }

    void LArGeantRunAction::ConstructRootTrees()
    {
        G4AnalysisManager *man = G4AnalysisManager::Instance();
        man->SetVerboseLevel(1);
        man->SetNtupleMerging(true);

        man->CreateNtuple("NEST", "NEST");
        man->CreateNtupleIColumn("event");
        man->CreateNtupleSColumn("particle_name");
        man->CreateNtupleDColumn("efield");
        man->CreateNtupleDColumn("dE");
        man->CreateNtupleDColumn("dx");
        man->CreateNtupleSColumn("lar_interaction");
        man->CreateNtupleDColumn("x_i");
        man->CreateNtupleDColumn("y_i");
        man->CreateNtupleDColumn("z_i");
        man->CreateNtupleDColumn("x_f");
        man->CreateNtupleDColumn("y_f");
        man->CreateNtupleDColumn("z_f");
        man->CreateNtupleIColumn("dP");
        man->CreateNtupleIColumn("dQ");
        man->FinishNtuple(0);
    }

    LArGeantRunAction::~LArGeantRunAction()
    {}

    void LArGeantRunAction::BeginOfRunAction(const G4Run* run)
    {
        G4AnalysisManager *man = G4AnalysisManager::Instance();

        G4int runID = run->GetRunID();
        std::stringstream strRunID;
        strRunID << runID;

        G4bool fileopen = man->OpenFile("output_"+strRunID.str()+".root");
        if (!fileopen)
        {
             G4cout << "File - output_" + strRunID.str() + ".root - not opened!" << G4endl;
        }
        else
        {
            G4cout << "File - output_" + strRunID.str() + ".root - opened successfully." << G4endl;
        }
    }

    void LArGeantRunAction::EndOfRunAction(const G4Run*)
    {
        G4AnalysisManager *man = G4AnalysisManager::Instance();
        man->Write();
        man->CloseFile();
    }
}