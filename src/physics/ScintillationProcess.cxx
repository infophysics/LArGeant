/**
 * @file ScintillationProcess.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-09-14
 */
#include "ScintillationProcess.hh"

namespace LArGeant
{
    ScintillationProcess::ScintillationProcess()
    : G4VRestDiscreteProcess(
        "ScintillationProcess", 
        fElectromagnetic
    )
    {
        mNESTInterface = NESTInterface::GetInterface();
        pParticleChange = &mParticleChange;
        SetProcessSubType(mProcessSubType);
    }

    ScintillationProcess::~ScintillationProcess() 
    {
    }  

    void ScintillationProcess::MakeSinglePhoton(
        NESTInterfaceResult& result
    ) 
    {
        if(mPhotonYieldFactor < RandomGen::rndm()->rand_uniform()) {
            return;
        }
        // Determine polarization of new photon
        G4ParticleMomentum photonMomentum(G4RandomDirection());
        G4ThreeVector perp = photonMomentum.cross(G4RandomDirection());
        G4ThreeVector photonPolarization = perp.unit();

        // default if non-detailed secondaries
        G4double sampledEnergy = 9.7 * eV;  
        if (mDetailedSecondaries)
        {
            sampledEnergy = mNESTInterface->GetLArNEST()->GetPhotonEnergy(
                mNESTInterface->GetLArDetector()->get_inGas()
            ) * eV;
        }
        G4DynamicParticle* photon = new G4DynamicParticle(
            G4OpticalPhoton::OpticalPhoton(), photonMomentum
        );
        photon->SetPolarization(
            photonPolarization.x(), 
            photonPolarization.y(), 
            photonPolarization.z()
        );
        photon->SetKineticEnergy(sampledEnergy);
        pParticleChange->AddSecondary(new G4Track(
            photon, result.init_time, result.final_position
        ));
    }

    void ScintillationProcess::MakeSingleElectron(
        NESTInterfaceResult& result
    ) 
    {
        if(mElectronYieldFactor < RandomGen::rndm()->rand_uniform()) {
            return;
        }
        G4ParticleMomentum electronMomentum = result.efield_direction;
        G4DynamicParticle* electron = new G4DynamicParticle(
            ThermalElectron::Definition(), electronMomentum
        );
        electron->SetKineticEnergy(result.electron_kinetic_energy);
        pParticleChange->AddSecondary(new G4Track(
            electron, result.init_time, result.final_position
        ));
    }

    void ScintillationProcess::MakePhotons(
        NESTInterfaceResult& result
    )
    {
        // Generate Optical Photons
        G4int numberOfPhotonsToGenerate = int(mPhotonYieldFactor * result.number_of_photons);
        for (int ii = 0; ii < numberOfPhotonsToGenerate; ii++) 
        {
            // Determine polarization of new photon
            G4ParticleMomentum photonMomentum(G4RandomDirection());
            G4ThreeVector perp = photonMomentum.cross(G4RandomDirection());
            G4ThreeVector photonPolarization = perp.unit();

            // default if non-detailed secondaries
            G4double sampledEnergy = 9.7 * eV;  
            if (mDetailedSecondaries)
            {
                sampledEnergy = mNESTInterface->GetLArNEST()->GetPhotonEnergy(
                    mNESTInterface->GetLArDetector()->get_inGas()
                ) * eV;
            }
            G4DynamicParticle* photon = new G4DynamicParticle(
                G4OpticalPhoton::OpticalPhoton(), photonMomentum
            );
            photon->SetPolarization(
                photonPolarization.x(), 
                photonPolarization.y(), 
                photonPolarization.z()
            );
            photon->SetKineticEnergy(sampledEnergy);
            pParticleChange->AddSecondary(new G4Track(
                photon, result.init_time, result.final_position
            ));
        }
    }
    void ScintillationProcess::MakeElectrons(
        NESTInterfaceResult& result
    )
    {
        // Generate Thermal Electrons
        G4int numberOfElectronsToGenerate = int(mElectronYieldFactor * result.number_of_electrons);
        for (int ii = 0; ii < numberOfElectronsToGenerate; ii++) 
        {
            G4ParticleMomentum electronMomentum = result.efield_direction;
            G4DynamicParticle* electron = new G4DynamicParticle(
                ThermalElectron::Definition(), electronMomentum
            );
            electron->SetKineticEnergy(result.electron_kinetic_energy);
            pParticleChange->AddSecondary(new G4Track(
                electron, result.init_time, result.final_position
            ));
        }
    }

    G4VParticleChange* ScintillationProcess::AtRestDoIt(
        const G4Track& aTrack, const G4Step& aStep
    ) 
    {
        pParticleChange->Initialize(aTrack);
        return G4VRestDiscreteProcess::AtRestDoIt(aTrack, aStep);
    }

    

    G4VParticleChange* ScintillationProcess::PostStepDoIt(
        const G4Track& aTrack, const G4Step& aStep
    ) 
    {
        pParticleChange->SetNumberOfSecondaries(1e7);
        pParticleChange->Initialize(aTrack);

        const G4Material* preMaterial = aStep.GetPreStepPoint()->GetMaterial();
        G4ThreeVector init_position = aStep.GetPreStepPoint()->GetPosition();
        G4ThreeVector final_position = aStep.GetPostStepPoint()->GetPosition();
        G4double init_time = aStep.GetPreStepPoint()->GetGlobalTime();

        // Collect NEST parameters
        G4String particle = aTrack.GetDefinition()->GetParticleName();
        G4double energy = aStep.GetTotalEnergyDeposit() / keV;
        G4double density = preMaterial->GetDensity() / (g / cm3);
        
        // Run NEST
        auto result = mNESTInterface->Calculate(
            particle, 
            init_position, 
            final_position, 
            init_time, 
            energy, 
            density
        );

        if(mRecordNESTResults)
        {
            auto AnalysisManager = G4AnalysisManager::Instance();
            AnalysisManager->FillNtupleIColumn(3, 0, result.number_of_photons);
            AnalysisManager->FillNtupleIColumn(3, 1, result.number_of_electrons);
            AnalysisManager->FillNtupleSColumn(3, 2, result.particle);
            AnalysisManager->FillNtupleDColumn(3, 3, result.init_position[0]);
            AnalysisManager->FillNtupleDColumn(3, 4, result.init_position[1]);
            AnalysisManager->FillNtupleDColumn(3, 5, result.init_position[2]);
            AnalysisManager->FillNtupleDColumn(3, 6, result.final_position[0]);
            AnalysisManager->FillNtupleDColumn(3, 7, result.final_position[1]);
            AnalysisManager->FillNtupleDColumn(3, 8, result.final_position[2]);
            AnalysisManager->FillNtupleDColumn(3, 9, result.init_time);
            AnalysisManager->FillNtupleDColumn(3, 10, result.energy);
            AnalysisManager->FillNtupleDColumn(3, 11, result.efield);
            AnalysisManager->FillNtupleDColumn(3, 12, result.density);
            AnalysisManager->FillNtupleDColumn(3, 13, result.electron_kinetic_energy);
            AnalysisManager->FillNtupleDColumn(3, 14, result.efield_direction[0]);
            AnalysisManager->FillNtupleDColumn(3, 15, result.efield_direction[1]);
            AnalysisManager->FillNtupleDColumn(3, 16, result.efield_direction[2]);
            AnalysisManager->AddNtupleRow(3);
        }

        MakePhotons(result);
        MakeElectrons(result);

        return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
    }

    G4double ScintillationProcess::GetMeanFreePath(
        const G4Track&, G4double, G4ForceCondition* condition
    ) 
    {
        *condition = StronglyForced;
        // what this does is enforce the G4S1Light physics process as always
        // happening, so in effect scintillation is a meta-process on top of
        // any and all other energy depositions which may occur, just like the
        // original G4Scintillation (disregard DBL_MAX, this function makes the
        // mean free path zero really, not infinite)

        return DBL_MAX;  // a C-defined constant
    }

    G4double ScintillationProcess::GetMeanLifeTime(
        const G4Track&, G4ForceCondition* condition
    ) 
    {
        *condition = Forced;
        // this function and this condition has the same effect as the above
        return DBL_MAX;
    }


}