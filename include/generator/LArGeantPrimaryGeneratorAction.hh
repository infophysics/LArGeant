/**
 * @file LArGeantPrimaryGeneratorAction.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"
#include "G4IonTable.hh"
#include "G4Event.hh"

namespace largeant
{
    class LArGeantPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
    {   
    public:
        LArGeantPrimaryGeneratorAction(
            G4int numberOfParticles, G4String particleName,
            G4ThreeVector position = {G4ThreeVector(0.,0.,0.)}, 
            G4ThreeVector momentumDirection = {G4ThreeVector(0.,0.,1.)},
            G4double momentum = {10. * MeV}
        );
        ~LArGeantPrimaryGeneratorAction();

        void setNumberOfParticles(G4int numberOfParticles);
        void setParticle(G4String particleName);
        void setParticlePosition(G4ThreeVector position);
        void setParticleMomentumDirection(G4ThreeVector momentumDirection);
        void setParticleMomentum(G4double momentum);

        // method to access particle gun
        G4int getNumberOfParticles() const { return fNumberOfParticles; }
        G4ParticleDefinition* getParticle() const { return fParticle; }
        G4String getParticleName() const { return fParticleName; }
        G4ThreeVector getParticlePosition() const { return fParticlePosition; }
        G4ThreeVector getParticleMomentumDirection() const { return fParticleMomentumDirection; }
        G4double getParticleMomentum() const { return fParticleMomentum; }
        G4ParticleGun* getParticleGun() const { return fParticleGun; }

        virtual void GeneratePrimaries(G4Event* event);

    private:
        G4ParticleGun* fParticleGun;

        G4int fNumberOfParticles;
        G4ParticleTable* fParticleTable;
        G4ParticleDefinition* fParticle;

        G4String fParticleName;
        G4ThreeVector fParticlePosition;
        G4ThreeVector fParticleMomentumDirection;
        G4double fParticleMomentum;
    };
}