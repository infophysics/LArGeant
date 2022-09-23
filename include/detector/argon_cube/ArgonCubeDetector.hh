/**
 * @file ArgonCubeDetector.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#pragma once
#include <vector>
#include <memory>

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

#include "Detector.hh"
#include "ArgonCubeActiveVolume.hh"

namespace LArGeant
{
    class ArgonCubeDetector : public Detector
    {
    public:
        ArgonCubeDetector();
        ~ArgonCubeDetector();

    private:

    };
}