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
    , fRadius(radius)
    , fWorldX(radius), fEnvX(radius)
    , fWorldY(radius), fEnvY(radius)
    , fWorldZ(radius), fEnvZ(radius)
    , fArgon(argon)
    {
        DefineMaterials();
    }

    LArGeantArgonSphere::~LArGeantArgonSphere()
    {
    }

    void LArGeantArgonSphere::DefineMaterials()
    {
        G4NistManager *nist = G4NistManager::Instance();
        fWorldMat.reset(nist->FindOrBuildMaterial("G4_AIR"));
        fEnvMat.reset(nist->FindOrBuildMaterial("G4_AIR"));
    }

    G4VPhysicalVolume *LArGeantArgonSphere::Construct()
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
        // create the argon sphere volume
        fSolidSphere = std::make_shared<G4Sphere>(
            "LArGeant", 
            0, 
            fRadius, 
            0, 
            2*CLHEP::pi, 
            0, 
            2*CLHEP::pi
        );
        
        fLogicalSphere = std::make_shared<G4LogicalVolume>(fSolidSphere.get(), fArgon.getLAr().get(), "LogicalSphere");
        fPhysicalSphere.reset(
            new G4PVPlacement(
                0, 
                G4ThreeVector(0., 0., 0.), 
                fLogicalSphere.get(), 
                "PhysicalSphere", 
                fLogicalEnv.get(), 
                false, 
                0, 
                true
            )
        );
        
        
        fScoringVolume = fLogicalSphere;

        G4Material* material = fPhysicalSphere->GetLogicalVolume()->GetMaterial();
        std::cout << "LAr material properties:" << std::endl;
        std::cout << "  [name]:     " << material->GetName() << std::endl;
        std::cout << "  [density]:  " << material->GetDensity() << std::endl;
        std::cout << "  [state]:    " << material->GetState() << std::endl;
        std::cout << "  [temp]:     " << material->GetTemperature() << std::endl;
        std::cout << "  [pressure]: " << material->GetPressure() << std::endl;

        return fPhysicalWorld.get();
    }
}