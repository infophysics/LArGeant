/**
 * @file ArgonCubeSiPMSensitiveDetector.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-05-01
 */
#include "ArgonCubeSiPMSensitiveDetector.hh"

namespace LArGeant
{
    ArgonCubeSiPMSensitiveDetector::ArgonCubeSiPMSensitiveDetector(G4String name)
    : G4VSensitiveDetector(name)
    {
        G4AnalysisManager *man = G4AnalysisManager::Instance();
        // man->CreateNtuple(name, name);
        // man->CreateNtupleIColumn("event");
        // man->CreateNtupleIColumn("track_id");
        // man->CreateNtupleDColumn("x_i");
        // man->CreateNtupleDColumn("y_i");
        // man->CreateNtupleDColumn("z_i");
        // man->CreateNtupleDColumn("energy");
        // man->CreateNtupleDColumn("wavelength");
        // man->CreateNtupleIColumn("sipm_channel");
        // man->CreateNtupleDColumn("sipm_x");
        // man->CreateNtupleDColumn("sipm_y");
        // man->CreateNtupleDColumn("sipm_z");
        // man->FinishNtuple(1);

        // load in efficiencies
        mQuantumEfficiency = new G4PhysicsFreeVector();

        std::ifstream datafile;
        datafile.open("sipm_efficiency.dat");
        while(1)
        {
            G4double wavelength, q_efficiency;
            datafile >> wavelength >> q_efficiency;
            mQuantumEfficiency->InsertValues(wavelength, q_efficiency/100.0);
            if(datafile.eof()) {
                break;
            }
        }
        datafile.close();
    }

    ArgonCubeSiPMSensitiveDetector::~ArgonCubeSiPMSensitiveDetector()
    {
    }

    G4bool ArgonCubeSiPMSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* history)
    {
        G4Track* track = step->GetTrack();
        track->SetTrackStatus(fStopAndKill);
        G4int track_id = track->GetTrackID();

        // check if particle is a photon
        G4String particle = track->GetParticleDefinition()->GetParticleName();
        if (particle != "gamma" && particle != "opticalphoton") {
            return false;
        }

        G4StepPoint* preStepPoint = step->GetPreStepPoint();
        G4StepPoint* postStepPoint = step->GetPostStepPoint();

        // get the photon energy
        G4double photonEnergy = step->GetTotalEnergyDeposit()/keV;

        // get the photon position
        G4ThreeVector photonPosition = preStepPoint->GetPosition();
        G4ThreeVector photonMomentum = preStepPoint->GetMomentum();
        G4double wavelength = (1.239841939 * eV / photonMomentum.mag()) * 1000;

        // determine efficiency
        if(G4UniformRand() >= mQuantumEfficiency->Value(wavelength)) {
            return false;
        }

        // get the detector position
        const G4VTouchable* touchable = step->GetPreStepPoint()->GetTouchable();
        G4int copyNo = touchable->GetCopyNumber();
        G4VPhysicalVolume* physVol = touchable->GetVolume();
        G4ThreeVector detectorPosition = physVol->GetTranslation();

        // get event number
        G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

        G4AnalysisManager *man = G4AnalysisManager::Instance();

        man->FillNtupleIColumn(1, 0, evt);
        man->FillNtupleIColumn(1, 1, track_id);
        man->FillNtupleDColumn(1, 2, photonPosition[0]);
        man->FillNtupleDColumn(1, 3, photonPosition[1]);
        man->FillNtupleDColumn(1, 4, photonPosition[2]);
        man->FillNtupleDColumn(1, 5, photonEnergy);
        man->FillNtupleDColumn(1, 6, wavelength);
        man->FillNtupleIColumn(1, 7, copyNo);
        man->FillNtupleDColumn(1, 8, detectorPosition[0]);
        man->FillNtupleDColumn(1, 9, detectorPosition[1]);
        man->FillNtupleDColumn(1, 10, detectorPosition[2]);
        man->AddNtupleRow(1);
        
        return true;
    }
}