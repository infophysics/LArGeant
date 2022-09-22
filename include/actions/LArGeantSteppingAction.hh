/**
 * @file LArGeantSteppingAction.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include <memory>
#include "G4UserSteppingAction.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"

#include "LArDetector.hh"
#include "LArNEST.hh"

#include "LArGeantArgonCubeDetector.hh"
#include "LArGeantEventAction.hh"
#include "LArGeantCore.hh"

namespace largeant
{
    class LArGeantSteppingAction : public G4UserSteppingAction
    {
    public:
        LArGeantSteppingAction(std::shared_ptr<LArGeantEventAction> EventAction);
        ~LArGeantSteppingAction();
        
        virtual void UserSteppingAction(const G4Step*);
        
    private:
        std::shared_ptr<LArGeantEventAction> mEventAction;
        std::shared_ptr<NEST::LArNEST> mLArNEST;
    };
}