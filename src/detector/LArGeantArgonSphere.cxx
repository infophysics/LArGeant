/**
 * @file LArGeantArgonSphere.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "LArGeantArgonSphere.hh"

namespace largeant
{
    LArGeantArgonSphere::LArGeantArgonSphere(LArGeantArgon argon, G4double radius)
    : G4VUserDetectorConstruction()
    , mRadius(radius)
    , mWorldX(radius), mEnvX(radius)
    , mWorldY(radius), mEnvY(radius)
    , mWorldZ(radius), mEnvZ(radius)
    , mArgon(argon)
    {
        DefineMaterials();
    }

    LArGeantArgonSphere::~LArGeantArgonSphere()
    {
    }

    void LArGeantArgonSphere::DefineMaterials()
    {
        G4NistManager *nist = G4NistManager::Instance();
        mWorldMat.reset(nist->FindOrBuildMaterial("G4_AIR"));
        mEnvMat.reset(nist->FindOrBuildMaterial("G4_AIR"));
    }

    G4VPhysicalVolume *LArGeantArgonSphere::Construct()
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
        // create the argon sphere volume
        mSolidSphere = std::make_shared<G4Sphere>(
            "LArGeant", 
            0, 
            mRadius, 
            0, 
            2*CLHEP::pi, 
            0, 
            2*CLHEP::pi
        );
        
        mLogicalSphere = std::make_shared<G4LogicalVolume>(mSolidSphere.get(), mArgon.getLAr().get(), "LogicalSphere");
        mPhysicalSphere.reset(
            new G4PVPlacement(
                0, 
                G4ThreeVector(0., 0., 0.), 
                mLogicalSphere.get(), 
                "PhysicalSphere", 
                mLogicalEnv.get(), 
                false, 
                0, 
                true
            )
        );
        
        
        mScoringVolume = mLogicalSphere;

        G4Material* material = mPhysicalSphere->GetLogicalVolume()->GetMaterial();
        std::cout << "LAr material properties:" << std::endl;
        std::cout << "  [name]:     " << material->GetName() << std::endl;
        std::cout << "  [density]:  " << material->GetDensity() << std::endl;
        std::cout << "  [state]:    " << material->GetState() << std::endl;
        std::cout << "  [temp]:     " << material->GetTemperature() << std::endl;
        std::cout << "  [pressure]: " << material->GetPressure() << std::endl;

        return mPhysicalWorld.get();
    }
}