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
#include "Core.hh"
#include "Detector.hh"
#include "DetectorComponent.hh"

class Detector;
class DetectorComponent;

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

        // setters for various objects
        static void SetPhysicsList(PhysicsList*);

		static void SetParticle(G4String);

        static void AddComponent(std::shared_ptr<DetectorComponent> component);

        // get the event manager
        static std::shared_ptr<EventManager>& GetEventManager() 
        { 	
			if (sInstance == nullptr) {
				sInstance = std::make_shared<EventManager>();
			}return sInstance; 
		}

        static std::shared_ptr<DetectorComponent> GetComponent(G4int index)
        {
            return sDetectorComponents[index];
        }

        static G4int GetNumberOfComponents() { return sDetectorComponents.size(); }

        std::vector<Primary> GeneratePrimaryList();

    private:
        static std::shared_ptr<EventManager> sInstance;

        // various objects
        inline static std::shared_ptr<PhysicsList> sPhysicsList = {nullptr};
        // inline static std::shared_ptr<Detector> sDetector = {nullptr};
        inline static std::vector<std::shared_ptr<DetectorComponent>> sDetectorComponents = {nullptr};
        
    };
}