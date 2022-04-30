/**
 * @file LArGeantArgonSphere.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include <memory>
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

#include "LArGeantArgon.hh"

namespace largeant
{
    class LArGeantArgonSphere : public G4VUserDetectorConstruction
    {
    public:
        LArGeantArgonSphere(LArGeantArgon argon, G4double radius);
        ~LArGeantArgonSphere();

        virtual G4VPhysicalVolume* Construct();

        std::shared_ptr<G4LogicalVolume> getLogicalWorldVolume()    { return fLogicalWorld; }
        std::shared_ptr<G4LogicalVolume> getLogicalEnvVolume()      { return fLogicalEnv; }
        std::shared_ptr<G4LogicalVolume> getLogicalSphereVolume()   { return fLogicalSphere; }

        std::shared_ptr<G4VPhysicalVolume> getPhysicalWorldVolume()  { return fPhysicalWorld; }
        std::shared_ptr<G4VPhysicalVolume> getPhysicalEnvVolume()    { return fPhysicalEnv; }
        std::shared_ptr<G4VPhysicalVolume> getPhysicalSphereVolume() { return fPhysicalSphere; }

        std::shared_ptr<G4LogicalVolume> GetScoringVolume() const { return fScoringVolume; }

    private:
        // parameters for the geometry of the sphere
        G4double fRadius; 
        G4double fWorldX, fWorldY, fWorldZ;
        G4double fEnvX, fEnvY, fEnvZ;
        // G4 volumes for the sphere
        std::shared_ptr<G4Box>  fSolidWorld, fSolidEnv;
        /// sphere
        std::shared_ptr<G4Sphere> fSolidSphere;                 
        /// logical volume for the sphere
        std::shared_ptr<G4LogicalVolume> fLogicalWorld, fLogicalEnv, fLogicalSphere;   
        /// physical volume for the sphere
        std::shared_ptr<G4VPhysicalVolume> fPhysicalWorld = {nullptr};
        std::shared_ptr<G4VPhysicalVolume> fPhysicalEnv = {nullptr};
        std::shared_ptr<G4VPhysicalVolume> fPhysicalSphere = {nullptr};    

        LArGeantArgon fArgon;
        std::shared_ptr<G4Material> fWorldMat = {nullptr};
        std::shared_ptr<G4Material> fEnvMat = {nullptr};

        void DefineMaterials();

        G4GenericMessenger *fMessenger;

    protected:
        std::shared_ptr<G4LogicalVolume>  fScoringVolume;
    };
}