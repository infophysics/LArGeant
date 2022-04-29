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
    : fNumberOfParticles(numberOfParticles)
    , fParticleName(particleName)
    , fParticlePosition(position)
    , fParticleMomentumDirection(momentumDirection)
    , fParticleMomentum(momentum)
    {
        fParticleGun = new G4ParticleGun(fNumberOfParticles);
        fParticleTable = G4ParticleTable::GetParticleTable();
        fParticle = fParticleTable->FindParticle(fParticleName);
        
        fParticleGun->SetParticlePosition(fParticlePosition);
        fParticleGun->SetParticleMomentumDirection(fParticleMomentumDirection);
        fParticleGun->SetParticleMomentum(fParticleMomentum);
        fParticleGun->SetParticleDefinition(fParticle);
    }

    LArGeantPrimaryGeneratorAction::~LArGeantPrimaryGeneratorAction()
    {
    }

    void LArGeantPrimaryGeneratorAction::setNumberOfParticles(G4int numberOfParticles)
    {
        fNumberOfParticles = numberOfParticles;
        fParticleGun->SetNumberOfParticles(fNumberOfParticles);
    }
    void LArGeantPrimaryGeneratorAction::setParticle(G4String particleName)
    {
        fParticleName = particleName;
        fParticle = fParticleTable->FindParticle(fParticleName);
        fParticleGun->SetParticleDefinition(fParticle);
    }
    void LArGeantPrimaryGeneratorAction::setParticlePosition(G4ThreeVector position)
    {
        fParticlePosition = position;
        fParticleGun->SetParticlePosition(fParticlePosition);
    }
    void LArGeantPrimaryGeneratorAction::setParticleMomentumDirection(G4ThreeVector momentumDirection)
    {
        fParticleMomentumDirection = momentumDirection;
        fParticleGun->SetParticleMomentumDirection(fParticleMomentumDirection);
    }
    void LArGeantPrimaryGeneratorAction::setParticleMomentum(G4double momentum)
    {
        fParticleMomentum = momentum;
        fParticleGun->SetParticleMomentum(fParticleMomentum);
    }

    void LArGeantPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
    {
        std::cout << fParticleGun->GetParticleMomentum() << std::endl;
        fParticleGun->GeneratePrimaryVertex(event);
    }
}