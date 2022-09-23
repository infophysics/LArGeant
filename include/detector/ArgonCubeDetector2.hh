// /**
//  * @file ArgonCubeDetector.hh
//  * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
//  * @brief 
//  * @version 0.1
//  * @date 2022-04-27
//  */
// #pragma once
// #include "G4VUserDetectorConstruction.hh"
// #include "G4VPhysicalVolume.hh"
// #include "G4LogicalVolume.hh"
// #include "G4Box.hh"
// #include "G4Tubs.hh"
// #include "G4PVPlacement.hh"
// #include "G4NistManager.hh"
// #include "G4SystemOfUnits.hh"
// #include "G4GenericMessenger.hh"
// #include "G4OpticalSurface.hh"
// #include "G4LogicalBorderSurface.hh"
// #include "G4LogicalSkinSurface.hh"

// #include "Argon.hh"
// #include "ArgonCubeSiPMSensitiveDetector.hh"

// namespace LArGeant
// {
//     class ArgonCubeDetector : public G4VUserDetectorConstruction
//     {
//     public:
//         ArgonCubeDetector(
//             Argon argon,
//             G4double worldX, 
//             G4double worldY, 
//             G4double worldZ,
//             G4int bottomNumberX, 
//             G4int bottomNumberZ,
//             G4double bottomThicknessY
//         );
//         ~ArgonCubeDetector();

//         virtual G4VPhysicalVolume* Construct();

//         std::shared_ptr<G4LogicalVolume> GetLogicalWorldVolume()    { return mLogicalWorld; }
//         std::shared_ptr<G4LogicalVolume> GetLogicalCubeVolume()     { return mLogicalCube; }

//         std::shared_ptr<G4VPhysicalVolume> GetPhysicalWorldVolume() { return mPhysicalWorld; }
//         std::shared_ptr<G4VPhysicalVolume> GetPhysicalCubeVolume()  { return mPhysicalCube; }

//         std::shared_ptr<G4LogicalVolume> GetScoringVolume() const   { return mLogicalCube; }

//         G4double GetWorldX() const { return mWorldX; }
//         G4double GetWorldY() const { return mWorldY; }
//         G4double GetWorldZ() const { return mWorldZ; }

//     private:
//         // parameters mor the geometry of the Cube
//         G4double mWorldX, mWorldY, mWorldZ;
//         G4double mEnvX, mEnvY, mEnvZ;
//         G4int mBottomNumberX = {10};
//         G4int mBottomNumberZ = {10};
//         G4double mBottomThicknessY;

//         // G4 volumes mor the Cube
//         std::shared_ptr<G4Box> mSolidWorld = {nullptr};
//         /// Cube
//         std::shared_ptr<G4Box> mSolidCube = {nullptr};

//         /// detector
//         std::shared_ptr<G4Box> mBottomSiPMSolidDetector = {nullptr};                 
        
//         /// logical volume mor the Cube
//         std::shared_ptr<G4LogicalVolume> mLogicalWorld = {nullptr};
//         std::shared_ptr<G4LogicalVolume> mLogicalCube = {nullptr};
//         std::shared_ptr<G4LogicalVolume> mBottomSiPMLogicalDetector = {nullptr};   
        
//         /// physical volume mor the Cube
//         std::shared_ptr<G4VPhysicalVolume> mPhysicalWorld = {nullptr};
//         std::shared_ptr<G4VPhysicalVolume> mPhysicalCube = {nullptr};   
//         std::shared_ptr<G4VPhysicalVolume> mPhysicalDetector = {nullptr}; 

//         std::vector<std::shared_ptr<G4VPhysicalVolume>> mBottomSiPMPhysicalDetector;
//         std::vector<std::shared_ptr<G4VPhysicalVolume>> mFrontSiPMPhysicalDetector;
//         std::vector<std::shared_ptr<G4VPhysicalVolume>> mLeftSideSiPMPhysicalDetector;
//         std::vector<std::shared_ptr<G4VPhysicalVolume>> mRightSideSiPMPhysicalDetector;

//         Argon mArgon;
//         std::shared_ptr<G4Material> mWorldMat = {nullptr};
//         std::shared_ptr<G4Material> mEnvMat = {nullptr};

//         void DefineMaterials();
        
//         G4GenericMessenger *mMessenger;

//         /// sensitive detector
//         std::shared_ptr<ArgonCubeSiPMSensitiveDetector> mBottomSiPMSensitiveDetector;
//         virtual void ConstructSDandField();

//     protected:
//         //std::shared_ptr<G4LogicalVolume> mScoringVolume;
//     };
// }