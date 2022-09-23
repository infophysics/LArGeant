/**
 * @file EventManagerMessenger.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-20
 */
#include "EventManagerMessenger.hh"

namespace LArGeant
{
    EventManagerMessenger::EventManagerMessenger()
    {
    }

    EventManagerMessenger::~EventManagerMessenger()
    {
    }

    void EventManagerMessenger::SetNewValue(G4UIcommand* command, G4String value)
    {
        auto eventManager = EventManager::GetEventManager();
    }
}