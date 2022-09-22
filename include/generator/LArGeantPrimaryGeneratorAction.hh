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

        void SetNumberOfParticles(G4int numberOfParticles);
        void SetParticle(G4String particleName);
        void SetParticlePosition(G4ThreeVector position);
        void SetParticleMomentumDirection(G4ThreeVector momentumDirection);
        void SetParticleMomentum(G4double momentum);

        // method to access particle gun
        G4int GetNumberOfParticles() const { return mNumberOfParticles; }
        G4ParticleDefinition* GetParticle() const { return mParticle; }
        G4String GetParticleName() const { return mParticleName; }
        G4ThreeVector GetParticlePosition() const { return mParticlePosition; }
        G4ThreeVector GetParticleMomentumDirection() const { return mParticleMomentumDirection; }
        G4double GetParticleMomentum() const { return mParticleMomentum; }
        G4ParticleGun* GetParticleGun() const { return mParticleGun; }

        virtual void GeneratePrimaries(G4Event* event);

    private:
        G4ParticleGun* mParticleGun;

        G4int mNumberOfParticles;
        G4ParticleTable* mParticleTable;
        G4ParticleDefinition* mParticle;

        G4String mParticleName;
        G4ThreeVector mParticlePosition;
        G4ThreeVector mParticleMomentumDirection;
        G4double mParticleMomentum;
    };
}