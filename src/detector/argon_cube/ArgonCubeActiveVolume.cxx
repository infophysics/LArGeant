/**
 * @file ArgonCubeActiveVolume.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArgonCubeActiveVolume.hh"

namespace LArGeant
{
    ArgonCubeActiveVolume::ArgonCubeActiveVolume(
        G4double CubeX, 
        G4double CubeY, 
        G4double CubeZ
    )
    : DetectorComponent()
    , mCubeX(CubeX)
    , mCubeY(CubeY)
    , mCubeZ(CubeZ)
    {
    }

    void ArgonCubeActiveVolume::Construct()
    {
        /// create the argon object
        mArgon.reset(
            new Argon(
                85.8 * kelvin,      /// temperature
                0.952 * atmosphere, /// pressure
                0.334,              /// ratio of Ar36
                0.063,              /// ratio of Ar38
                99.603              /// ratio of Ar40
            )
        );

        // create the argon Cube volume
        SetSolidVolume(
            new G4Box(
                "LArVolume", 
                mCubeX, 
                mCubeY, 
                mCubeZ
            )
        );
        SetLogicalVolume(
            new G4LogicalVolume(
                GetSolidVolumePointer(), 
                mArgon->GetLAr().get(), 
                "LogicalVolume"
            )
        );
        SetPhysicalVolume(
            new G4PVPlacement(
                0, 
                G4ThreeVector(0., 0., 0.), 
                GetLogicalVolumePointer(), 
                "PhysicalVolume", 
                GetMotherLogical(), 
                false, 
                0, 
                true
            )
        );
    }

    ArgonCubeActiveVolume::~ArgonCubeActiveVolume()
    {
    }
}