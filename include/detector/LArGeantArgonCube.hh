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
        LArGeantArgonCube(LArGeantArgon argon,
            G4double worldX, G4double worldY, G4double worldZ
        );
        ~LArGeantArgonCube();

        virtual G4VPhysicalVolume* Construct();

        std::shared_ptr<G4LogicalVolume> getLogicalWorldVolume()    { return fLogicalWorld; }
        std::shared_ptr<G4LogicalVolume> getLogicalEnvVolume()      { return fLogicalEnv; }
        std::shared_ptr<G4LogicalVolume> getLogicalCubeVolume()     { return fLogicalCube; }

        std::shared_ptr<G4VPhysicalVolume> getPhysicalWorldVolume() { return fPhysicalWorld; }
        std::shared_ptr<G4VPhysicalVolume> getPhysicalEnvVolume()   { return fPhysicalEnv; }
        std::shared_ptr<G4VPhysicalVolume> getPhysicalCubeVolume()  { return fPhysicalCube; }

        std::shared_ptr<G4LogicalVolume> GetScoringVolume() const   { return fScoringVolume; }

        G4double getWorldX() const { return fWorldX; }
        G4double getWorldY() const { return fWorldY; }
        G4double getWorldZ() const { return fWorldZ; }

    private:
        // parameters for the geometry of the Cube
        G4double fWorldX, fWorldY, fWorldZ;
        G4double fEnvX, fEnvY, fEnvZ;
        // G4 volumes for the Cube
        std::shared_ptr<G4Box> fSolidWorld, fSolidEnv;
        /// Cube
        std::shared_ptr<G4Box> fSolidCube;                 
        /// logical volume for the Cube
        std::shared_ptr<G4LogicalVolume> fLogicalWorld, fLogicalEnv, fLogicalCube;   
        /// physical volume for the Cube
        std::shared_ptr<G4VPhysicalVolume> fPhysicalWorld = {nullptr};
        std::shared_ptr<G4VPhysicalVolume> fPhysicalEnv = {nullptr};
        std::shared_ptr<G4VPhysicalVolume> fPhysicalCube = {nullptr};    

        LArGeantArgon fArgon;
        std::shared_ptr<G4Material> fWorldMat = {nullptr};
        std::shared_ptr<G4Material> fEnvMat = {nullptr};

        void DefineMaterials();
        
        G4GenericMessenger *fMessenger;

    protected:
        std::shared_ptr<G4LogicalVolume> fScoringVolume;
    };
}