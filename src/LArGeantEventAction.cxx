/**
 * @file LArGeantEventAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "LArGeantEventAction.hh"

namespace largeant
{
    LArGeantEventAction::LArGeantEventAction(LArGeantRunAction*)
    : G4UserEventAction()
    {
    }

    LArGeantEventAction::~LArGeantEventAction()
    {}

    void LArGeantEventAction::BeginOfEventAction(const G4Event*)
    {
    }

    void LArGeantEventAction::EndOfEventAction(const G4Event*)
    {
        G4AnalysisManager *man = G4AnalysisManager::Instance();
    }
}