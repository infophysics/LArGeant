/**
 * @file LArGeantSteppingAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "LArGeantSteppingAction.hh"

namespace largeant
{
    LArGeantSteppingAction::LArGeantSteppingAction(LArGeantEventAction *eventAction)
    : G4UserSteppingAction()
    {
        fEventAction = eventAction;
    }

    LArGeantSteppingAction::~LArGeantSteppingAction()
    {}

    void LArGeantSteppingAction::UserSteppingAction(const G4Step *step)
    {   
        G4LogicalVolume *volume 
            = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
        
        const LArGeantArgonSphere *detectorConstruction 
            = static_cast<const LArGeantArgonSphere*> 
            (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    }
}