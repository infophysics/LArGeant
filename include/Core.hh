/**
 * @file Core.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include <string>
#include <map>

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "LArNEST.hh"

namespace LArGeant
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


    struct Primary
    {
        G4ParticleDefinition* definition = {nullptr};
        G4String name = "geantino";
        G4double time = 0.;
        G4ThreeVector position = {0.,0.,0.};
        G4double energy = 0.;
        G4ThreeVector momentum_direction = {0.,0.,0.};

        Primary(
            G4String particle,
            G4double t,
            G4ThreeVector pos,
            G4double e, 
            G4ThreeVector mom_dir
        )
        : name(particle), time(t), position(pos)
        , energy(e), momentum_direction(mom_dir)
        {
            auto ParticleTable = G4ParticleTable::GetParticleTable();
            definition = ParticleTable->FindParticle(particle);
        }
    };
}