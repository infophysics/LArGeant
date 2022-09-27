/**
 * @file ScintillationPhysics.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "ScintillationPhysics.hh"

namespace LArGeant
{
    ScintillationPhysics::ScintillationPhysics(G4String name)
    : G4VPhysicsConstructor(name)
    {
    }

    ScintillationPhysics::~ScintillationPhysics()
    {
    }

    void ScintillationPhysics::ConstructProcess()
    {
        G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
    
        // Add standard EM Processes
        //
        G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
        theParticleIterator->reset();
        while ((*theParticleIterator)() ) {
            G4ParticleDefinition* particle = theParticleIterator->value();
            G4String particleName = particle->GetParticleName();
            
            if (particleName == "gamma") {
                ////ph->RegisterProcess(new G4RayleighScattering, particle);
                ph->RegisterProcess(new G4PhotoElectricEffect, particle);
                G4ComptonScattering* cs = new G4ComptonScattering;
                cs->SetEmModel(new G4KleinNishinaModel());
                ph->RegisterProcess(cs, particle);
                ph->RegisterProcess(new G4GammaConversion, particle);
                
            } else if (particleName == "e-") {
                ph->RegisterProcess(new G4eMultipleScattering(), particle);
                G4eIonisation* eIoni = new G4eIonisation();
                eIoni->SetStepFunction(0.1, 100 * um);
                ph->RegisterProcess(eIoni, particle);
                ph->RegisterProcess(new G4eBremsstrahlung(), particle);
                
            } else if (particleName == "e+") {
                ph->RegisterProcess(new G4eMultipleScattering(), particle);
                G4eIonisation* eIoni = new G4eIonisation();
                eIoni->SetStepFunction(0.1, 100 * um);
                ph->RegisterProcess(eIoni, particle);
                ph->RegisterProcess(new G4eBremsstrahlung(), particle);
                ph->RegisterProcess(new G4eplusAnnihilation(), particle);
                
            } else if (particleName == "mu+" || particleName == "mu-") {
                ph->RegisterProcess(new G4MuMultipleScattering(), particle);
                G4MuIonisation* muIoni = new G4MuIonisation();
                muIoni->SetStepFunction(0.1, 50 * um);
                ph->RegisterProcess(muIoni, particle);
                ph->RegisterProcess(new G4MuBremsstrahlung(), particle);
                ph->RegisterProcess(new G4MuPairProduction(), particle);
                
            } else if (particleName == "proton" || particleName == "pi-" ||
                    particleName == "pi+") {
                ph->RegisterProcess(new G4hMultipleScattering(), particle);
                G4hIonisation* hIoni = new G4hIonisation();
                hIoni->SetStepFunction(0.1, 20 * um);
                ph->RegisterProcess(hIoni, particle);
                ph->RegisterProcess(new G4hBremsstrahlung(), particle);
                ph->RegisterProcess(new G4hPairProduction(), particle);
                
            } else if (particleName == "alpha" || particleName == "He3") {
                ph->RegisterProcess(new G4hMultipleScattering(), particle);
                G4ionIonisation* ionIoni = new G4ionIonisation();
                ionIoni->SetStepFunction(0.1, 1 * um);
                ph->RegisterProcess(ionIoni, particle);
                ph->RegisterProcess(new G4NuclearStopping(), particle);
                
            } else if (particleName == "GenericIon") {
                ph->RegisterProcess(new G4hMultipleScattering(), particle);
                G4ionIonisation* ionIoni = new G4ionIonisation();
                ionIoni->SetEmModel(new G4IonParametrisedLossModel());
                ionIoni->SetStepFunction(0.1, 1 * um);
                ph->RegisterProcess(ionIoni, particle);
                ph->RegisterProcess(new G4NuclearStopping(), particle);
                
            } else if ((!particle->IsShortLived()) &&
                    (particle->GetPDGCharge() != 0.0) &&
                    (particle->GetParticleName() != "chargedgeantino")) {
                // all others charged particles except geantino
                ph->RegisterProcess(new G4hMultipleScattering(), particle);
                ph->RegisterProcess(new G4hIonisation(), particle);
            }
        }
    
        // Em options
        //
        // Main options and setting parameters are shown here.
        // Several of them have default values.
        //
        G4EmParameters* EmParameters = G4EmParameters::Instance();
        
        // physics tables
        //
        EmParameters->SetMinEnergy(10 * eV);      // default 100 eV
        EmParameters->SetMaxEnergy(10 * TeV);     // default 100 TeV
        EmParameters->SetNumberOfBinsPerDecade(12 * 10);    // default=12*7
        
        // multiple coulomb scattering
        //
        EmParameters->SetMscStepLimitType(fUseSafety);  // default
        
        // Deexcitation
        //
        G4VAtomDeexcitation* de = new G4UAtomicDeexcitation();
        de->SetFluo(true);
        de->SetAuger(false);
        de->SetPIXE(true);
        G4LossTableManager::Instance()->SetAtomDeexcitation(de);
        
        G4EmModelActivator mact(GetPhysicsName());


        auto particleIteratorP=GetParticleIterator();
        particleIteratorP->reset();

        LArDetector* detector = new LArDetector();
        auto lar_nest = new NEST::LArNEST(detector);  

        while( (*particleIteratorP)() )
        {
            G4ParticleDefinition* particle = particleIteratorP->value();
            G4ProcessManager* pmanager = particle->GetProcessManager();
            G4String particleName = particle->GetParticleName();

            if ( !(particleName.find("e-")!=std::string::npos  || particleName.find("opticalphoton")!=std::string::npos ) ) {
                continue;
            }
            if (pmanager) 
            {
                LArNESTScintillationProcess* theNEST2ScintillationProcess = new LArNESTScintillationProcess("S1", fElectromagnetic, lar_nest, detector); //gndet);
                theNEST2ScintillationProcess->SetDetailedSecondaries(true);
                theNEST2ScintillationProcess->SetStackElectrons(true);

                if (theNEST2ScintillationProcess->IsApplicable(*particle) && pmanager) 
                {
                    pmanager->AddProcess(theNEST2ScintillationProcess, ordDefault + 1, ordInActive, ordDefault + 1);
                }

                G4OpBoundaryProcess* fBoundaryProcess = new G4OpBoundaryProcess();
                G4OpAbsorption* fAbsorptionProcess = new G4OpAbsorption();
                G4OpWLS* fTheWLSProcess = new G4OpWLS();

                if (particleName == "opticalphoton" && pmanager) 
                {
                    pmanager->AddDiscreteProcess(fAbsorptionProcess);
                    // pmanager->AddDiscreteProcess(fRayleighScatteringProcess);
                    pmanager->AddDiscreteProcess(fTheWLSProcess);
                    pmanager->AddDiscreteProcess(fBoundaryProcess);
                }
            }
        }
    }
}