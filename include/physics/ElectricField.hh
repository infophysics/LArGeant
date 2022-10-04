/**
 * @file ElectricField.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @date 2022-10-04
 */
#pragma once

#include "G4ElectricField.hh"
#include "G4UniformElectricField.hh"
#include "G4UniformMagField.hh"
#include "G4MagneticField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4EquationOfMotion.hh"
#include "G4EqMagElectricField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"

#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "ElectricFieldMessenger.hh"

namespace LArGeant
{
    class ElectricField
    {
    public:
        ElectricField(G4ThreeVector);  
        ElectricField();               
        virtual ~ElectricField();

        void SetStepperType(G4int i);
        void SetMinStep(G4double s);
        void SetFieldValue(G4ThreeVector fieldVector);
        void SetFieldZValue(G4double fieldValue);

        void SetLocalFieldValue(G4ThreeVector fieldVector);
        void SetLocalFieldZValue(G4double fieldValue);

        G4ThreeVector GetConstantFieldValue();
        G4ThreeVector GetLocalFieldValue();

        void UpdateIntegrator();

        G4FieldManager* GetLocalFieldManager() { return mLocalFieldManager;}
    
    protected:
        G4FieldManager*         GetGlobalFieldManager();
        void CreateStepper();

    private:
        G4double                mMinStep;
        G4bool                  mVerbose;
        G4FieldManager*         mFieldManager;
        G4FieldManager*         mLocalFieldManager;
        G4ChordFinder*          mChordFinder;
        G4ChordFinder*          mLocalChordFinder;
        G4EqMagElectricField*   mEquation;
        G4EqMagElectricField*   mLocalEquation;

        G4ElectricField*        mEMfield;
        G4ElectricField*        mLocalEMfield;

        G4ThreeVector           mElFieldValue;
        G4ThreeVector           mLocalElFieldValue;

        G4MagIntegratorStepper* mStepper;
        G4MagIntegratorStepper* mLocalStepper;
        G4MagInt_Driver*        mIntgrDriver;
        G4MagInt_Driver*        mLocalIntgrDriver;
        G4int                   mStepperType;

        ElectricFieldMessenger*      mFieldMessenger;

    };

}