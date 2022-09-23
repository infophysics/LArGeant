/**
 * @file EventManagerMessenger.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-20
 */
#pragma once
#include <memory>

#include "G4UImessenger.hh"

#include "EventManager.hh"

namespace LArGeant
{
    class EventManagerMessenger : public G4UImessenger
    {
    public:
        EventManagerMessenger();
        ~EventManagerMessenger();

        void SetNewValue(G4UIcommand* command, G4String value);
        
    private:

    };
}