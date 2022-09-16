/**
 * @file LArGeantCore.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-09-12
 */
#include "LArGeantCore.hh"

namespace largeant
{
    NEST::LArInteraction GetLArInteraction(std::string G4Particle)
    {
        if (
            G4Particle == "neutron" || 
            G4Particle == "anti_neutron" ||
            G4Particle == "nu_e" ||
            G4Particle == "anti_nu_e" ||
            G4Particle == "nu_mu" ||
            G4Particle == "anti_nu_mu" ||
            G4Particle == "nu_tau" ||
            G4Particle == "anti_nu_tau"
        ) 
        {
            return NEST::LArInteraction::NR;
        }
        else if (
            G4Particle == "alpha" ||
            G4Particle == "anti_alpha"
        ) 
        {
            return NEST::LArInteraction::Alpha;
        }
        else
        {
            return NEST::LArInteraction::ER;
        }
    }

    std::map<NEST::LArInteraction, std::string> LArInteractionMap = 
    {
        {NEST::LArInteraction::NR,      "NR"},
        {NEST::LArInteraction::ER,      "ER"},
        {NEST::LArInteraction::Alpha,   "Alpha"},
    };

    ArrayManager::ArrayManager()
    {
    }

    ArrayManager* ArrayManager::sInstance = nullptr;

    ArrayManager* ArrayManager::GetInstance()
    {
        if(sInstance == nullptr) {
            sInstance = new ArrayManager();
        }
        return sInstance;
    }
}