/**
 * @file LArGeantSteppingAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "LArGeantSteppingAction.hh"

namespace largeant
{
    LArGeantSteppingAction::LArGeantSteppingAction(std::shared_ptr<LArGeantEventAction> eventAction)
    : G4UserSteppingAction()
    {
        fEventAction = eventAction;
        LArDetector* detector = new LArDetector();
        fLArNEST = std::make_shared<NEST::LArNEST>(detector);
    }

    LArGeantSteppingAction::~LArGeantSteppingAction()
    {}

    void LArGeantSteppingAction::UserSteppingAction(const G4Step *step)
    {   
        // determine the stepping volume
        G4LogicalVolume *volume 
            = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
        const LArGeantArgonCubeDetector *detectorConstruction 
            = static_cast<const LArGeantArgonCubeDetector*> 
            (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        
        // check that edep is in the active volume
        G4LogicalVolume *scoringVolume = detectorConstruction->GetScoringVolume().get();
        if (volume != scoringVolume) {
            return;
        }

        // get edep value in keV (for NEST)
        G4double edep = step->GetTotalEnergyDeposit()/keV;
        G4double dx = step->GetStepLength();

        // get efield value
        G4double efield = fEventAction->GetElectricField();

        // get particle info
        G4AnalysisManager *man = G4AnalysisManager::Instance();
        G4int event = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
        G4String particle = step->GetTrack()->GetParticleDefinition()->GetParticleName();
        G4StepPoint* preStepPoint = step->GetPreStepPoint();
        G4StepPoint* postStepPoint = step->GetPostStepPoint();

        // get the particle position
        G4ThreeVector particleStartPosition = preStepPoint->GetPosition();
        G4ThreeVector particleEndPosition = postStepPoint->GetPosition();
        G4ThreeVector particleMomentum = preStepPoint->GetMomentum();

        // run NEST
        NEST::LArInteraction interaction = GetLArInteraction(particle);
        NEST::LArNESTResult result = fLArNEST->FullCalculation(
            interaction,
            edep,
            efield,
            1.393,
            false
        );

        if (std::isnan(result.fluctuations.NphFluctuation) || std::isnan(result.fluctuations.NeFluctuation)) {
            return;
        }

        // save nest results
        man->FillNtupleIColumn(0, 0, event);
        man->FillNtupleSColumn(0, 1, particle);
        man->FillNtupleDColumn(0, 2, efield);
        man->FillNtupleDColumn(0, 3, edep);
        man->FillNtupleDColumn(0, 4, dx);
        man->FillNtupleSColumn(0, 5, LArInteractionMap[interaction]);
        man->FillNtupleDColumn(0, 6, particleStartPosition[0]);
        man->FillNtupleDColumn(0, 7, particleStartPosition[1]);
        man->FillNtupleDColumn(0, 8, particleStartPosition[2]);
        man->FillNtupleDColumn(0, 9, particleEndPosition[0]);
        man->FillNtupleDColumn(0, 10, particleEndPosition[1]);
        man->FillNtupleDColumn(0, 11, particleEndPosition[2]);
        man->FillNtupleIColumn(0, 12, int(result.fluctuations.NphFluctuation));
        man->FillNtupleIColumn(0, 13, int(result.fluctuations.NeFluctuation));
        man->AddNtupleRow(0);
    }
}