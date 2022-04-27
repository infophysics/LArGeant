/**
 * @file LArGeantDetectorConstruction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "LArGeantDetectorConstruction.hh"

namespace largeant
{
    LArGeantDetectorConstruction::LArGeantDetectorConstruction()
    : G4VUserDetectorConstruction()
    {
        // initialize sphere geometry parameters
        fWorldX  = 50001;
        fWorldY  = 50001;
        fWorldZ  = 50001;
        fEnvX    = fWorldX;
        fEnvY    = fWorldY;
        fEnvZ    = fWorldZ;

        fRadius = 50000;
        // initialize argon parameters
        fTemperature = 85.8 * kelvin;
        fPressure    = 0.952 * atmosphere;

        fAr36MassMol = 35.967*g/mole;
        fAr38MassMol = 37.962*g/mole;
        fAr40MassMol = 39.962*g/mole;

        fAr36Ratio = 0.334;
        fAr38Ratio = 0.063;
        fAr40Ratio = 99.603;

        fAverageDensity = 1.406*g/cm3;
        fNaturalArDensity = 1.3973*g/cm3;
    }

    LArGeantDetectorConstruction::~LArGeantDetectorConstruction()
    {
    }

    void LArGeantDetectorConstruction::DefineMaterials()
    {
        G4NistManager *nist = G4NistManager::Instance();

        fIAr36 = new G4Isotope(
            "Ar36",         // name
            18.,            // atomic number
            36,             // atomic mass
            fAr36MassMol);  // molar mass
        
        fIAr38 = new G4Isotope(
            "Ar38",         // name
            18.,            // atomic number
            38,             // atomic mass
            fAr38MassMol);  // molar mass

        fIAr40 = new G4Isotope(
            "Ar40",         // name
            18.,            // atomic number
            40,             // atomic mass
            fAr40MassMol);  // molar mass

        // add the isotopes to the definition of Ar
        fArIsotopes = new G4Element("ArIsotopes", "Ar", 3);
        fArIsotopes->AddIsotope(fIAr36, fAr36Ratio*perCent);
        fArIsotopes->AddIsotope(fIAr38, fAr38Ratio*perCent);
        fArIsotopes->AddIsotope(fIAr40, fAr40Ratio*perCent);

        // need now the definition of LAr with the composition
        fLAr = new G4Material(
            "LAr",          // name
            fAverageDensity,// density
            1,              // # of components
            kStateLiquid,   // state
            fTemperature,   // temperature
            fPressure);     // pressure
        
        fLAr->AddElement(fArIsotopes, 1);

        fWorldMat = nist->FindOrBuildMaterial("G4_AIR");
        fEnvMat   = nist->FindOrBuildMaterial("G4_AIR");
    }

    G4VPhysicalVolume *LArGeantDetectorConstruction::Construct()
    {
        DefineMaterials();
        // create the world volume
        fSolidWorld  = new G4Box("Solid World", fWorldX, fWorldY, fWorldZ);
        fLogicalWorld  = new G4LogicalVolume(fSolidWorld, fWorldMat, "Logical World");
        fPhysicalWorld   = new G4PVPlacement(
            0, 
            G4ThreeVector(0., 0., 0.),
            fLogicalWorld,
            "Physical World",
            0, 
            false,
            0);
        // create the envelope volume
        fSolidEnv    = new G4Box("envWorld", fEnvX, fEnvY, fEnvZ);
        fLogicalEnv    = new G4LogicalVolume(fSolidEnv, fEnvMat, "Logical Env");
        fPhysicalEnv     = new G4PVPlacement(
            0,
            G4ThreeVector(0., 0., 0.),
            fLogicalEnv,
            "Physical Env",
            fLogicalWorld,
            false,
            0,
            true);
        // create the argon sphere volume
        fSolidSphere = new G4Sphere(
            "LArGeant", 
            0, 
            fRadius, 
            0, 
            2*CLHEP::pi, 
            0, 
            2*CLHEP::pi);
        
        fLogicalSphere = new G4LogicalVolume(fSolidSphere, fLAr, "Logical Sphere");
        fPhysicalSphere  = new G4PVPlacement(
            0, 
            G4ThreeVector(0., 0., 0.), 
            fLogicalSphere, 
            "Physical Sphere", 
            fLogicalEnv, 
            false, 
            0, 
            true);
        
        fScoringVolume = fLogicalSphere;

        G4Material* material = fPhysicalSphere->GetLogicalVolume()->GetMaterial();
        std::cout << "LAr material properties:" << std::endl;
        std::cout << "  [name]:     " << material->GetName() << std::endl;
        std::cout << "  [density]:  " << material->GetDensity() << std::endl;
        std::cout << "  [state]:    " << material->GetState() << std::endl;
        std::cout << "  [temp]:     " << material->GetTemperature() << std::endl;
        std::cout << "  [pressure]: " << material->GetPressure() << std::endl;

        return fPhysicalWorld;
    }

}