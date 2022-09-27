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
    }

    EventManager::~EventManager()
    {
    }

	void EventManager::AddComponent(std::shared_ptr<DetectorComponent> component)
	{
		sDetectorComponents.push_back(component);
	}

    void EventManager::SetPhysicsList(PhysicsList* physicsList)
    {
        sPhysicsList.reset(physicsList);
    }

    std::vector<Primary> EventManager::GeneratePrimaryList()
    {
        std::vector<Primary> primaries;
        primaries.emplace_back(
            Primary(
                "mu-",
                1,
                {0., 0., 1.},
                {5 * MeV},
                {0., 0., 1.}
            )
        );
        primaries.emplace_back(
            Primary(
                "mu-",
                1,
                {0., 0., 1.},
                {5 * MeV},
                {0., 1., 0.}
            )
        );

        return primaries;
    }
}