/**
 * @file LArGeantLArNEST.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-09-14
 */
#include "LArGeantLArNEST.hh"

namespace largeant
{
    LArNESTScintillationProcess::LArNESTScintillationProcess(
        const G4String& processName, G4ProcessType type, LArDetector* detector
    )
    : LArNESTScintillationProcess(processName, type, new NEST::LArNEST(detector), detector) 
    {
    }

    LArNESTScintillationProcess::LArNESTScintillationProcess(
        const G4String& processName, G4ProcessType type,
        NEST::LArNEST* customcalc, LArDetector* detector
    )
    : G4VRestDiscreteProcess(processName, type)
    , fLArNEST(customcalc)
    , fDetector(detector) 
    {
        pParticleChange = &fParticleChange;
        SetProcessSubType(fScintillation);

        if (verboseLevel > 0) {
            G4cout << GetProcessName() << " is created " << G4endl;
        }
    }

    LArNESTScintillationProcess::~LArNESTScintillationProcess() {}  // destructor needed to avoid linker error

    G4Track* LArNESTScintillationProcess::MakePhoton(G4ThreeVector xyz, double t) 
    {
        // Determine polarization of new photon
        G4ParticleMomentum photonMomentum(G4RandomDirection());
        G4ThreeVector perp = photonMomentum.cross(G4RandomDirection());
        G4ThreeVector photonPolarization = perp.unit();
        // LArDetector* detector = fLArNEST->GetDetector();
        LArDetector* detector;
        G4double sampledEnergy = 7.08 * eV;  // default if non-detailed secondaries
        if (detailed_secondaries)
        {
            sampledEnergy = fLArNEST->PhotonEnergy(
                false /*i.e. S1*/, detector->get_inGas(),
                detector->get_T_Kelvin()
            ) * eV;
        }
        G4DynamicParticle* aQuantum = new G4DynamicParticle(
            G4OpticalPhoton::OpticalPhoton(), photonMomentum
        );
        aQuantum->SetPolarization(
            photonPolarization.x(), photonPolarization.y(), photonPolarization.z()
        );
        aQuantum->SetKineticEnergy(sampledEnergy);
        return new G4Track(aQuantum, t, xyz);
    }

    G4Track* LArNESTScintillationProcess::MakeElectron(
        G4ThreeVector xyz, double density, 
        double t, double kin_E
    ) 
    {
        // Determine polarization of new photon

        double efield_here = fDetector->FitEF(xyz.x(), xyz.y(), xyz.z());
        std::vector<double> efield_vec = fDetector->FitDirEF(xyz.x(), xyz.y(), xyz.z());
        G4ThreeVector efield_dir_here = G4ThreeVector(efield_vec[0], efield_vec[1], efield_vec[2]);

        if (efield_here > 0) 
        {
            G4ParticleMomentum electronMomentum = efield_dir_here.unit();
            G4DynamicParticle* aQuantum = new G4DynamicParticle(
                LArNESTThermalElectron::ThermalElectron(), electronMomentum
            );
            aQuantum->SetKineticEnergy(kin_E);
            return new G4Track(aQuantum, t, xyz);
        } 
        else 
        {
            return nullptr;
        }
    }

    void LArNESTScintillationProcess::TryPopLineages(const G4Track& aTrack, const G4Step& aStep) 
    {
        pParticleChange->SetNumberOfSecondaries(1e7);

        // ready to pop out OP and TE?
        if (
            aTrack.GetKineticEnergy() == 0 &&
            LArGeantStackingAction::theStackingAction->isUrgentEmpty() &&
            aStep.GetSecondary()->empty()
        ) 
        {
            lineages_prevEvent.clear();
            for (auto& lineage : lineages) 
            {
                double etot = std::accumulate(
                    lineage.hits.begin(), lineage.hits.end(), 0.,
                    [](double a, Hit b) { return a + b.E; }
                );
                if (etot == 0) {
                    continue;
                }
                G4ThreeVector maxHit_xyz = std::max_element(
                    lineage.hits.begin(), lineage.hits.end(),
                    [](Hit a, Hit b) { return a.E < b.E; }
                )->xyz;
                double efield_here = fDetector->FitEF(maxHit_xyz.x(), maxHit_xyz.y(), maxHit_xyz.z());

                lineage.result = fLArNEST->FullCalculation(
                    lineage.type, etot, efield_here, lineage.density, false
                );
                lineage.result_calculated = true;

                // check if photons were generated
                if (int(lineage.result.fluctuations.NphFluctuation)) 
                {
                    auto photontimes = lineage.result.photon_times.begin();
                    double ecum = 0;
                    int phot_cum = 0;
                    for (auto& hit : lineage.hits) 
                    {
                        hit.result.NphFluctuation = round(
                            (int(lineage.result.fluctuations.NphFluctuation) - phot_cum) * hit.E / (etot - ecum)
                        );
                        ecum += hit.E;
                        phot_cum += int(hit.result.NphFluctuation);
                        for (int i = 0; i < int(hit.result.NphFluctuation); ++i) 
                        {
                            if (
                                YieldFactor == 1 ||
                                (YieldFactor > 0 && RandomGen::rndm()->rand_uniform() < YieldFactor)
                            ) 
                            {
                                if (stack_photons) 
                                {
                                    G4Track* onePhoton = MakePhoton(hit.xyz, *photontimes + hit.t);
                                    pParticleChange->AddSecondary(onePhoton);
                                }
                            }
                            ++photontimes;
                        }
                    }
                }

                // check if electrons were generated
                if (int(lineage.result.fluctuations.NeFluctuation)) 
                {
                    double ecum = 0;
                    double el_cum = 0;
                    double electron_speed = fLArNEST->SetDriftVelocity(
                        fDetector->get_T_Kelvin(), lineage.density, efield_here
                    );
                    // double electron_kin_E =
                    //     LArNESTThermalElectron::ThermalElectron()->GetPDGMass() *
                    //     std::pow(electron_speed * mm / us, 2);

                    // Change units from NEST (mm/us) to G4 intrinsic units (mm/ns)
                    electron_speed = electron_speed * mm / us;
                    G4double v     = electron_speed / CLHEP::c_light;
                    G4double gamma = 1/std::sqrt(1-std::pow(v, 2));
                    double electron_kin_E = LArNESTThermalElectron::ThermalElectron()->GetPDGMass() * (gamma - 1);

                    for (auto& hit : lineage.hits) 
                    {
                        hit.result.NeFluctuation = round(
                            (int(lineage.result.fluctuations.NeFluctuation) - el_cum) * hit.E / (etot - ecum)
                        );
                        ecum += hit.E;
                        el_cum += int(hit.result.NeFluctuation);
                        for (int i = 0; i < int(hit.result.NeFluctuation); ++i) 
                        {
                            if (YieldFactor == 1 ||
                                (YieldFactor > 0 &&
                                RandomGen::rndm()->rand_uniform() < YieldFactor)) 
                            {
                                if (stack_electrons) 
                                {
                                    G4Track* oneElectron = MakeElectron(
                                        hit.xyz, lineage.density,
                                        hit.t, electron_kin_E
                                    );
                                    if (oneElectron) pParticleChange->AddSecondary(oneElectron);
                                }
                            }
                        }
                    }
                }
                lineages_prevEvent.push_back(lineage);
            }
            if (analysisTrigger) {
                analysisTrigger(lineages_prevEvent);
            }
            lineages.clear();
            track_lins.clear();
        }
        return;
    }

    G4VParticleChange* LArNESTScintillationProcess::AtRestDoIt(
        const G4Track& aTrack, const G4Step& aStep
    ) 
    {
        pParticleChange->Initialize(aTrack);
        return G4VRestDiscreteProcess::AtRestDoIt(aTrack, aStep);
    }

    Lineage LArNESTScintillationProcess::GetChildType(
        const G4Track* parent, const G4Track* child
    ) const 
    {
        // logic to determine what processes are kicked off by this track and also set
        // the info

        G4String sec_creator = "";
        if (child->GetCreatorProcess()) {
            sec_creator = child->GetCreatorProcess()->GetProcessName();
        }
        if (
            parent && parent->GetDefinition() == G4Neutron::Definition() &&
            (child->GetDefinition()->GetAtomicNumber() > 0)
        )  // neutron inelastic scatters never join the lineage.
        {
            return Lineage(NEST::LArInteraction::NR);
        } 
        else if (
            parent && parent->GetDefinition()->GetAtomicMass() == 83 &&
            parent->GetDefinition()->GetAtomicNumber() == 36 &&
            parent->GetDefinition()->GetIonLifeTime() * .693 < 2 * 60 * 60 * s &&
            parent->GetDefinition()->GetIonLifeTime() * .693 > 1 * 60 * 60 * s
        ) 
        {
            return Lineage(NEST::LArInteraction::NR);
        } 
        else if (parent && parent->GetDefinition() == G4Gamma::Definition()) 
        {
            if (sec_creator.contains("compt")) {
                return Lineage(NEST::LArInteraction::ER);
            } else if (sec_creator.contains("conv")) {  // conv is pair production
                return Lineage(NEST::LArInteraction::ER);
            } else if (sec_creator.contains("phot")) {
                return Lineage(NEST::LArInteraction::ER);
            }
        } else if (
            child->GetDefinition() == G4Electron::Definition() &&
            (sec_creator.contains("Decay") || !parent)
        ) 
        {
            return Lineage(NEST::LArInteraction::ER);
        } 
        else if (
            child->GetDefinition()->GetAtomicMass() > 1 &&
            (sec_creator.contains("Decay") || !parent)
        ) 
        {
            return Lineage(NEST::LArInteraction::ER);
        }
        return Lineage(NEST::LArInteraction::NoneType);
    }

    G4VParticleChange* LArNESTScintillationProcess::PostStepDoIt(
        const G4Track& aTrack, const G4Step& aStep
    ) 
    {
        pParticleChange->Initialize(aTrack);

        auto myLinID = track_lins.find(
            std::make_tuple(aTrack.GetParentID(), aTrack.GetVertexPosition(),
                            aTrack.GetVertexMomentumDirection())
        );
        // Type of this step.
        NEST::LArInteraction step_type = NEST::LArInteraction::NoneType;
        uint sec_mylinid = 0;
        const vector<const G4Track*> secondaries = *aStep.GetSecondaryInCurrentStep();

        // If the current track is already in a lineage, its secondaries inherit that
        // lineage.
        bool break_lineage = true;
        if (myLinID != track_lins.end()) 
        {
            break_lineage = false;
            // break_lineage |=
            //     (lineages[myLinID->second].type ==
            //     ion);  // ions do not pass on their lineage to their secondaries, since
            //             // those secondaries are radioactive decay
            if (aTrack.GetDefinition() == G4Gamma::Definition()) 
            {
                G4ThreeVector dist_from_vertex =
                    aTrack.GetVertexPosition() - aStep.GetPostStepPoint()->GetPosition();
                if (dist_from_vertex.mag() > gamma_break) {
                    break_lineage = true;  // long-travelling gammas break the lineage,
                                        // since they are excluded from the single-site
                                        // experimental results infomring the NEST model
                }
            }
            if (!break_lineage) 
            {
                for (const G4Track* sec : secondaries) 
                {
                    if (sec->GetDefinition() == G4OpticalPhoton::Definition() ||
                        sec->GetCreatorProcess()->GetProcessName().contains("annihil")
                    ) {
                        continue;
                    }
                    track_lins.insert(
                        make_pair(make_tuple(
                            sec->GetParentID(), sec->GetPosition(),
                            sec->GetMomentumDirection()), myLinID->second)
                    );
                    // if (verbose > 2) 
                    // {
                    //     G4cout << "added "
                    //         << sec->GetDynamicParticle()
                    //             ->GetParticleDefinition()
                    //             ->GetParticleName()
                    //         << " to lineage " << lineages.size() - 1 << " type "
                    //         << lineages[myLinID->second].type << " parent "
                    //         << aTrack.GetDefinition()->GetParticleName() << G4endl;
                    // }
                }
            }
        }
        // otherwise, we may need to start a new lineage
        if (break_lineage) 
        {
            // What if the parent is a primary? Give it a lineage just as if it were one
            // of its own secondaries
            if (aTrack.GetParentID() == 0 && myLinID == track_lins.end()) 
            {
                Lineage sec_lin = GetChildType(0, &aTrack);
                NEST::LArInteraction sec_type = sec_lin.type;
                if (sec_type != NEST::LArInteraction::NoneType) 
                {
                    lineages.push_back(sec_lin);
                    // if (verbose > 1) {
                    //     G4cout << "Made new lineage from primary of type " << sec_type << G4endl;
                    // }

                    track_lins.insert(std::make_pair(
                        make_tuple(aTrack.GetParentID(), aTrack.GetVertexPosition(),
                                aTrack.GetVertexMomentumDirection()), lineages.size() - 1)
                    );
                    myLinID = --track_lins.end();
                    for (const G4Track* sec : secondaries) 
                    {
                        if (sec->GetDefinition() == G4OpticalPhoton::Definition() ||
                            sec->GetCreatorProcess()->GetProcessName().contains("annihil")
                        ) 
                        {
                            continue;
                        }
                        step_type = sec_type;
                        track_lins.insert(
                            make_pair(make_tuple(sec->GetParentID(), sec->GetPosition(),
                                                sec->GetMomentumDirection()),
                                    lineages.size() - 1)
                        );
                        if (verbose > 2) 
                        {
                            std::cout << "added "
                                << sec->GetDynamicParticle()
                                    ->GetParticleDefinition()
                                    ->GetParticleName()
                                << " to lineage " << lineages.size() - 1 << std::endl;
                        }
                    }
                }
            }

            for (const G4Track* sec : secondaries) 
            {
                // Each secondary has a type (including the possible NEST::LArInteraction::NoneType)
                Lineage sec_lin = GetChildType(&aTrack, sec);
                NEST::LArInteraction sec_type = sec_lin.type;
                // The first secondary will change the step_type. Subsequent secondaries
                // better have the same type as the first. If they don't, something is
                // weird
                assert(sec_type == step_type || sec_type == NEST::LArInteraction::NoneType ||
                        step_type == NEST::LArInteraction::NoneType
                );
                // if this is the first secondary to have a non-None type, we've started a
                // new lineage
                if (
                    sec_type != NEST::LArInteraction::NoneType &&
                    (step_type == NEST::LArInteraction::NoneType)
                ) 
                {
                    // if (verbose > 1)
                    // {
                    //     G4cout << "Made new lineage " << lineages.size()
                    //         << " from secondary of particle " << aTrack.GetTrackID() << "("
                    //         << aTrack.GetDefinition()->GetParticleName() << " -> "
                    //         << sec->GetDefinition()->GetParticleName() << ")"
                    //         << " of type " << sec_type
                    //         << sec->GetCreatorProcess()->GetProcessName() << G4endl;
                    // }
                    lineages.push_back(sec_lin);
                }
                step_type = sec_type;
                // If the secondary has a non-None type, it also gets a lineage ID.
                if (sec_type != NEST::LArInteraction::NoneType) 
                {
                    track_lins.insert(
                        std::make_pair(make_tuple(sec->GetParentID(), sec->GetPosition(),
                                                sec->GetMomentumDirection()),
                                    lineages.size() - 1)
                    );
                    // for comptons/PEs to add in recoil energy in the parent step
                    if (sec_type == NEST::LArInteraction::ER) 
                    {
                        myLinID = --track_lins.end();
                        ++sec_mylinid;
                    }
                    if (verbose > 1) {
                        G4cout << "Reassigned myLindID " << sec_mylinid << " times" << G4endl;
                    }
                }
            }
        }
        //  If the current track is part of a lineage...
        auto myLinID2 = track_lins.find(
            make_tuple(aTrack.GetParentID(), aTrack.GetVertexPosition(),
                        aTrack.GetVertexMomentumDirection())
        );
        if (myLinID2 == track_lins.end()) 
        {
            if (sec_mylinid) {
                myLinID2 = myLinID;
            }
            else 
            {
                TryPopLineages(aTrack, aStep);
                return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
            }
        }
        Lineage* myLineage = &lineages.at(myLinID->second);
        //...if the step deposited energy...
        if (aStep.GetTotalEnergyDeposit() <= 0) 
        {
            TryPopLineages(aTrack, aStep);
            return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
        }

        //... in a noble element...
        const G4Material* preMaterial = aStep.GetPreStepPoint()->GetMaterial();
        const G4Material* postMaterial = aStep.GetPostStepPoint()->GetMaterial();
        const G4Element *ElementA = NULL, *ElementB = NULL;
        if (preMaterial) 
        {
            const G4ElementVector* theElementVector1 = preMaterial->GetElementVector();
            ElementA = (*theElementVector1)[0];
        }
        if (postMaterial) 
        {
            const G4ElementVector* theElementVector2 = postMaterial->GetElementVector();
            ElementB = (*theElementVector2)[0];
        }
        G4int z1, z2;
        G4bool NobleNow = false, NobleLater = false;
        if (ElementA) {
            z1 = (G4int)(ElementA->GetZ());
        }
        else {
            z1 = -1;
        }
        if (ElementB) {
            z2 = (G4int)(ElementB->GetZ());
        }
        else {
            z2 = -1;
        }
        if (z1 == 2 || z1 == 10 || z1 == 18 || z1 == 36 || z1 == 54) {
            NobleNow = true;
        }
        if (z2 == 2 || z2 == 10 || z2 == 18 || z2 == 36 || z2 == 54) {
            NobleLater = true;
        }

        if (!NobleNow) 
        {
            TryPopLineages(aTrack, aStep);
            return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
        }

        // ...retrieve the particle's position, time, attributes at both the
        // beginning and the end of the current step along its track...
        G4StepPoint* pPreStepPoint = aStep.GetPreStepPoint();
        G4StepPoint* pPostStepPoint = aStep.GetPostStepPoint();
        G4ThreeVector x1 = pPostStepPoint->GetPosition();
        G4ThreeVector x0 = pPreStepPoint->GetPosition();
        G4double evtStrt = pPreStepPoint->GetGlobalTime();
        G4double t0 = pPreStepPoint->GetGlobalTime();

        G4double Density = preMaterial->GetDensity() / (g / cm3);
        if (myLineage->density == -1) {
            myLineage->density = Density;
        }
        double step_E = aStep.GetTotalEnergyDeposit() / keV;

        // add this hit to the appropriate lineage
        Hit stepHit(step_E, t0, x1);
        myLineage->hits.push_back(stepHit);

        // the end (exiting)
        TryPopLineages(aTrack, aStep);
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