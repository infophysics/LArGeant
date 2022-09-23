/**
 * @file EventManager.hh
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

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIterminal.hh"
#include "G4UIExecutive.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4VModularPhysicsList.hh"

#include "PhysicsList.hh"

namespace LArGeant
{
    /**
     * @brief Class for handling generation of source primaries,
     * input/output, data saving, messenging, etc.
     */
    class EventManager
    {
    public:
        EventManager(EventManager const&) = delete;
        void operator=(EventManager const&) = delete;
		EventManager();
        ~EventManager();

        /**
         * @brief 
         *  
         * @details
         * 	Change log:
         * 		2022-09-22 - Initial creation of file.
         */
        static void Initialize();

        // setters for various objects
        static void SetPhysicsList(PhysicsList*);

		static void SetParticle(G4String);

        // get the event manager
        static std::shared_ptr<EventManager>& GetEventManager() 
        { 	
			if (sInstance == nullptr) {
				sInstance = std::make_shared<EventManager>();
			}return sInstance; 
		}

    private:
        static std::shared_ptr<EventManager> sInstance;

        // various objects
        inline static std::shared_ptr<PhysicsList> sPhysicsList = {nullptr};
        
    };
}