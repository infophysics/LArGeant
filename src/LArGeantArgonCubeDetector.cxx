/**
 * @file LArGeantArgonCubeDetector.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "LArGeantArgonCubeDetector.hh"

namespace largeant
{
    LArGeantArgonCubeDetector::LArGeantArgonCubeDetector(
        LArGeantArgonCube* argonCube
    )
    : G4VUserDetectorConstruction()
    {
        fWorldX = argonCube->getWorldX();
        fWorldY = argonCube->getWorldY();
        fWorldZ = argonCube->getWorldZ();
        G4NistManager *nist = G4NistManager::Instance();
        fWorldMat = nist->FindOrBuildMaterial("G4_AIR");
    }

    LArGeantArgonCubeDetector::~LArGeantArgonCubeDetector()
    {
    }

    G4VPhysicalVolume *LArGeantArgonCubeDetector::Construct()
    {
        fSolidDetector = new G4Box(
            "SolidDetector",
            0.005 * m,      // half-width
            0.005 * m,      // half-length
            0.01 * m        // half-height
        ); 
        fLogicalDetector = new G4LogicalVolume(
            fSolidDetector,
            fWorldMat,
            "LogicalDetector"
        );
        for (G4int ii = 0; ii < fNumberX; ii++)
        {
            for (G4int jj = 0; jj < fNumberY; jj++)
            {
                // G4VPhysicalVolume* physicalDetector = new G4PVPlacement(
                //     0.0,
                //     G4ThreeVector(
                //         -fWorldX + (ii + fWorldX)/fNumberX,
                //         -fWorldY + (jj + fWorldY)/fNumberY,
                //         0
                //     ),
                //     fLogicalDetector,
                //     "PhysicalDetector",
                //     fLogicalWorld,
                //     false,
                //     ii + jj * fNumberX
                // );
            }
        }
    }
}