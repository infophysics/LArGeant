/**
 * @file Detector.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "Detector.hh"

namespace LArGeant
{
    Detector::Detector()
    {
    }

    Detector::~Detector()
    {
    }

    std::shared_ptr<DetectorComponent> Detector::GetDetectorComponent(G4int ii)
    {
        if (ii < mDetectorComponents.size()) {
            return mDetectorComponents[ii];
        }
        else {
            return mDetectorComponents[0];
        }
    }

    void Detector::AddDetectorComponent(std::shared_ptr<DetectorComponent> component)
    {
        mDetectorComponents.emplace_back(component);
    }
}