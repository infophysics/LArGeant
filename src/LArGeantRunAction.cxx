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

        man->CreateNtuple("Hits", "Hits");
        man->CreateNtupleIColumn("fEvent");
        man->CreateNtupleDColumn("fX");
        man->CreateNtupleDColumn("fY");
        man->CreateNtupleDColumn("fZ");
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

        G4bool fileopen = man->OpenFile("output"+strRunID.str()+".root");
        if (!fileopen)
        {
            std::cout << "File not opened!" << std::endl;
        }
       
    }

    void LArGeantRunAction::EndOfRunAction(const G4Run*)
    {
        G4AnalysisManager *man = G4AnalysisManager::Instance();
        man->Write();
        man->CloseFile();
    }
}