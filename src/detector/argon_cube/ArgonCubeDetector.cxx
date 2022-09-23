/**
 * @file ArgonCubeDetector.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-23
 */
#include "ArgonCubeDetector.hh"

namespace LArGeant
{
    ArgonCubeDetector::ArgonCubeDetector()
    : Detector()
    {
        /// create the argon cube active volume
        std::shared_ptr<ArgonCubeActiveVolume> argonCubeActiveVolume 
            = std::make_shared<ArgonCubeActiveVolume>(
            50 * m, 
            50 * m,
            50 * m
        );
        AddDetectorComponent(argonCubeActiveVolume);
    }

    ArgonCubeDetector::~ArgonCubeDetector()
    {
    }
}