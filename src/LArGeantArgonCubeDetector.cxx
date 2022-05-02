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
        G4int numberX,
        G4int numberY,
        G4double thickness
    )
    : G4VUserDetectorConstruction()
    , fWorldX(worldX), fEnvX(worldX)
    , fWorldY(worldY), fEnvY(worldY)
    , fWorldZ(worldZ), fEnvZ(worldZ)
    , fNumberX(numberX)
    , fNumberY(numberY)
    , fThickness(thickness)
    , fArgon(argon)
    {
        DefineMaterials();
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
        fSolidWorld  = std::make_shared<G4Box>(
            "SolidWorld", 
            fWorldX+fThickness, 
            fWorldY+fThickness, 
            fWorldZ+fThickness
        );
        fLogicalWorld = std::make_shared<G4LogicalVolume>(fSolidWorld.get(), fWorldMat.get(), "LogicalWorld");
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
        // create the envelope volume
        fSolidEnv = std::make_shared<G4Box>(
            "SolidEnv", 
            fEnvX+fThickness, 
            fEnvY+fThickness, 
            fEnvZ+fThickness
        );
        fLogicalEnv = std::make_shared<G4LogicalVolume>(fSolidEnv.get(), fEnvMat.get(), "LogicalEnv");
        fPhysicalEnv.reset(
            new G4PVPlacement(
                0,
                G4ThreeVector(0., 0., 0.),
                fLogicalEnv.get(),
                "PhysicalEnv",
                fLogicalWorld.get(),
                false,
                0,
                true
            )
        );
        // create the argon Cube volume
        fSolidCube = std::make_shared<G4Box>(
            "LArCube", 
            fWorldX, 
            fWorldY, 
            fWorldZ
        );
        fLogicalCube = std::make_shared<G4LogicalVolume>(fSolidCube.get(), fArgon.getLAr().get(), "LogicalCube");
        fPhysicalCube.reset(
            new G4PVPlacement(
                0, 
                G4ThreeVector(0., 0., 0.), 
                fLogicalCube.get(), 
                "PhysicalCube", 
                fLogicalEnv.get(), 
                false, 
                0, 
                true
            )
        );

        fSolidDetector = std::make_shared<G4Box>(
            "SolidDetector",
            (fWorldX)/fNumberX,
            (fWorldY)/fNumberY,
            fThickness/2.0      // half-height
        ); 
        fLogicalDetector = std::make_shared<G4LogicalVolume>(
            fSolidDetector.get(),
            fWorldMat.get(),
            "LogicalDetector"
        );      
        for (G4int ii = 0; ii < fNumberX; ii++)
        {
            for (G4int jj = 0; jj < fNumberY; jj++)
            {
                fFrontFacePhysicalDetector.emplace_back(
                    new G4PVPlacement(
                        0,
                        G4ThreeVector(
                            -(fWorldX) + (fWorldX)/fNumberX + ii * (2 * fWorldX)/fNumberX,
                            -(fWorldY) + (fWorldY)/fNumberY + jj * (2 * fWorldY)/fNumberY,
                            fWorldZ+fThickness/2.0
                        ),
                        fLogicalDetector.get(),
                        "PhysicalDetector",
                        fLogicalWorld.get(),
                        false,
                        ii + jj * fNumberX,
                        true
                    )
                );
            }
        }
                
        fScoringVolume = fLogicalCube;

        G4Material* material = fPhysicalCube->GetLogicalVolume()->GetMaterial();
        std::cout << "LAr material properties:" << std::endl;
        std::cout << "  [name]:     " << material->GetName() << std::endl;
        std::cout << "  [density]:  " << material->GetDensity() << std::endl;
        std::cout << "  [state]:    " << material->GetState() << std::endl;
        std::cout << "  [temp]:     " << material->GetTemperature() << std::endl;
        std::cout << "  [pressure]: " << material->GetPressure() << std::endl;

        return fPhysicalWorld.get();
    }

    void LArGeantArgonCubeDetector::ConstructSDandField()
    {
        fSensitiveDetector = std::make_shared<LArGeantArgonCubeSensitiveDetector>("SensitiveDetector");
        fLogicalDetector->SetSensitiveDetector(fSensitiveDetector.get());
    }
}