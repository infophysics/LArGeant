/**
 * @file LArGeantArgonCubeDetector.hh
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

#include "LArGeantArgonCube.hh"
#include "LArGeantArgonCubeSiPMSensitiveDetector.hh"

namespace largeant
{
    class LArGeantArgonCubeDetector : public G4VUserDetectorConstruction
    {
    public:
        LArGeantArgonCubeDetector(
            LArGeantArgon argon,
            G4double worldX, 
            G4double worldY, 
            G4double worldZ,
            G4int bottomNumberX, 
            G4int bottomNumberZ,
            G4double bottomThicknessY
        );
        ~LArGeantArgonCubeDetector();

        virtual G4VPhysicalVolume* Construct();

        std::shared_ptr<G4LogicalVolume> getLogicalWorldVolume()    { return fLogicalWorld; }
        std::shared_ptr<G4LogicalVolume> getLogicalCubeVolume()     { return fLogicalCube; }

        std::shared_ptr<G4VPhysicalVolume> getPhysicalWorldVolume() { return fPhysicalWorld; }
        std::shared_ptr<G4VPhysicalVolume> getPhysicalCubeVolume()  { return fPhysicalCube; }

        std::shared_ptr<G4LogicalVolume> GetScoringVolume() const   { return fScoringVolume; }

        G4double getWorldX() const { return fWorldX; }
        G4double getWorldY() const { return fWorldY; }
        G4double getWorldZ() const { return fWorldZ; }

    private:
        // parameters for the geometry of the Cube
        G4double fWorldX, fWorldY, fWorldZ;
        G4double fEnvX, fEnvY, fEnvZ;
        G4int fBottomNumberX = {10};
        G4int fBottomNumberZ = {10};
        G4double fBottomThicknessY;

        // G4 volumes for the Cube
        std::shared_ptr<G4Box> fSolidWorld = {nullptr};
        /// Cube
        std::shared_ptr<G4Box> fSolidCube = {nullptr};

        /// detector
        std::shared_ptr<G4Box> fBottomSiPMSolidDetector = {nullptr};                 
        
        /// logical volume for the Cube
        std::shared_ptr<G4LogicalVolume> fLogicalWorld = {nullptr};
        std::shared_ptr<G4LogicalVolume> fLogicalCube = {nullptr};
        std::shared_ptr<G4LogicalVolume> fBottomSiPMLogicalDetector = {nullptr};   
        
        /// physical volume for the Cube
        std::shared_ptr<G4VPhysicalVolume> fPhysicalWorld = {nullptr};
        std::shared_ptr<G4VPhysicalVolume> fPhysicalCube = {nullptr};   
        std::shared_ptr<G4VPhysicalVolume> fPhysicalDetector = {nullptr}; 

        std::vector<std::shared_ptr<G4VPhysicalVolume>> fBottomSiPMPhysicalDetector;
        std::vector<std::shared_ptr<G4VPhysicalVolume>> fFrontSiPMPhysicalDetector;
        std::vector<std::shared_ptr<G4VPhysicalVolume>> fLeftSideSiPMPhysicalDetector;
        std::vector<std::shared_ptr<G4VPhysicalVolume>> fRightSideSiPMPhysicalDetector;

        LArGeantArgon fArgon;
        std::shared_ptr<G4Material> fWorldMat = {nullptr};
        std::shared_ptr<G4Material> fEnvMat = {nullptr};

        void DefineMaterials();
        
        G4GenericMessenger *fMessenger;

        /// sensitive detector
        std::shared_ptr<LArGeantArgonCubeSiPMSensitiveDetector> fBottomSiPMSensitiveDetector;
        virtual void ConstructSDandField();

    protected:
        std::shared_ptr<G4LogicalVolume> fScoringVolume;
    };
}