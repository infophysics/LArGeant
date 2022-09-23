/**
 * @file Detector.hh
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

#include "DetectorComponent.hh"

namespace LArGeant
{
    class Detector
    {
    public:
        Detector();
        ~Detector();

        G4int GetNumberOfComponents() { return mDetectorComponents.size(); }
        std::shared_ptr<DetectorComponent> GetDetectorComponent(G4int ii);

        void AddDetectorComponent(std::shared_ptr<DetectorComponent>);

    private:
        std::vector<std::shared_ptr<DetectorComponent>> mDetectorComponents;

    };
}