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
        G4double density,
        G4int parent_track_id
    )
    {
        if(mCalculationMode == 0) {
            return CalculateLXe(
                particle, init_position,
                final_position, init_time,
                energy, density, parent_track_id
            );
        }
        else {
            return CalculateLAr(
                particle, init_position,
                final_position, init_time,
                energy, density, parent_track_id
            );
        }
    }

    Lineage NESTInterface::GetChildType(const G4Track* parent, const G4Track* child) const
    {
        // // Determine the creation process if there is one.
        // G4String sec_creator = "";
        // if (child->GetCreatorProcess()) {
        //     sec_creator = child->GetCreatorProcess()->GetProcessName();
        // }
        // // neutron inelastic scatters never join the lineage.
        // if (
        //     (parent && parent->GetDefinition() == G4Neutron::Definition()) &&
        //     (child->GetDefinition()->GetAtomicNumber() > 0)
        // )  
        // {
        //     return Lineage(NR);
        // }
        // // Krypton 83 
        // else if (
        //     (parent && parent->GetDefinition()->GetAtomicMass() == 83) &&
        //     (parent->GetDefinition()->GetAtomicNumber() == 36) &&
        //     (parent->GetDefinition()->GetIonLifeTime() * .693 < 2 * 60 * 60 * s) &&
        //     (parent->GetDefinition()->GetIonLifeTime() * .693 > 1 * 60 * 60 * s)
        // ) 
        // {
        //     return Lineage(Kr83m);
        // } 
        // // Gamma
        // else if (parent && parent->GetDefinition() == G4Gamma::Definition()) 
        // {
        //     if (sec_creator.contains("compt")) {
        //         return Lineage(beta);
        //     } 
        //     // conv is pair production
        //     else if (sec_creator.contains("conv")) {  
        //         return Lineage(beta);
        //     } 
        //     else if (sec_creator.contains("phot")) {
        //         return Lineage(gammaRay);
        //     }
        // } 
        // // Electron
        // else if (
        //     child->GetDefinition() == G4Electron::Definition() &&
        //     (sec_creator.contains("Decay") || !parent)
        // ) 
        // {
        //     return Lineage(beta);
        // } 
        // // Ion
        // else if (
        //     child->GetDefinition()->GetAtomicMass() > 1 &&
        //     (sec_creator.contains("Decay") || !parent)
        // ) 
        // {
        //     Lineage ion_lin = Lineage(ion);
        //     ion_lin.A = child->GetDefinition()->GetAtomicMass();
        //     ion_lin.Z = child->GetDefinition()->GetAtomicNumber();
        //     return ion_lin;
        // }
        // return Lineage(NoneType);
    }

    void NESTInterface::PopulateLineages(const G4Track& track, const G4Step& step)
    {
        // // ready to pop out OP and TE?
        // if (!(
        //     (track.GetKineticEnergy() == 0) &&
        //     (NESTStackingAction::theStackingAction->isUrgentEmpty()) &&
        //     (step.GetSecondary()->empty())
        // )) {
        //     return;
        // } 
        // mPreviousLineages.clear();
        // for (auto& lineage : mLineages) 
        // {
        //     double total_energy = std::accumulate(
        //         lineage.hits.begin(), lineage.hits.end(), 0.,
        //         [](double a, Hit b) { return a + b.E; }
        //     );
        //     if (total_energy == 0) {
        //         continue;
        //     }

        //     G4ThreeVector maxHit_xyz = std::max_element(
        //         lineage.hits.begin(), lineage.hits.end(),
        //         [](Hit a, Hit b) { return a.E < b.E; }
        //     )->xyz;
        //     double efield_here = fDetector->FitEF(maxHit_xyz.x(), maxHit_xyz.y(), maxHit_xyz.z());
            
        //     lineage.result = fNESTcalc->FullCalculation(
        //         lineage.type, total_energy, lineage.density, efield_here, lineage.A,
        //         lineage.Z, NESTcalc::default_NRYieldsParam, NESTcalc::default_NRERWidthsParam,
        //         detailed_secondaries
        //     );
        //     lineage.result_calculated = true;
        //     if (lineage.result.quanta.photons) 
        //     {
        //         auto photontimes = lineage.result.photon_times.begin();
        //         double cumulative_energy = 0;
        //         int cumulative_photons = 0;
        //         for (auto& hit : lineage.hits) 
        //         {
        //             hit.result.photons =
        //                 round((lineage.result.quanta.photons - cumulative_photons) * hit.E /
        //                         (total_energy - cumulative_energy));
        //             cumulative_energy += hit.E;
        //             cumulative_photons += hit.result.photons;
        //             for (int i = 0; i < hit.result.photons; ++i) 
        //             {
        //                 if (
        //                     (YieldFactor == 1) || 
        //                     (YieldFactor > 0 && RandomGen::rndm()->rand_uniform() < YieldFactor) &&
        //                     stack_photons
        //                 ) 
        //                 {
        //                     G4Track* onePhoton = MakePhoton(hit.xyz, *photontimes + hit.t);
        //                     pParticleChange->AddSecondary(onePhoton);
        //                 }
        //                 ++photontimes;
        //             }
        //         }
        //     }
        //     if (lineage.result.quanta.electrons) 
        //     {
        //         double cumulative_energy = 0;
        //         double cumulative_electrons = 0;
        //         double electron_speed = fNESTcalc->SetDriftVelocity(
        //             fDetector->get_T_Kelvin(), lineage.density, efield_here);

        //         electron_speed = electron_speed * mm / us;
        //         G4double v     = electron_speed / CLHEP::c_light;
        //         G4double gamma = 1/std::sqrt(1-std::pow(v, 2));
        //         double electron_kin_E =
        //         NESTThermalElectron::ThermalElectron()->GetPDGMass() * (gamma - 1);

        //         for (auto& hit : lineage.hits) 
        //         {
        //             hit.result.electrons =
        //                 round((lineage.result.quanta.electrons - cumulative_electrons) * hit.E /
        //                         (total_energy - cumulative_energy));
        //             cumulative_energy += hit.E;
        //             cumulative_electrons += hit.result.electrons;
        //             for (int i = 0; i < hit.result.electrons; ++i) 
        //             {
        //                 if (
        //                     (YieldFactor == 1) ||
        //                     (YieldFactor > 0 && RandomGen::rndm()->rand_uniform() < YieldFactor)
        //                     (stack_electrons) 
        //                 )
        //                 {
        //                     G4Track* oneElectron = MakeElectron(hit.xyz, lineage.density,
        //                                                         hit.t, electron_kin_E);
        //                     if (oneElectron) pParticleChange->AddSecondary(oneElectron);
        //                 }
        //             }
        //         }
        //     }
        //     mPreviousLineages.push_back(lineage);
        // }
        // mLineages.clear();
        // track_lins.clear();
    }

    NESTInterfaceResult NESTInterface::CalculateLXe(
        G4String particle,
        G4ThreeVector init_position,
        G4ThreeVector final_position,
        G4double init_time,
        G4double energy,
        G4double density,
        G4int parent_track_id
    )
    {
        NESTInterfaceResult InterfaceResult;   

        return InterfaceResult;
    }

    NESTInterfaceResult NESTInterface::CalculateLAr(
        G4String particle,
        G4ThreeVector init_position,
        G4ThreeVector final_position,
        G4double init_time,
        G4double energy,
        G4double density,
        G4int parent_track_id
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
            efield_direction.unit(),
            parent_track_id
        };
    }
}