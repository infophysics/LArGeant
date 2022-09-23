/**
 * @file DetectorComponent.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "DetectorComponent.hh"

namespace LArGeant
{
    DetectorComponent::DetectorComponent()
    {
    }

    DetectorComponent::~DetectorComponent()
    {
    }

    void DetectorComponent::SetSolidVolume(G4VSolid* solid)
    {
        mSolidVolume.reset(solid);
    }

    void DetectorComponent::SetLogicalVolume(G4LogicalVolume* logical)
    {
        mLogicalVolume.reset(logical);
    }

    void DetectorComponent::SetPhysicalVolume(G4VPhysicalVolume* physical)
    {
        mPhysicalVolume.reset(physical);
    }
}