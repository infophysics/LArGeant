/**
 * @file LArGeantPrimaryGeneratorAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "LArGeantPrimaryGeneratorAction.hh"

namespace largeant
{
    LArGeantPrimaryGeneratorAction::LArGeantPrimaryGeneratorAction(
        G4int numberOfParticles, G4String particleName,
        G4ThreeVector position, G4ThreeVector momentumDirection,
        G4double momentum
    )
    : mNumberOfParticles(numberOfParticles)
    , mParticleName(particleName)
    , mParticlePosition(position)
    , mParticleMomentumDirection(momentumDirection)
    , mParticleMomentum(momentum)
    {
        mParticleGun = new G4ParticleGun(mNumberOfParticles);
        mParticleTable = G4ParticleTable::GetParticleTable();
        mParticle = mParticleTable->FindParticle(mParticleName);
        
        mParticleGun->SetParticlePosition(mParticlePosition);
        mParticleGun->SetParticleMomentumDirection(mParticleMomentumDirection);
        mParticleGun->SetParticleMomentum(mParticleMomentum);
        mParticleGun->SetParticleDefinition(mParticle);
    }

    LArGeantPrimaryGeneratorAction::~LArGeantPrimaryGeneratorAction()
    {
    }

    void LArGeantPrimaryGeneratorAction::SetNumberOfParticles(G4int numberOfParticles)
    {
        mNumberOfParticles = numberOfParticles;
        mParticleGun->SetNumberOfParticles(mNumberOfParticles);
    }
    void LArGeantPrimaryGeneratorAction::SetParticle(G4String particleName)
    {
        mParticleName = particleName;
        mParticle = mParticleTable->FindParticle(mParticleName);
        mParticleGun->SetParticleDefinition(mParticle);
    }
    void LArGeantPrimaryGeneratorAction::SetParticlePosition(G4ThreeVector position)
    {
        mParticlePosition = position;
        mParticleGun->SetParticlePosition(mParticlePosition);
    }
    void LArGeantPrimaryGeneratorAction::SetParticleMomentumDirection(G4ThreeVector momentumDirection)
    {
        mParticleMomentumDirection = momentumDirection;
        mParticleGun->SetParticleMomentumDirection(mParticleMomentumDirection);
    }
    void LArGeantPrimaryGeneratorAction::SetParticleMomentum(G4double momentum)
    {
        mParticleMomentum = momentum;
        mParticleGun->SetParticleMomentum(mParticleMomentum);
    }

    void LArGeantPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
    {
        mParticleGun->GeneratePrimaryVertex(event);
    }
}