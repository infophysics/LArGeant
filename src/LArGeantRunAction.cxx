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
    }

    LArGeantRunAction::~LArGeantRunAction()
    {}

    void LArGeantRunAction::BeginOfRunAction(const G4Run* run)
    {
        G4AnalysisManager *man = G4AnalysisManager::Instance();

        G4int runID = run->GetRunID();

        std::stringstream strRunID;
        strRunID << runID;

        man->OpenFile("output"+strRunID.str()+".root");
    }

    void LArGeantRunAction::EndOfRunAction(const G4Run*)
    {
        G4AnalysisManager *man = G4AnalysisManager::Instance();

        man->Write();
        man->CloseFile();
    }
}