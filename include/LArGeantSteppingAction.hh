/**
 * @file LArGeantSteppingAction.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include "G4UserSteppingAction.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"

#include "LArGeantDetectorConstruction.hh"
#include "LArGeantEventAction.hh"

namespace largeant
{
    class LArGeantSteppingAction : public G4UserSteppingAction
    {
    public:
        LArGeantSteppingAction(LArGeantEventAction* eventAction);
        ~LArGeantSteppingAction();
        
        virtual void UserSteppingAction(const G4Step*);
        
    private:
        LArGeantEventAction *fEventAction;
    };
}