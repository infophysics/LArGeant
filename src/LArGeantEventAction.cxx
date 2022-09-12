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
    LArGeantEventAction::LArGeantEventAction(std::shared_ptr<LArGeantRunAction> runAction)
    : G4UserEventAction()
    {
        fEdep = 0.0;
    }

    LArGeantEventAction::~LArGeantEventAction()
    {}

    void LArGeantEventAction::BeginOfEventAction(const G4Event*)
    {
        fEdep = 0.0;
    }

    void LArGeantEventAction::EndOfEventAction(const G4Event*)
    {
    }
}