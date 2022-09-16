/**
 * @file LArGeantStackingAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-09-14
 */
#include "LArGeantStackingAction.hh"

namespace largeant
{
    LArGeantStackingAction::LArGeantStackingAction() 
    {
    }

    LArGeantStackingAction::~LArGeantStackingAction() 
    {
    }

    LArGeantStackingAction* LArGeantStackingAction::theStackingAction = 0;
    G4StackManager* LArGeantStackingAction::savedManager = 0;

    G4ClassificationOfNewTrack LArGeantStackingAction::ClassifyNewTrack(
        const G4Track* track
    ) 
    {
        savedManager = stackManager;
        if (track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
            return fWaiting;
        }
        return fUrgent;
    }

    bool LArGeantStackingAction::isUrgentEmpty() 
    {
        if (!savedManager) 
        {
            std::cerr
                << "savedManager not set by LArGeantStackingAction::ClassifyNewTrack(). "
                "Did you set up LArGeantStackingAction as your stacking action? Did you "
                "override ClassifyNewTrack and forget to set savedManager?"
                << std::endl;
        }
        return savedManager->GetNUrgentTrack() == 0;
    }

    void LArGeantStackingAction::NewStage() 
    {
    }
}
