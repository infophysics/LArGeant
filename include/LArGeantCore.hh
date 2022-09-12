/**
 * @file LArGeantArgonCore.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include <string>

#include "LArNEST.hh"

namespace largeant
{
    NEST::LArInteraction GetLArInteraction(std::string G4Particle);
}