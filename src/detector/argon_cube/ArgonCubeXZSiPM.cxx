/**
 * @file ArgonCubeXZSiPM.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArgonCubeXZSiPM.hh"

namespace LArGeant
{
    ArgonCubeXZSiPM::ArgonCubeXZSiPM(
        G4int SiPMID,
        G4double CubeX, 
        G4double CubeY, 
        G4double CubeZ,
        G4ThreeVector Position
    )
    : DetectorComponent("ArgonCubeXZSiPM_" + std::to_string(SiPMID), true)
    , mSiPMID(SiPMID)
    , mCubeX(CubeX)
    , mCubeY(CubeY)
    , mCubeZ(CubeZ)
    , mPosition(Position)
    {
        mMaterial.reset(G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR"));
    }

    void ArgonCubeXZSiPM::Construct()
    {
        // create the argon Cube volume
        SetSolidVolume(
            new G4Box(
                "SiPM_"+std::to_string(mSiPMID) + "_SolidVolume", 
                mCubeX, 
                mCubeY, 
                mCubeZ
            )
        );
        SetLogicalVolume(
            new G4LogicalVolume(
                GetSolidVolumePointer(), 
                mMaterial.get(), 
                "SiPM_"+std::to_string(mSiPMID) + "_LogicalVolume"
            )
        );
        SetPhysicalVolume(
            new G4PVPlacement(
                0, 
                mPosition, 
                GetLogicalVolumePointer(), 
                "SiPM_"+std::to_string(mSiPMID) + "_PhysicalVolume", 
                GetMotherLogical(), 
                false, 
                mSiPMID, 
                true
            )
        );
    }

    ArgonCubeXZSiPM::~ArgonCubeXZSiPM()
    {
    }

    void ArgonCubeXZSiPM::ProcessHits(G4Step*, G4TouchableHistory*)
    {
        G4cout << "Hit!" << G4endl;
    }

}