/**
 * @file ElectricFieldMessenger.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @date 2022-10-04
 */
#pragma once

#include "G4UImessenger.hh"

#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

namespace LArGeant
{
    class ElectricField;
    class ElectricFieldMessenger: public G4UImessenger
    {
    public:
        ElectricFieldMessenger(ElectricField* );
        virtual ~ElectricFieldMessenger();

        virtual void SetNewValue(G4UIcommand*, G4String);
    
    private:

        ElectricField*     mElectricField;

        G4UIdirectory*             mFieldDir;
        G4UIcmdWithAnInteger*      mStepperCmd;
        G4UIcmdWithADoubleAndUnit* mElFieldZCmd;
        G4UIcmdWith3VectorAndUnit* mElFieldCmd;
        G4UIcmdWithADoubleAndUnit* mMinStepCmd;
        G4UIcmdWithoutParameter*   mUpdateCmd;
    };
}