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
        LArGeantArgon argon,
        G4double worldX, 
        G4double worldY, 
        G4double worldZ,
        G4int bottomNumberX,
        G4int bottomNumberZ,
        G4double bottomThicknessY
    )
    : G4VUserDetectorConstruction()
    , fWorldX(worldX), fEnvX(worldX)
    , fWorldY(worldY), fEnvY(worldY)
    , fWorldZ(worldZ), fEnvZ(worldZ)
    , fBottomNumberX(bottomNumberX)
    , fBottomNumberZ(bottomNumberZ)
    , fBottomThicknessY(bottomThicknessY)
    , fArgon(argon)
    {
        DefineMaterials();
        // set up messenger
        fMessenger = new G4GenericMessenger(this ,"/argon_cube/", "ArgonCube");
        fMessenger->DeclareProperty("bottom_array/numberX", fBottomNumberX, "Bottom SiPM number along x");
        fMessenger->DeclareProperty("bottom_array/numberY", fBottomNumberZ, "Bottom SiPM number along z");
    }

    LArGeantArgonCubeDetector::~LArGeantArgonCubeDetector()
    {
    }
    
    void LArGeantArgonCubeDetector::DefineMaterials()
    {
        G4NistManager *nist = G4NistManager::Instance();
        fWorldMat.reset(nist->FindOrBuildMaterial("G4_AIR"));
        fEnvMat.reset(nist->FindOrBuildMaterial("G4_AIR"));
    }

    G4VPhysicalVolume *LArGeantArgonCubeDetector::Construct()
    {
        // create the world volume
        fSolidWorld.reset(
            new G4Box(
                "SolidWorld", 
                fWorldX+fBottomThicknessY, 
                fWorldY+fBottomThicknessY, 
                fWorldZ+fBottomThicknessY
            )
        );
        fLogicalWorld.reset(
            new G4LogicalVolume(
                fSolidWorld.get(), 
                fWorldMat.get(), 
                "LogicalWorld"
            )
        );
        fPhysicalWorld.reset(
            new G4PVPlacement(
                0, 
                G4ThreeVector(0., 0., 0.),
                fLogicalWorld.get(),
                "PhysicalWorld",
                0, 
                false,
                0
            )
        );
        // create the argon Cube volume
        fSolidCube.reset(
            new G4Box(
                "LArCube", 
                fWorldX, 
                fWorldY, 
                fWorldZ
            )
        );
        fLogicalCube.reset(
            new G4LogicalVolume(
                fSolidCube.get(), 
                fArgon.getLAr().get(), 
                "LogicalCube"
            )
        );
        fPhysicalCube.reset(
            new G4PVPlacement(
                0, 
                G4ThreeVector(0., 0., 0.), 
                fLogicalCube.get(), 
                "PhysicalCube", 
                fLogicalWorld.get(), 
                false, 
                0, 
                true
            )
        );

        // construct bottom sipm array
        fBottomSiPMSolidDetector.reset(
            new G4Box(
                "SolidDetector",
                (fWorldX)/fBottomNumberX,
                fBottomThicknessY/2.0,
                (fWorldZ)/fBottomNumberZ
            )
        ); 
        fBottomSiPMLogicalDetector.reset(
            new G4LogicalVolume(
                fBottomSiPMSolidDetector.get(),
                fWorldMat.get(),
                "LogicalDetector"
            )
        );      
        fBottomSiPMPhysicalDetector.clear();
        for (G4int ii = 0; ii < fBottomNumberX; ii++)
        {
            for (G4int jj = 0; jj < fBottomNumberZ; jj++)
            {
                fBottomSiPMPhysicalDetector.emplace_back(
                    new G4PVPlacement(
                        0,
                        G4ThreeVector(
                            -(fWorldX) + (fWorldX)/fBottomNumberX + ii * (2 * fWorldX)/fBottomNumberX,
                            -(fWorldY+fBottomThicknessY/2.0),
                            -(fWorldZ) + (fWorldZ)/fBottomNumberZ + jj * (2 * fWorldZ)/fBottomNumberZ
                        ),
                        fBottomSiPMLogicalDetector.get(),
                        "PhysicalDetector",
                        fLogicalWorld.get(),
                        false,
                        ii + jj * fBottomNumberZ,
                        true
                    )
                );
            }
        }
                
        fScoringVolume = fLogicalCube;
        return fPhysicalWorld.get();
    }

    void LArGeantArgonCubeDetector::ConstructSDandField()
    {
        fBottomSiPMSensitiveDetector = std::make_shared<LArGeantArgonCubeSiPMSensitiveDetector>("bottom_sipm");
        fBottomSiPMLogicalDetector->SetSensitiveDetector(fBottomSiPMSensitiveDetector.get());
    }
}