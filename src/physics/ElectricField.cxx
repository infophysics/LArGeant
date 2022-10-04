/**
 * @file ElectricField.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @date 2022-10-04
 */
#include "ElectricField.hh"

namespace LArGeant
{
    void ElectricField::SetStepperType(G4int i)    
    { 
        mStepperType = i; 
        CreateStepper(); 
    }
    void ElectricField::SetMinStep(G4double s)     
    { 
        mMinStep = s; 
    }

    ElectricField::ElectricField()
    : mMinStep(0.010*mm)
    , mFieldManager(0)
    , mLocalFieldManager(0)
    , mChordFinder(0)
    , mLocalChordFinder(0)
    , mEquation(0)
    , mLocalEquation(0)
    , mEMfield(0)
    , mLocalEMfield(0)
    , mElFieldValue()
    , mLocalElFieldValue()
    , mStepper(0)
    , mLocalStepper(0)
    , mIntgrDriver(0)
    , mLocalIntgrDriver(0)
    , mStepperType(4)
    , mFieldMessenger(nullptr)   
    {
        mEMfield = new G4UniformElectricField(G4ThreeVector(0.0,500*volt/cm,0.0));
        mLocalEMfield = new G4UniformElectricField(G4ThreeVector(0.0,500*volt/cm,0.0));

        mEquation = new G4EqMagElectricField(mEMfield);
        mLocalEquation = new G4EqMagElectricField(mLocalEMfield);

        mFieldManager = GetGlobalFieldManager();
        mLocalFieldManager = new G4FieldManager();

        UpdateIntegrator();
        mFieldMessenger = new ElectricFieldMessenger(this);
    }

    ElectricField::ElectricField(G4ThreeVector fieldVector)
    : mMinStep(0.010*mm)
    , mFieldManager(0)
    , mLocalFieldManager(0)
    , mChordFinder(0)
    , mLocalChordFinder(0)
    , mEquation(0)
    , mLocalEquation(0)
    , mEMfield(0)
    , mLocalEMfield(0)
    , mElFieldValue()
    , mLocalElFieldValue()
    , mStepper(0)
    , mLocalStepper(0)
    , mIntgrDriver(0)
    , mLocalIntgrDriver(0)
    , mStepperType(4)
    , mFieldMessenger(nullptr)
    {
        mEMfield = new G4UniformElectricField(fieldVector);
        mLocalEMfield = new G4UniformElectricField(fieldVector);

        mEquation = new G4EqMagElectricField(mEMfield);
        mLocalEquation = new G4EqMagElectricField(mLocalEMfield);

        mFieldManager = GetGlobalFieldManager();
        mLocalFieldManager = new G4FieldManager();

        UpdateIntegrator();
        mFieldMessenger = new ElectricFieldMessenger(this);
    }

    ElectricField::~ElectricField()
    {
        delete mFieldMessenger; mFieldMessenger = nullptr;
        delete mChordFinder;    mChordFinder= nullptr;
        delete mStepper;        mStepper = nullptr;
        delete mEquation;       mEquation = nullptr;
        delete mEMfield;        mEMfield = nullptr;
    }

    void ElectricField::UpdateIntegrator()
    {
        // Register this field to 'global' Field Manager and
        // Create Stepper and Chord Finder with predefined type, minstep (resp.)

        // It must be possible to call 'again' after an alternative stepper
        //   has been chosen, or other changes have been made
        assert(mEquation!=nullptr);
        assert(mLocalEquation!=nullptr);

        G4cout<< " ElectricField: The minimal step is equal to "
                << mMinStep/mm << " mm" << G4endl;

        if (mChordFinder) {
            delete mChordFinder;
            mChordFinder= nullptr;
            // The chord-finder's destructor deletes the driver
            mIntgrDriver= nullptr;
        }
        if (mLocalChordFinder) {
            delete mLocalChordFinder;
            mLocalChordFinder= nullptr;
            // The chord-finder's destructor deletes the driver
            mLocalIntgrDriver= nullptr;
        }
        
        // Currently driver does not 'own' stepper      ( 17.05.2017 J.A. )
        //   -- so this stepper is still a valid object after this

        if( mStepper ) {
            delete mStepper;
            mStepper = nullptr;
        }
        if( mLocalStepper ) {
            delete mLocalStepper;
            mLocalStepper = nullptr;
        }
        
        // Create the new objects, in turn for all relevant classes
        //  -- Careful to call this after all old objects are destroyed, and
        //      pointers nullified.
        CreateStepper();  // Note that this method deleted the existing Stepper!
        // G4cout << "ElectricField::UpdateIntegrator> "
        //        << "New value of stepper ptr= " << mStepper << G4endl;
        assert(mStepper != nullptr);
        assert(mLocalStepper != nullptr);

        if( mStepper ) {
            mIntgrDriver = new G4MagInt_Driver(mMinStep,
                                                mStepper,
                                                mStepper->GetNumberOfVariables());
            if( mIntgrDriver ){ 
                mChordFinder = new G4ChordFinder(mIntgrDriver);
            }
        }
        if( mLocalStepper ) {
            mLocalIntgrDriver = new G4MagInt_Driver(mMinStep,
                                                mLocalStepper,
                                                mLocalStepper->GetNumberOfVariables());
            if( mLocalIntgrDriver ){ 
                mLocalChordFinder = new G4ChordFinder(mLocalIntgrDriver);
            }
        }
    
        mFieldManager->SetChordFinder(mChordFinder);
        mFieldManager->SetDetectorField(mEMfield);

        mLocalFieldManager->SetChordFinder(mLocalChordFinder);
        mLocalFieldManager->SetDetectorField(mLocalEMfield);
    }

    void ElectricField::CreateStepper()
    {
        // Deletes the existing stepper
        //   and creates a new stepper object of the chosen stepper type

        const G4int nvar = 8;

        auto oldStepper= mStepper;
        auto oldLocalStepper= mLocalStepper;
        switch ( mStepperType )
        {
            case 0:
            mStepper = new G4ExplicitEuler( mEquation, nvar );
            mLocalStepper = new G4ExplicitEuler( mLocalEquation, nvar );
            G4cout<<"G4ExplicitEuler is calledS"<<G4endl;
            break;
            case 1:
            mStepper = new G4ImplicitEuler( mEquation, nvar );
            mLocalStepper = new G4ImplicitEuler( mLocalEquation, nvar );
            G4cout<<"G4ImplicitEuler is called"<<G4endl;
            break;
            case 2:
            mStepper = new G4SimpleRunge( mEquation, nvar );
            mLocalStepper = new G4SimpleRunge( mLocalEquation, nvar );
            G4cout<<"G4SimpleRunge is called"<<G4endl;
            break;
            case 3:
            mStepper = new G4SimpleHeum( mEquation, nvar );
            mLocalStepper = new G4SimpleHeum( mLocalEquation, nvar );
            G4cout<<"G4SimpleHeum is called"<<G4endl;
            break;
            case 4:
            mStepper = new G4ClassicalRK4( mEquation, nvar );
            mLocalStepper = new G4ClassicalRK4( mLocalEquation, nvar );
            G4cout<<"G4ClassicalRK4 is called"<<G4endl;
            break;
            case 5:
            mStepper = new G4CashKarpRKF45( mEquation, nvar );
            mLocalStepper = new G4CashKarpRKF45( mLocalEquation, nvar );
            G4cout<<"G4CashKarpRKF45 is called"<<G4endl;
            break;
            case 6:
            mStepper = 0; // new G4RKG3_Stepper( mEquation, nvar );
            mLocalStepper = 0; // new G4RKG3_Stepper( mEquation, nvar );
            G4cout<<"G4RKG3_Stepper is not currently working for Electric Field"
                    <<G4endl;
            break;
            case 7:
            mStepper = 0; // new G4HelixExplicitEuler( mEquation );
            mLocalStepper = 0; // new G4HelixExplicitEuler( mEquation );
            G4cout<<"G4HelixExplicitEuler is not valid for Electric Field"<<G4endl;
            break;
            case 8:
            mStepper = 0; // new G4HelixImplicitEuler( mEquation );
            mLocalStepper = 0; // new G4HelixImplicitEuler( mEquation );
            G4cout<<"G4HelixImplicitEuler is not valid for Electric Field"<<G4endl;
            break;
            case 9:
            mStepper = 0; // new G4HelixSimpleRunge( mEquation );
            mLocalStepper = 0; // new G4HelixSimpleRunge( mEquation );
            G4cout<<"G4HelixSimpleRunge is not valid for Electric Field"<<G4endl;
            break;
            default:  /* mStepper = 0; // Older code */
            mStepper = new G4ClassicalRK4( mEquation, nvar );
            mLocalStepper = new G4ClassicalRK4( mLocalEquation, nvar );
            G4cout<<"G4ClassicalRK4 (default) is called"<<G4endl;
            break;
        }

        delete oldStepper;
        delete oldLocalStepper;
        // Now must make sure it is 'stripped' from the dependent object(s)
        //  ... but the next line does this anyway - by informing
        //      the driver (if it exists) about the new stepper.

        // Always inform the (existing) driver about the new stepper
        if( mIntgrDriver )
            mIntgrDriver->RenewStepperAndAdjust( mStepper );
        if( mLocalIntgrDriver )
            mLocalIntgrDriver->RenewStepperAndAdjust( mLocalStepper );
    }

    void ElectricField::SetFieldZValue(G4double fieldValue)
    {
        // Set the value of the Global Field to fieldValue along Z

        G4ThreeVector fieldVector( 0.0, 0.0, fieldValue );

        SetFieldValue( fieldVector );
    }

    void ElectricField::SetFieldValue(G4ThreeVector fieldVector)
    {
        if (mEMfield) delete mEMfield;

        // Set the value of the Global Field value to fieldVector

        // Find the Field Manager for the global field
        G4FieldManager* fieldMgr= GetGlobalFieldManager();

        if (fieldVector != G4ThreeVector(0.,0.,0.))
        {
            mEMfield = new G4UniformElectricField(fieldVector);
        }
        else
        {
            // If the new field's value is Zero, then it is best to
            //  insure that it is not used for propagation.
            mEMfield = 0;
        }
        fieldMgr->SetDetectorField(mEMfield);
        mEquation->SetFieldObj(mEMfield);  // must now point to the new field
    }

    void ElectricField::SetLocalFieldZValue(G4double fieldValue)
    {
        G4ThreeVector fieldVector( 0.0, 0.0, fieldValue );
        SetLocalFieldValue( fieldVector );
    }

    void ElectricField::SetLocalFieldValue(G4ThreeVector fieldVector)
    {
        if (mLocalEMfield) delete mLocalEMfield;
        if (fieldVector != G4ThreeVector(0.,0.,0.)) {
            mLocalEMfield = new G4UniformElectricField(fieldVector);
        }
        else {
            mLocalEMfield = 0;
        }
        GetLocalFieldManager()->SetDetectorField(mLocalEMfield);
        mLocalEquation->SetFieldObj(mLocalEMfield); 
    }

    G4FieldManager*  ElectricField::GetGlobalFieldManager()
    {
        //  Utility method

        return G4TransportationManager::GetTransportationManager()
                ->GetFieldManager();
    }
}