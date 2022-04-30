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

namespace largeant
{
    class LArGeantArgonCubeDetector : public G4VUserDetectorConstruction
    {
    public:
        LArGeantArgonCubeDetector(LArGeantArgonCube* argonCube);
        ~LArGeantArgonCubeDetector();

        virtual G4VPhysicalVolume* Construct();

    private:
        G4Box* fSolidDetector;
        G4LogicalVolume* fLogicalDetector;

        G4double fWorldX, fWorldY, fWorldZ;
        G4Material *fWorldMat;

        /// number of photo detectors along x and y
        G4int fNumberX, fNumberY;

        G4GenericMessenger *fMessenger;
    };
}