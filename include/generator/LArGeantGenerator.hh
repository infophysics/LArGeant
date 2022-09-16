/**
 * @file LArGeantGenerator.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4IonTable.hh"
#include "G4ChargedGeantino.hh"
#include "G4GenericMessenger.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

namespace largeant
    {
    class LArGeantPrimaryGenerator : public G4VUserPrimaryGeneratorAction
    {
    public:
        LArGeantPrimaryGenerator();
        ~LArGeantPrimaryGenerator();
        
        virtual void GeneratePrimaries(G4Event* event);

        // method to access particle gun
        const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
        
    private:
        // position and momentum of the particle
        G4double fX, fY, fZ;
        G4ThreeVector fPosition;
        G4ThreeVector fMomentum;

        G4ParticleGun *fParticleGun;
        G4Box* fEnvelopeBox;

        G4GenericMessenger *fMessenger;
    };
}