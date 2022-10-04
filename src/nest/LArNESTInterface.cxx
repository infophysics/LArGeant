/**
 * @file LArNESTInterface.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-09-14
 */
#include "LArNESTInterface.hh"

namespace LArGeant
{
    LArNESTScintillationProcess::LArNESTScintillationProcess(
        const G4String& processName, 
        G4ProcessType type, 
        LArDetector* detector
    )
    : LArNESTScintillationProcess(processName, type, new NEST::LArNEST(detector), detector) 
    {
    }

    LArNESTScintillationProcess::LArNESTScintillationProcess(
        const G4String& processName, G4ProcessType type,
        NEST::LArNEST* customcalc, LArDetector* detector
    )
    : G4VRestDiscreteProcess(processName, type)
    , mLArNEST(customcalc)
    , mDetector(detector) 
    {
        pParticleChange = &mParticleChange;
        SetProcessSubType(fScintillation);
    }

    LArNESTScintillationProcess::~LArNESTScintillationProcess() 
    {
    }  

    G4Track* LArNESTScintillationProcess::MakePhoton(G4ThreeVector xyz, double t) 
    {
        // Determine polarization of new photon
        G4ParticleMomentum photonMomentum(G4RandomDirection());
        G4ThreeVector perp = photonMomentum.cross(G4RandomDirection());
        G4ThreeVector photonPolarization = perp.unit();
        // default if non-detailed secondaries
        G4double sampledEnergy = 9.7 * eV;  
        if (mDetailedSecondaries)
        {
            sampledEnergy = mLArNEST->GetPhotonEnergy(
                mDetector->get_inGas()
            ) * eV;
        }
        G4DynamicParticle* photon = new G4DynamicParticle(
            G4OpticalPhoton::OpticalPhoton(), photonMomentum
        );
        photon->SetPolarization(
            photonPolarization.x(), photonPolarization.y(), photonPolarization.z()
        );
        photon->SetKineticEnergy(sampledEnergy);
        return new G4Track(photon, t, xyz);
    }

    G4Track* LArNESTScintillationProcess::MakeElectron(
        G4ThreeVector xyz, double density, 
        double t, double kin_E
    ) 
    {
        double efield_here = mDetector->FitEF(xyz.x(), xyz.y(), xyz.z());
        std::vector<double> efield_vec = mDetector->FitDirEF(xyz.x(), xyz.y(), xyz.z());
        //G4ThreeVector efield_dir_here = G4ThreeVector(efield_vec[0], efield_vec[1], efield_vec[2]);
        G4ThreeVector efield_dir_here = G4ThreeVector(0, 1, 0);
        efield_here = 500 * volt / cm;
        if (efield_here > 0) 
        {
            G4ParticleMomentum electronMomentum = efield_dir_here.unit();
            G4DynamicParticle* electron = new G4DynamicParticle(
                LArNESTThermalElectron::ThermalElectron(), electronMomentum
            );
            electron->SetKineticEnergy(kin_E);
            return new G4Track(electron, t, xyz);
        } 
        else 
        {
            return nullptr;
        }
    }

    G4VParticleChange* LArNESTScintillationProcess::AtRestDoIt(
        const G4Track& aTrack, const G4Step& aStep
    ) 
    {
        pParticleChange->Initialize(aTrack);
        return G4VRestDiscreteProcess::AtRestDoIt(aTrack, aStep);
    }

    // Lineage LArNESTScintillationProcess::GetChildType(
    //     const G4Track* parent, const G4Track* child
    // ) const 
    // {
    //     // G4cout << parent->GetDefinition()->GetAtomicNumber() << G4endl;
    //     // G4cout << parent->GetDefinition()->GetAtomicMass() << G4endl;
    //     // G4cout << parent->GetDefinition()->GetParticleName() << G4endl;
    //     // logic to determine what processes are 
    //     // kicked off by this track and also set the info
    //     G4String sec_creator = "";
    //     if (child->GetCreatorProcess()) {
    //         sec_creator = child->GetCreatorProcess()->GetProcessName();
    //     }
    //     if (
    //         parent && 
    //         parent->GetDefinition() == G4Neutron::Definition() &&
    //         child->GetDefinition()->GetAtomicNumber() > 0
    //     )  // neutron inelastic scatters never join the lineage.
    //     {
    //         return Lineage(NEST::LArInteraction::NR);
    //     } 
    //     else if (
    //         parent && 
    //         parent->GetDefinition()->GetParticleName() == "alpha"
    //     )
    //     {
    //         return Lineage(NEST::LArInteraction::Alpha);
    //     }
    //     else if (
    //         parent &&
    //         parent->GetDefinition() == G4Gamma::Definition()
    //     ) 
    //     { 
    //         return Lineage(NEST::LArInteraction::ER);
    //     } 
    //     else if (
    //         parent && 
    //         parent->GetDefinition() == G4Electron::Definition()
    //     )
    //     {
    //         return Lineage(NEST::LArInteraction::ER);
    //     }
    //     else if (
    //         child->GetDefinition() == G4Electron::Definition() &&
    //         (sec_creator.contains("Decay") || !parent)
    //     ) 
    //     {
    //         return Lineage(NEST::LArInteraction::ER);
    //     } 
    //     else if (
    //         child->GetDefinition()->GetParticleName() == "alpha" &&
    //         !parent
    //     ) 
    //     {
    //         return Lineage(NEST::LArInteraction::Alpha);
    //     } 
    //     else if (
    //         child->GetDefinition()->GetAtomicMass() > 1 &&
    //         (sec_creator.contains("Decay") || !parent)
    //     ) 
    //     {
    //         return Lineage(NEST::LArInteraction::ER);
    //     }
    //     return Lineage(NEST::LArInteraction::NoneType);
    // }

    G4VParticleChange* LArNESTScintillationProcess::PostStepDoIt(
        const G4Track& aTrack, const G4Step& aStep
    ) 
    {
        pParticleChange->SetNumberOfSecondaries(1e7);
        pParticleChange->Initialize(aTrack);

        const G4Material* preMaterial = aStep.GetPreStepPoint()->GetMaterial();
        G4StepPoint* pPreStepPoint = aStep.GetPreStepPoint();
        G4StepPoint* pPostStepPoint = aStep.GetPostStepPoint();
        G4ThreeVector x1 = pPostStepPoint->GetPosition();
        G4ThreeVector x0 = pPreStepPoint->GetPosition();
        G4double evtStrt = pPreStepPoint->GetGlobalTime();
        G4double t0 = pPreStepPoint->GetGlobalTime();

        G4double Density = preMaterial->GetDensity() / (g / cm3);
        double step_E = aStep.GetTotalEnergyDeposit() / keV;
        double efield_here = mDetector->FitEF(x1[0], x1[1], x1[2]);
        NEST::LArInteraction interaction = NEST::LArInteraction::ER;
        if (aTrack.GetDefinition()->GetParticleName() == "alpha")
        {
            interaction = NEST::LArInteraction::Alpha;
        }
        else if (
            aTrack.GetDefinition()->GetParticleName() == "e-" ||
            aTrack.GetDefinition()->GetParticleName() == "mu-" ||
            aTrack.GetDefinition()->GetParticleName() == "gamma" ||
            aTrack.GetDefinition()->GetParticleName() == "e+" ||
            aTrack.GetDefinition()->GetParticleName() == "mu+"
        )
        {
            interaction = NEST::LArInteraction::ER;
        }
        else
        {
            interaction = NEST::LArInteraction::NR;
        }

        auto result = mLArNEST->FullCalculation(
            interaction, step_E, efield_here, Density, false
        );
        if (int(result.fluctuations.NphFluctuation)) 
        {
            for (int i = 0; i < int(result.fluctuations.NphFluctuation); ++i) 
            {
                if (
                    mYieldFactor == 1 ||
                    (mYieldFactor > 0 && RandomGen::rndm()->rand_uniform() < mYieldFactor)
                ) 
                {
                    if (mStackPhotons) 
                    {
                        //G4Track* onePhoton = MakePhoton(hit.xyz, *photontimes + hit.t);
                        G4Track* onePhoton = MakePhoton(x1, t0);
                        pParticleChange->AddSecondary(onePhoton);
                    }
                }
            }
        }

        // check if electrons were generated
        if (int(result.fluctuations.NeFluctuation)) 
        {
            double electron_speed = mLArNEST->SetDriftVelocity(
                mDetector->get_T_Kelvin(), Density, efield_here
            );
            // Change units from NEST (mm/us) to G4 intrinsic units (mm/ns)
            electron_speed = electron_speed * (mm / microsecond);
            G4double v     = electron_speed / CLHEP::c_light;
            G4double gamma = 1/std::sqrt(1-std::pow(v, 2));
            double electron_kin_E = LArNESTThermalElectron::ThermalElectron()->GetPDGMass() * (gamma - 1);
            for (int i = 0; i < int(result.fluctuations.NeFluctuation); ++i) 
            {
                if (mYieldFactor == 1 ||
                    (mYieldFactor > 0 &&
                    RandomGen::rndm()->rand_uniform() < mYieldFactor)) 
                {
                    if (mStackElectrons) 
                    {
                        G4Track* oneElectron = MakeElectron(
                            x1, Density,
                            t0, electron_kin_E
                        );
                        if (oneElectron) pParticleChange->AddSecondary(oneElectron);
                    }
                }
            }
        }
        return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
    }

    G4double LArNESTScintillationProcess::GetMeanFreePath(
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

    G4double LArNESTScintillationProcess::GetMeanLifeTime(
        const G4Track&, G4ForceCondition* condition
    ) 
    {
        *condition = Forced;
        // this function and this condition has the same effect as the above
        return DBL_MAX;
    }

    LArNESTThermalElectron* LArNESTThermalElectron::theInstance = 0;

    LArNESTThermalElectron* LArNESTThermalElectron::Definition() 
    {
        if (theInstance != 0) return theInstance;
        const G4String name = "thermalelectron";
        // search in particle table]
        G4ParticleTable* pTable = G4ParticleTable::GetParticleTable();
        G4ParticleDefinition* anInstance = pTable->FindParticle(name);
        if (anInstance == 0) 
        {
            // create particle
            //
            //    Arguments for constructor are as follows
            //               name             mass          width         charge
            //             2*spin           parity  C-conjugation
            //          2*Isospin       2*Isospin3       G-parity
            //               type    lepton number  baryon number   PDG encoding
            //             stable         lifetime    decay table
            //             shortlived      subType    anti_encoding

            // use constants in CLHEP
            //  static const double electron_mass_c2 = 0.51099906 * MeV;

            anInstance = new G4ParticleDefinition(
                name, electron_mass_c2, 0.0 * MeV, -1. * eplus, 1, 0, 0, 0, 0, 0,
                "lepton", 1, 0, 11, true, -1.0, NULL, false, "e");
            // Bohr Magnetron
            G4double muB = -0.5 * eplus * hbar_Planck / (electron_mass_c2 / c_squared);

            anInstance->SetPDGMagneticMoment(muB * 2. * 1.0011596521859);
        }
        theInstance = reinterpret_cast<LArNESTThermalElectron*>(anInstance);
        return theInstance;
    }

    LArNESTThermalElectron* LArNESTThermalElectron::ThermalElectronDefinition() 
    { 
        return Definition(); 
    }

    LArNESTThermalElectron* LArNESTThermalElectron::ThermalElectron() 
    {
        return Definition();
    }
}