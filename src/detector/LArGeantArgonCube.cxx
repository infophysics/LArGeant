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
    , mWorldX(worldX), mEnvX(worldX)
    , mWorldY(worldY), mEnvY(worldY)
    , mWorldZ(worldZ), mEnvZ(worldZ)
    , mArgon(argon)
    {
        DefineMaterials();
    }

    LArGeantArgonCube::~LArGeantArgonCube()
    {
    }

    void LArGeantArgonCube::DefineMaterials()
    {
        G4NistManager *nist = G4NistManager::Instance();
        mWorldMat.reset(nist->FindOrBuildMaterial("G4_AIR"));
        mEnvMat.reset(nist->FindOrBuildMaterial("G4_AIR"));
    }

    G4VPhysicalVolume *LArGeantArgonCube::Construct()
    {
        // create the world volume
        mSolidWorld  = std::make_shared<G4Box>("SolidWorld", mWorldX, mWorldY, mWorldZ);
        mLogicalWorld  = std::make_shared<G4LogicalVolume>(mSolidWorld.get(), mWorldMat.get(), "LogicalWorld");
        mPhysicalWorld.reset(
            new G4PVPlacement(
                0, 
                G4ThreeVector(0., 0., 0.),
                mLogicalWorld.get(),
                "PhysicalWorld",
                0, 
                false,
                0
            )
        );
        // create the envelope volume
        mSolidEnv    = std::make_shared<G4Box>("SolidEnv", mEnvX, mEnvY, mEnvZ);
        mLogicalEnv  = std::make_shared<G4LogicalVolume>(mSolidEnv.get(), mEnvMat.get(), "LogicalEnv");
        mPhysicalEnv.reset(
            new G4PVPlacement(
                0,
                G4ThreeVector(0., 0., 0.),
                mLogicalEnv.get(),
                "PhysicalEnv",
                mLogicalWorld.get(),
                false,
                0,
                true
            )
        );
        // create the argon Cube volume
        mSolidCube = std::make_shared<G4Box>(
            "LArCube", 
            mWorldX, 
            mWorldY, 
            mWorldZ
        );
        mLogicalCube = std::make_shared<G4LogicalVolume>(mSolidCube.get(), mArgon.getLAr().get(), "LogicalCube");
        mPhysicalCube.reset(
            new G4PVPlacement(
                0, 
                G4ThreeVector(0., 0., 0.), 
                mLogicalCube.get(), 
                "PhysicalCube", 
                mLogicalEnv.get(), 
                false, 
                0, 
                true
            )
        );
        
        
        mScoringVolume = mLogicalCube;

        G4Material* material = mPhysicalCube->GetLogicalVolume()->GetMaterial();
        std::cout << "LAr material properties:" << std::endl;
        std::cout << "  [name]:     " << material->GetName() << std::endl;
        std::cout << "  [density]:  " << material->GetDensity() << std::endl;
        std::cout << "  [state]:    " << material->GetState() << std::endl;
        std::cout << "  [temp]:     " << material->GetTemperature() << std::endl;
        std::cout << "  [pressure]: " << material->GetPressure() << std::endl;

        return mPhysicalWorld.get();
    }
}