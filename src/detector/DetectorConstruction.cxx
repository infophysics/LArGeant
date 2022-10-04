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
            electricField->SetLocalFieldValue(G4ThreeVector(0,500.0 * volt/cm, 0));
            G4AutoDelete::Register(electricField);
            mElectricField.Put(electricField);
        }
        std::shared_ptr<SensitiveDetector> Sensitive = std::make_shared<SensitiveDetector>(
            "SensitiveDetector"
        );
        mDetector->SetSensitiveDetector(Sensitive);
        for (G4int ii = 0; ii < mDetector->GetNumberOfComponents(); ii++)
        {
            if(mDetector->GetDetectorComponent(ii)->GetSensitive())
            {
                G4cout << "Adding sensitive detector to " << mDetector->GetDetectorComponent(ii)->GetName() << G4endl;
                mDetector->GetDetectorComponent(ii)->GetLogicalVolume()->SetSensitiveDetector(mDetector->GetSensitiveDetectorPointer());
            }
            if(mDetector->GetDetectorComponent(ii)->GetElectricField())
            {
                mDetector->GetDetectorComponent(ii)->GetLogicalVolume()->SetFieldManager(mElectricField.Get()->GetLocalFieldManager(), true);
            }
        }
    }
}