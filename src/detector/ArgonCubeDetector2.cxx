// /**
//  * @file ArgonCubeDetector.cxx
//  * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
//  * @brief 
//  * @version 0.1
//  * @date 2022-04-27
//  */
// #include "ArgonCubeDetector.hh"

// namespace LArGeant
// {
//     ArgonCubeDetector::ArgonCubeDetector(
//         Argon argon,
//         G4double worldX, 
//         G4double worldY, 
//         G4double worldZ,
//         G4int bottomNumberX,
//         G4int bottomNumberZ,
//         G4double bottomThicknessY
//     )
//     : G4VUserDetectorConstruction()
//     , mWorldX(worldX), mEnvX(worldX)
//     , mWorldY(worldY), mEnvY(worldY)
//     , mWorldZ(worldZ), mEnvZ(worldZ)
//     , mBottomNumberX(bottomNumberX)
//     , mBottomNumberZ(bottomNumberZ)
//     , mBottomThicknessY(bottomThicknessY)
//     , mArgon(argon)
//     {
//         DefineMaterials();
//         // set up messenger
//         mMessenger = new G4GenericMessenger(this ,"/argon_cube/", "ArgonCube");
//         mMessenger->DeclareProperty("bottom_array/numberX", mBottomNumberX, "Bottom SiPM number along x");
//         mMessenger->DeclareProperty("bottom_array/numberY", mBottomNumberZ, "Bottom SiPM number along z");
//     }

//     ArgonCubeDetector::~ArgonCubeDetector()
//     {
//         G4cout << "Destructing ArgonCube Detector..." << G4endl;
//     }
    
//     void ArgonCubeDetector::DefineMaterials()
//     {
//         G4NistManager *nist = G4NistManager::Instance();
//         mWorldMat.reset(nist->FindOrBuildMaterial("G4_AIR"));
//         mEnvMat.reset(nist->FindOrBuildMaterial("G4_AIR"));
//     }

//     G4VPhysicalVolume *ArgonCubeDetector::Construct()
//     {
//         // create the world volume
//         mSolidWorld.reset(
//             new G4Box(
//                 "SolidWorld", 
//                 mWorldX+mBottomThicknessY, 
//                 mWorldY+mBottomThicknessY, 
//                 mWorldZ+mBottomThicknessY
//             )
//         );
//         mLogicalWorld.reset(
//             new G4LogicalVolume(
//                 mSolidWorld.get(), 
//                 mWorldMat.get(), 
//                 "LogicalWorld"
//             )
//         );
//         mPhysicalWorld.reset(
//             new G4PVPlacement(
//                 0, 
//                 G4ThreeVector(0., 0., 0.),
//                 mLogicalWorld.get(),
//                 "PhysicalWorld",
//                 0, 
//                 false,
//                 0
//             )
//         );
//         // create the argon Cube volume
//         mSolidCube.reset(
//             new G4Box(
//                 "LArCube", 
//                 mWorldX, 
//                 mWorldY, 
//                 mWorldZ
//             )
//         );
//         mLogicalCube.reset(
//             new G4LogicalVolume(
//                 mSolidCube.get(), 
//                 mArgon.GetLAr().get(), 
//                 "LogicalCube"
//             )
//         );
//         mPhysicalCube.reset(
//             new G4PVPlacement(
//                 0, 
//                 G4ThreeVector(0., 0., 0.), 
//                 mLogicalCube.get(), 
//                 "PhysicalCube", 
//                 mLogicalWorld.get(), 
//                 false, 
//                 0, 
//                 true
//             )
//         );

//         // construct bottom sipm array
//         mBottomSiPMSolidDetector.reset(
//             new G4Box(
//                 "SolidDetector",
//                 (mWorldX)/mBottomNumberX,
//                 mBottomThicknessY/2.0,
//                 (mWorldZ)/mBottomNumberZ
//             )
//         ); 
//         mBottomSiPMLogicalDetector.reset(
//             new G4LogicalVolume(
//                 mBottomSiPMSolidDetector.get(),
//                 mWorldMat.get(),
//                 "LogicalDetector"
//             )
//         );      
//         mBottomSiPMPhysicalDetector.clear();
//         for (G4int ii = 0; ii < mBottomNumberX; ii++)
//         {
//             for (G4int jj = 0; jj < mBottomNumberZ; jj++)
//             {
//                 mBottomSiPMPhysicalDetector.emplace_back(
//                     new G4PVPlacement(
//                         0,
//                         G4ThreeVector(
//                             -(mWorldX) + (mWorldX)/mBottomNumberX + ii * (2 * mWorldX)/mBottomNumberX,
//                             -(mWorldY+mBottomThicknessY/2.0),
//                             -(mWorldZ) + (mWorldZ)/mBottomNumberZ + jj * (2 * mWorldZ)/mBottomNumberZ
//                         ),
//                         mBottomSiPMLogicalDetector.get(),
//                         "PhysicalDetector",
//                         mLogicalWorld.get(),
//                         false,
//                         ii + jj * mBottomNumberZ,
//                         true
//                     )
//                 );
//             }
//         }
                
//         //fScoringVolume = mLogicalCube;
//         return mPhysicalWorld.get();
//     }

//     void ArgonCubeDetector::ConstructSDandField()
//     {
//         mBottomSiPMSensitiveDetector = std::make_shared<ArgonCubeSiPMSensitiveDetector>("bottom_sipm");
//         mBottomSiPMLogicalDetector->SetSensitiveDetector(mBottomSiPMSensitiveDetector.get());
//     }
// }