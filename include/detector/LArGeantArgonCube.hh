/**
 * @file LArGeantArgonCube.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
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
    class LArGeantArgonCube : public G4VUserDetectorConstruction
    {
    public:
        LArGeantArgonCube(LArGeantArgon Argon,
            G4double worldX, G4double worldY, G4double worldZ
        );
        ~LArGeantArgonCube();

        virtual G4VPhysicalVolume* Construct();

        std::shared_ptr<G4LogicalVolume> GetLogicalWorldVolume()    { return mLogicalWorld; }
        std::shared_ptr<G4LogicalVolume> GetLogicalEnvVolume()      { return mLogicalEnv; }
        std::shared_ptr<G4LogicalVolume> GetLogicalCubeVolume()     { return mLogicalCube; }

        std::shared_ptr<G4VPhysicalVolume> GetPhysicalWorldVolume() { return mPhysicalWorld; }
        std::shared_ptr<G4VPhysicalVolume> GetPhysicalEnvVolume()   { return mPhysicalEnv; }
        std::shared_ptr<G4VPhysicalVolume> GetPhysicalCubeVolume()  { return mPhysicalCube; }

        std::shared_ptr<G4LogicalVolume> GetScoringVolume() const   { return mScoringVolume; }

        G4double GetWorldX() const { return mWorldX; }
        G4double GetWorldY() const { return mWorldY; }
        G4double GetWorldZ() const { return mWorldZ; }

    private:
        // parameters mor the geometry of the Cube
        G4double mWorldX, mWorldY, mWorldZ;
        G4double mEnvX, mEnvY, mEnvZ;
        // G4 volumes mor the Cube
        std::shared_ptr<G4Box> mSolidWorld, mSolidEnv;
        /// Cube
        std::shared_ptr<G4Box> mSolidCube;                 
        /// logical volume mor the Cube
        std::shared_ptr<G4LogicalVolume> mLogicalWorld, mLogicalEnv, mLogicalCube;   
        /// physical volume mor the Cube
        std::shared_ptr<G4VPhysicalVolume> mPhysicalWorld = {nullptr};
        std::shared_ptr<G4VPhysicalVolume> mPhysicalEnv = {nullptr};
        std::shared_ptr<G4VPhysicalVolume> mPhysicalCube = {nullptr};    

        LArGeantArgon mArgon;
        std::shared_ptr<G4Material> mWorldMat = {nullptr};
        std::shared_ptr<G4Material> mEnvMat = {nullptr};

        void DefineMaterials();
        
        G4GenericMessenger *mMessenger;

    protected:
        std::shared_ptr<G4LogicalVolume> mScoringVolume;
    };
}