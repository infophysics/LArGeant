/**
 * @file DetectorConstruction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "DetectorConstruction.hh"

namespace LArGeant
{
    DetectorConstruction::DetectorConstruction(
        G4double ExperimentalHallX,
        G4double ExperimentalHallY,
        G4double ExperimentalHallZ,
        Detector* detector
    )
    : G4VUserDetectorConstruction()
    , mExperimentalHallX(ExperimentalHallX)
    , mExperimentalHallY(ExperimentalHallY)
    , mExperimentalHallZ(ExperimentalHallZ)
    {
        mDetector.reset(detector);
        DefineMaterials();
    }

    void DetectorConstruction::DefineMaterials()
    {
        G4cout << "Constructing Experimental Hall with G4_AIR" << G4endl;
        G4NistManager *nist = G4NistManager::Instance();
        mExperimentalHallMaterial.reset(nist->FindOrBuildMaterial("G4_AIR"));
    }

    DetectorConstruction::~DetectorConstruction()
    {
    }

    void DetectorConstruction::SetDetector(Detector* detector)
    {
        mDetector.reset(detector);
    }

    G4VPhysicalVolume* DetectorConstruction::Construct()
    {
        // create the ExperimentalHall mother
        // volume.
        mSolidExperimentalHall.reset(
            new G4Box(
                "Solid_ExperimentalHall", 
                mExperimentalHallX, 
                mExperimentalHallY, 
                mExperimentalHallZ
            )
        );
        mLogicalExperimentalHall.reset(
            new G4LogicalVolume(
                mSolidExperimentalHall.get(), 
                mExperimentalHallMaterial.get(), 
                "Logical_ExperimentalHall"
            )
        );
        mPhysicalExperimentalHall.reset(
            new G4PVPlacement(
                0, 
                G4ThreeVector(0., 0., 0.),
                mLogicalExperimentalHall.get(),
                "Physical_ExperimentalHall",
                0, 
                false,
                0
            )
        );

        auto Manager = EventManager::GetEventManager();
        for (G4int ii = 0; ii < mDetector->GetNumberOfComponents(); ii++)
        {
            mDetector->GetDetectorComponent(ii)->SetMotherLogical(mLogicalExperimentalHall.get());
            mDetector->GetDetectorComponent(ii)->Construct();
            Manager->AddComponent(mDetector->GetDetectorComponent(ii));
        }

        return mPhysicalExperimentalHall.get();
    }


    void DetectorConstruction::ConstructSDandField()
    {
        if(!mElectricField.Get())
        {
            ElectricField* electricField = new ElectricField(G4ThreeVector(0,0,0));

            // Grab the E-field from the NEST detector
            auto detector = NESTInterface::GetInterface()->GetLArDetector();
            G4double efield = detector->FitEF(0, 0, 0);
            std::vector<G4double> efield_dir = detector->FitDirEF(0, 0, 0);
            G4ThreeVector efield_direction = {
                efield * efield_dir[0] * volt / cm, 
                efield * efield_dir[1] * volt / cm, 
                efield * efield_dir[2] * volt / cm
            };

            electricField->SetLocalFieldValue(efield_direction);
            G4AutoDelete::Register(electricField);
            mElectricField.Put(electricField);
        }
        SensitiveDetector* Sensitive = new SensitiveDetector("SensitiveDetector");
        G4int CopyNumber = 0;
        for (G4int ii = 0; ii < mDetector->GetNumberOfComponents(); ii++)
        {
            if(mDetector->GetDetectorComponent(ii)->GetSensitive()) {
                SetSensitiveDetector(mDetector->GetDetectorComponent(ii)->GetLogicalVolume().get(), Sensitive);
                EventManager::GetEventManager()->SetComponentCopyNumber(CopyNumber, ii);
                CopyNumber += 1;
            }
            if(mDetector->GetDetectorComponent(ii)->GetElectricField()) {
                mDetector->GetDetectorComponent(ii)->GetLogicalVolume()->SetFieldManager(mElectricField.Get()->GetLocalFieldManager(), true);
            }
        }
    }
}