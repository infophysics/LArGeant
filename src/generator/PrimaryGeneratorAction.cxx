/**
 * @file PrimaryGeneratorAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "PrimaryGeneratorAction.hh"

namespace LArGeant
{
    PrimaryGeneratorAction::PrimaryGeneratorAction()
    {
    }

    PrimaryGeneratorAction::~PrimaryGeneratorAction()
    {
    }
        
    PrimaryGeneratorAction::PrimaryGeneratorAction(
        G4int numberOfParticles, 
        G4String particleName,
        G4ThreeVector position, 
        G4ThreeVector momentumDirection,
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

    void PrimaryGeneratorAction::SetPrimaries(std::vector<Primary> primaries)
    {
        mPrimaries = primaries;
    }

    void PrimaryGeneratorAction::SetNumberOfParticles(G4int numberOfParticles)
    {
        mNumberOfParticles = numberOfParticles;
        mParticleGun->SetNumberOfParticles(mNumberOfParticles);
    }
    void PrimaryGeneratorAction::SetParticle(G4String particleName)
    {
        mParticleName = particleName;
        mParticle = mParticleTable->FindParticle(mParticleName);
        mParticleGun->SetParticleDefinition(mParticle);
    }
    void PrimaryGeneratorAction::SetParticlePosition(G4ThreeVector position)
    {
        mParticlePosition = position;
        mParticleGun->SetParticlePosition(mParticlePosition);
    }
    void PrimaryGeneratorAction::SetParticleMomentumDirection(G4ThreeVector momentumDirection)
    {
        mParticleMomentumDirection = momentumDirection;
        mParticleGun->SetParticleMomentumDirection(mParticleMomentumDirection);
    }
    void PrimaryGeneratorAction::SetParticleMomentum(G4double momentum)
    {
        mParticleMomentum = momentum;
        mParticleGun->SetParticleMomentum(mParticleMomentum);
    }

    void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
    {
        for (auto primary : mPrimaries)
        {
            mParticleGun->SetNumberOfParticles(1);
            mParticleGun->SetParticleDefinition(primary.definition);
            mParticleGun->SetParticleTime(primary.time);
            mParticleGun->SetParticlePosition(primary.position);
            mParticleGun->SetParticleEnergy(primary.energy);
            mParticleGun->SetParticleMomentumDirection(primary.momentum_direction);
            mParticleGun->GeneratePrimaryVertex(event);
        }
    }
}