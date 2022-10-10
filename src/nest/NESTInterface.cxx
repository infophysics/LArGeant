/**
 * @file NESTInterface.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @date 2022-10-05
 */
#include "NESTInterface.hh"

namespace LArGeant
{

    std::shared_ptr<NESTInterface> NESTInterface::sNESTInterface = {nullptr};

    std::shared_ptr<NESTInterface> NESTInterface::GetInterface() 
    {
        if (sNESTInterface == nullptr) {
            sNESTInterface = std::shared_ptr<NESTInterface>(new NESTInterface());
        }
        return sNESTInterface;
    }

    void NESTInterface::SetLArDetector(std::shared_ptr<VDetector> detector)
    {
        mLArDetector = detector;
    }
    void NESTInterface::SetLXeDetector(std::shared_ptr<VDetector> detector)
    {
        mLXeDetector = detector;
    }
    void NESTInterface::SetLXeNEST(std::shared_ptr<NEST::NESTcalc> LXeNEST)
    {
        mLXeNEST = LXeNEST;
    }
    void NESTInterface::SetLArNEST(std::shared_ptr<NEST::LArNEST> LArNEST)
    {
        mLArNEST = LArNEST;
    }
    void NESTInterface::SetCalculationMode(G4bool mode)
    {
        mCalculationMode = mode;
    }
    void NESTInterface::SetCalculationMode(G4String mode)
    {
        if(mode == "LXe") {
            mCalculationMode = 0;
        }
        else if (mode == "LAr") {
            mCalculationMode = 1;
        }
        else
        {
            G4cout << "WARNING! Trying to set NEST mode to: " << mode
                << ", but only LXe and LAr are available.  Setting to LAr." << G4endl;
            mCalculationMode = 1;
        }
    }

    NESTInterfaceResult NESTInterface::Calculate(
        G4String particle,
        G4ThreeVector init_position,
        G4ThreeVector final_position,
        G4double init_time,
        G4double energy,
        G4double density
    )
    {
        if(mCalculationMode == 0) {
            return CalculateLXe(
                particle, init_position,
                final_position, init_time,
                energy, density
            );
        }
        else {
            return CalculateLAr(
                particle, init_position,
                final_position, init_time,
                energy, density
            );
        }
    }

    NESTInterfaceResult NESTInterface::CalculateLXe(
        G4String particle,
        G4ThreeVector init_position,
        G4ThreeVector final_position,
        G4double init_time,
        G4double energy,
        G4double density
    )
    {
        NESTInterfaceResult InterfaceResult;
        // Lineage ScintillationProcess::GetChildType(
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
        return InterfaceResult;
    }

    NESTInterfaceResult NESTInterface::CalculateLAr(
        G4String particle,
        G4ThreeVector init_position,
        G4ThreeVector final_position,
        G4double init_time,
        G4double energy,
        G4double density
    )
    {
        // determine the interaction type
        NEST::LArInteraction interaction = NEST::LArInteraction::ER;
        if (particle == "alpha")
        {
            interaction = NEST::LArInteraction::Alpha;
        }
        else if (
            particle == "e-" ||
            particle == "mu-" ||
            particle == "gamma" ||
            particle == "e+" ||
            particle == "mu+"
        )
        {
            interaction = NEST::LArInteraction::ER;
        }
        else
        {
            interaction = NEST::LArInteraction::NR;
        }

        G4double efield = mLArDetector->FitEF(init_position[0], init_position[1], init_position[2]);
        std::vector<G4double> efield_dir = mLArDetector->FitDirEF(init_position[0], init_position[1], init_position[2]);
        G4ThreeVector efield_direction = {efield_dir[0], efield_dir[1], efield_dir[2]};

        // determine the electron kinetic energy
        double electron_speed = mLArNEST->SetDriftVelocity(
            mLArDetector->get_T_Kelvin(), density, efield
        );
        // Change units from NEST (mm/us) to G4 intrinsic units (mm/ns)
        electron_speed = electron_speed * (mm / microsecond);
        G4double v     = electron_speed / CLHEP::c_light;
        G4double gamma = 1/std::sqrt(1-std::pow(v, 2));
        G4double electronKineticEnergy = ThermalElectron::Definition()->GetPDGMass() * (gamma - 1);

        auto result = mLArNEST->FullCalculation(
            interaction, energy, efield, density, false
        );
        if(result.fluctuations.NphFluctuation < 0 || std::isnan(result.fluctuations.NphFluctuation)) {
            result.fluctuations.NphFluctuation = 0;
        }
        if(result.fluctuations.NeFluctuation < 0 || std::isnan(result.fluctuations.NeFluctuation)) {
            result.fluctuations.NeFluctuation = 0;
        }
        return {
            (int)result.fluctuations.NphFluctuation, 
            (int)result.fluctuations.NeFluctuation,
            particle,
            init_position,
            final_position,
            init_time,
            energy,
            efield,
            density,
            electronKineticEnergy,
            efield_direction.unit()
        };
    }
}