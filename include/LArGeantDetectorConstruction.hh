/**
 * @file LArGeantDetectorConstruction.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Sphere.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

namespace largeant
{
    class LArGeantDetectorConstruction : public G4VUserDetectorConstruction
    {
    public:
        LArGeantDetectorConstruction();
        ~LArGeantDetectorConstruction();

        virtual G4VPhysicalVolume* Construct();

        G4LogicalVolume* getLogicalWorldVolume()    { return fLogicalWorld; }
        G4LogicalVolume* getLogicalEnvVolume()      { return fLogicalEnv; }
        G4LogicalVolume* getLogicalSphereVolume()   { return fLogicalSphere; }

        G4VPhysicalVolume* getPhysicalWorldVolume()  { return fPhysicalWorld; }
        G4VPhysicalVolume* getPhysicalEnvVolume()    { return fPhysicalEnv; }
        G4VPhysicalVolume* getPhysicalSphereVolume() { return fPhysicalSphere; }

        G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

    private:
        // parameters for the geometry of the sphere
        G4double fRadius; 
        G4double fWorldX, fWorldY, fWorldZ;
        G4double fEnvX, fEnvY, fEnvZ;
        // G4 volumes for the sphere
        G4Box               *fSolidWorld, *fSolidEnv;
        /// sphere
        G4Sphere            *fSolidSphere;                 
        /// logical volume for the sphere
        G4LogicalVolume     *fLogicalWorld, *fLogicalEnv, *fLogicalSphere;   
        /// physical volume for the sphere
        G4VPhysicalVolume   *fPhysicalWorld,  *fPhysicalEnv, *fPhysicalSphere;    

        G4double fTemperature;
        G4double fPressure;

        G4double fAr36Density, fAr38Density, fAr40Density;
        G4double fAr36MassMol, fAr38MassMol, fAr40MassMol;
        G4double fAr36Ratio, fAr38Ratio, fAr40Ratio;

        G4double fAverageMassMol;
        G4double fAverageDensity;
        G4double fNaturalArDensity;

        G4Isotope *fIAr36, *fIAr38, *fIAr40;
        G4Element *fArIsotopes;
        G4Material *fWorldMat, *fEnvMat, *fLAr;

        void DefineMaterials();
        virtual void ConstructSDandField();

        G4GenericMessenger *fMessenger;

    protected:
        G4LogicalVolume*  fScoringVolume;
    };
}