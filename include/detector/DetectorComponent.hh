/**
 * @file DetectorComponent.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#pragma once
#include <memory>

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"

namespace LArGeant
{
    class DetectorComponent
    {
    public:
        DetectorComponent();
        ~DetectorComponent();

        virtual void Construct() = 0;

        void SetSolidVolume(G4VSolid* solid);
        void SetLogicalVolume(G4LogicalVolume* logical);
        void SetPhysicalVolume(G4VPhysicalVolume* physical);

        void SetMotherLogical(G4LogicalVolume* mother) { mMotherLogicalVolume.reset(mother); }
        G4LogicalVolume* GetMotherLogical() { return mMotherLogicalVolume.get(); }

        std::shared_ptr<G4VSolid> GetSolidVolume() { return mSolidVolume; }
        std::shared_ptr<G4LogicalVolume> GetLogicalVolume() { return mLogicalVolume; }
        std::shared_ptr<G4VPhysicalVolume> GetPhysicalVolume() { return mPhysicalVolume; }

        G4VSolid* GetSolidVolumePointer() { return mSolidVolume.get(); }
        G4LogicalVolume* GetLogicalVolumePointer() { return mLogicalVolume.get(); }
        G4VPhysicalVolume* GetPhysicalVolumePointer() { return mPhysicalVolume.get(); }

    private:
        std::shared_ptr<G4VSolid> mSolidVolume = {nullptr};
        std::shared_ptr<G4LogicalVolume> mLogicalVolume = {nullptr};
        std::shared_ptr<G4VPhysicalVolume> mPhysicalVolume = {nullptr};

        std::shared_ptr<G4LogicalVolume> mMotherLogicalVolume = {nullptr};

    };
}