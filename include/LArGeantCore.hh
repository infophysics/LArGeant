/**
 * @file LArGeantArgonCore.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include <string>
#include <map>

#include "LArNEST.hh"

namespace largeant
{
    NEST::LArInteraction GetLArInteraction(std::string G4Particle);
    extern std::map<NEST::LArInteraction, std::string> LArInteractionMap;

    class ArrayManager
    {
    private:
        static ArrayManager* sInstance;

    protected:
        ArrayManager();

    public:
        ArrayManager(ArrayManager& other) = delete;
        void operator=(const ArrayManager&) = delete;

        static ArrayManager* GetInstance();
    };
}