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
        G4AnalysisManager *man = G4AnalysisManager::Instance();
        man->SetVerboseLevel(1);
        man->SetNtupleMerging(true);

        man->CreateNtuple("Photons", "Photons");
        man->CreateNtupleIColumn("fEvent");
        man->CreateNtupleDColumn("fX");
        man->CreateNtupleDColumn("fY");
        man->CreateNtupleDColumn("fZ");
        man->CreateNtupleDColumn("fWavelength");
        man->FinishNtuple(0);

        man->CreateNtuple("Hits", "Hits");
        man->CreateNtupleIColumn("fEvent");
        man->CreateNtupleDColumn("fX");
        man->CreateNtupleDColumn("fY");
        man->CreateNtupleDColumn("fZ");
        man->FinishNtuple(1);

        man->CreateNtuple("NEST", "NEST");
        man->CreateNtupleIColumn("fEvent");
        man->CreateNtupleSColumn("fParticle");
        man->CreateNtupleDColumn("fEdep");
        man->CreateNtupleDColumn("fX");
        man->CreateNtupleDColumn("fY");
        man->CreateNtupleDColumn("fZ");
        man->CreateNtupleDColumn("fNumberOfPhotons");
        man->CreateNtupleDColumn("fNumberOfElectrons");
        man->FinishNtuple(2);
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