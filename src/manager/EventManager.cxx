/**
 * @file EventManager.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-20
 */
#include "EventManager.hh"

namespace LArGeant
{
    std::shared_ptr<EventManager> EventManager::sInstance;

    EventManager::EventManager()
    {
		if (sInstance != nullptr) {
			return;
		}
    }

    EventManager::~EventManager()
    {
    }

    /**
     * @brief 
     * @details
     * 	Change log:
     * 		2022-09-22 - Initial creation of file.
     */
    void EventManager::Initialize()
    {
    }

    void EventManager::SetPhysicsList(PhysicsList* physicsList)
    {
        sPhysicsList.reset(physicsList);
    }
}