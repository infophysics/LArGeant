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
        // set up messenger
        fMessenger = new G4GenericMessenger(this ,"/argon_cube/", "ArgonCube");
        fMessenger->DeclareProperty("numberX", fNumberX, "Number of Rows");
        fMessenger->DeclareProperty("numberY", fNumberY, "Number of Columns");
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
                fWorldX+fThickness, 
                fWorldY+fThickness, 
                fWorldZ+fThickness
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

        fSolidDetector.reset(
            new G4Box(
                "SolidDetector",
                (fWorldX)/fNumberX,
                (fWorldY)/fNumberY,
                fThickness/2.0      // half-height
            )
        ); 
        fLogicalDetector.reset(
            new G4LogicalVolume(
                fSolidDetector.get(),
                fWorldMat.get(),
                "LogicalDetector"
            )
        );      
        fFrontFacePhysicalDetector.clear();
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
                        ii + jj * fNumberY,
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