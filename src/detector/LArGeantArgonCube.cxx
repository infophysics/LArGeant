/**
 * @file LArGeantArgonCube.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "LArGeantArgonCube.hh"

namespace largeant
{
    LArGeantArgonCube::LArGeantArgonCube(
        LArGeantArgon argon,
        G4double worldX, 
        G4double worldY, 
        G4double worldZ
    )
    : G4VUserDetectorConstruction()
    , fWorldX(worldX), fEnvX(worldX)
    , fWorldY(worldY), fEnvY(worldY)
    , fWorldZ(worldZ), fEnvZ(worldZ)
    , fArgon(argon)
    {
        DefineMaterials();
    }

    LArGeantArgonCube::~LArGeantArgonCube()
    {
    }

    void LArGeantArgonCube::DefineMaterials()
    {
        G4NistManager *nist = G4NistManager::Instance();
        fWorldMat.reset(nist->FindOrBuildMaterial("G4_AIR"));
        fEnvMat.reset(nist->FindOrBuildMaterial("G4_AIR"));
    }

    G4VPhysicalVolume *LArGeantArgonCube::Construct()
    {
        // create the world volume
        fSolidWorld  = std::make_shared<G4Box>("SolidWorld", fWorldX, fWorldY, fWorldZ);
        fLogicalWorld  = std::make_shared<G4LogicalVolume>(fSolidWorld.get(), fWorldMat.get(), "LogicalWorld");
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
        fSolidEnv    = std::make_shared<G4Box>("SolidEnv", fEnvX, fEnvY, fEnvZ);
        fLogicalEnv  = std::make_shared<G4LogicalVolume>(fSolidEnv.get(), fEnvMat.get(), "LogicalEnv");
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
}