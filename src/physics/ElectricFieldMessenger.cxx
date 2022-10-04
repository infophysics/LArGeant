/**
 * @file ElectricFieldMessenger.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @date 2022-10-04
 */
#include "ElectricFieldMessenger.hh"
#include "ElectricField.hh"

namespace LArGeant
{
    ElectricFieldMessenger::ElectricFieldMessenger(ElectricField* fieldSetup)
    : G4UImessenger()
    , mElectricField(fieldSetup)
    , mFieldDir(0)
    , mStepperCmd(0)
    , mElFieldZCmd(0)
    , mElFieldCmd(0)
    , mMinStepCmd(0)
    , mUpdateCmd(0)
    {
        mFieldDir = new G4UIdirectory("/field/");
        mFieldDir->SetGuidance("Electric field tracking control.");

        mStepperCmd = new G4UIcmdWithAnInteger("/field/setStepperType",this);
        mStepperCmd->SetGuidance("Select stepper type for electric field");
        mStepperCmd->SetParameterName("choice",true);
        mStepperCmd->SetDefaultValue(4);
        mStepperCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

        mUpdateCmd = new G4UIcmdWithoutParameter("/field/update",this);
        mUpdateCmd->SetGuidance("Update calorimeter geometry.");
        mUpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
        mUpdateCmd->SetGuidance("if you changed geometrical value(s).");
        mUpdateCmd->AvailableForStates(G4State_Idle);

        mElFieldZCmd = new G4UIcmdWithADoubleAndUnit("/field/setFieldZ",this);
        mElFieldZCmd->SetGuidance("Define uniform Electric field.");
        mElFieldZCmd->SetGuidance("Electric field will be in Z direction.");
        mElFieldZCmd->SetGuidance("Value of Electric field has to be given in volt/m");
        mElFieldZCmd->SetParameterName("Ez",false,false);
        mElFieldZCmd->SetDefaultUnit("megavolt/m");
        mElFieldZCmd->AvailableForStates(G4State_Idle);
        
        mElFieldCmd = new G4UIcmdWith3VectorAndUnit("/field/setField",this);
        mElFieldCmd->SetGuidance("Define uniform Electric field.");
        mElFieldCmd->SetGuidance("Value of Electric field has to be given in volt/m");
        mElFieldCmd->SetParameterName("Ex","Ey","Ez",false,false);
        mElFieldCmd->SetDefaultUnit("megavolt/m");
        mElFieldCmd->AvailableForStates(G4State_Idle);
        
        mMinStepCmd = new G4UIcmdWithADoubleAndUnit("/field/setMinStep",this);
        mMinStepCmd->SetGuidance("Define minimal step");
        mMinStepCmd->SetParameterName("min step",false,false);
        mMinStepCmd->SetDefaultUnit("mm");
        mMinStepCmd->AvailableForStates(G4State_Idle);
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    ElectricFieldMessenger::~ElectricFieldMessenger()
    {
        delete mStepperCmd;
        delete mElFieldZCmd;
        delete mElFieldCmd;
        delete mMinStepCmd;
        delete mFieldDir;
        delete mUpdateCmd;
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    void ElectricFieldMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
    {
        if( command == mStepperCmd )
            mElectricField->SetStepperType(mStepperCmd->GetNewIntValue(newValue));
        if( command == mUpdateCmd )
            mElectricField->UpdateIntegrator();
        if( command == mElFieldZCmd )
            mElectricField->SetFieldZValue(mElFieldZCmd->GetNewDoubleValue(newValue));
        if( command == mElFieldCmd )
            mElectricField->SetFieldValue(mElFieldCmd->GetNew3VectorValue(newValue));
        if( command == mMinStepCmd )
            mElectricField->SetMinStep(mMinStepCmd->GetNewDoubleValue(newValue));
    }
}