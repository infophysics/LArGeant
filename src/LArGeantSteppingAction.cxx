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
        G4LogicalVolume *scoringVolume = detectorConstruction->GetScoringVolume().get();

        G4double edep = step->GetTotalEnergyDeposit();
        if (volume != scoringVolume) {
            return;
        }

        G4AnalysisManager *man = G4AnalysisManager::Instance();
        G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
        G4String particle = step->GetTrack()->GetParticleDefinition()->GetParticleName();
        G4StepPoint* preStepPoint = step->GetPreStepPoint();
        G4StepPoint* postStepPoint = step->GetPostStepPoint();

        // get the particle position
        G4ThreeVector particlePosition = preStepPoint->GetPosition();
        G4ThreeVector particleMomentum = preStepPoint->GetMomentum();

        // run NEST
        NEST::LArInteraction interaction = GetLArInteraction(particle);
        NEST::LArNESTResult result = fLArNEST->FullCalculation(
            interaction,
            edep/keV,
            500.0,
            1.393,
            false
        );

        if (std::isnan(result.fluctuations.NphFluctuation) || std::isnan(result.fluctuations.NeFluctuation)) {
            return;
        }

        man->FillNtupleIColumn(2, 0, evt);
        man->FillNtupleSColumn(2, 1, particle);
        man->FillNtupleDColumn(2, 2, edep);
        man->FillNtupleDColumn(2, 3, particlePosition[0]);
        man->FillNtupleDColumn(2, 4, particlePosition[1]);
        man->FillNtupleDColumn(2, 5, particlePosition[2]);
        man->FillNtupleDColumn(2, 6, result.fluctuations.NphFluctuation);
        man->FillNtupleDColumn(2, 7, result.fluctuations.NeFluctuation);
        man->AddNtupleRow(2);
    }
}