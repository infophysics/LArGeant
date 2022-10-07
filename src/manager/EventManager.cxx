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
        // primaries.emplace_back(
        //     Primary(
        //         "alpha",
        //         0,
        //         {0., 0., 0.},
        //         {5.486 * MeV},
        //         {0., 0., 1.}
        //     )
        // );
        primaries.emplace_back(
            Primary(
                "mu-",
                1,
                {0., 0., 1.},
                {5 * MeV},
                {0., 1., 0.}
            )
        );
        primaries.emplace_back(
            Primary(
                "mu-",
                10,
                {0., 0., 1.},
                {5 * MeV},
                {0., 1., 0.}
            )
        );

        return primaries;
    }

    G4int EventManager::GetIndex(G4String tuple)
    {
        for(G4int ii = 0; ii < sTuples.size(); ii++)
        {
            if(sTuples[ii].name == tuple) {
                return ii;
            }
        }
        sCurrentTupleIndex += 1;
        sTuples.emplace_back(Tuple(tuple, sCurrentTupleIndex));
        return sCurrentTupleIndex;
    }
}