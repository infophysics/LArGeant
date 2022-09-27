/**
 * @file ArgonSphere.hh
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

#include "Argon.hh"

namespace LArGeant
{
    class ArgonSphere : public G4VUserDetectorConstruction
    {
    public:
        ArgonSphere(Argon argon, G4double radius);
        ~ArgonSphere();

        virtual G4VPhysicalVolume* Construct();

        std::shared_ptr<G4LogicalVolume> GetLogicalWorldVolume()    { return mLogicalWorld; }
        std::shared_ptr<G4LogicalVolume> GetLogicalEnvVolume()      { return mLogicalEnv; }
        std::shared_ptr<G4LogicalVolume> GetLogicalSphereVolume()   { return mLogicalSphere; }

        std::shared_ptr<G4VPhysicalVolume> GetPhysicalWorldVolume()  { return mPhysicalWorld; }
        std::shared_ptr<G4VPhysicalVolume> GetPhysicalEnvVolume()    { return mPhysicalEnv; }
        std::shared_ptr<G4VPhysicalVolume> GetPhysicalSphereVolume() { return mPhysicalSphere; }

        std::shared_ptr<G4LogicalVolume> GetScoringVolume() const { return mScoringVolume; }

    private:
        // parameters for the geometry of the sphere
        G4double mRadius; 
        G4double mWorldX, mWorldY, mWorldZ;
        G4double mEnvX, mEnvY, mEnvZ;
        // G4 volumes mor the sphere
        std::shared_ptr<G4Box>  mSolidWorld, mSolidEnv;
        /// sphere
        std::shared_ptr<G4Sphere> mSolidSphere;                 
        /// logical volume mor the sphere
        std::shared_ptr<G4LogicalVolume> mLogicalWorld, mLogicalEnv, mLogicalSphere;   
        /// physical volume mor the sphere
        std::shared_ptr<G4VPhysicalVolume> mPhysicalWorld = {nullptr};
        std::shared_ptr<G4VPhysicalVolume> mPhysicalEnv = {nullptr};
        std::shared_ptr<G4VPhysicalVolume> mPhysicalSphere = {nullptr};    

        Argon mArgon;
        std::shared_ptr<G4Material> mWorldMat = {nullptr};
        std::shared_ptr<G4Material> mEnvMat = {nullptr};

        void DefineMaterials();

        G4GenericMessenger *fMessenger;

    protected:
        std::shared_ptr<G4LogicalVolume>  mScoringVolume;
    };
}