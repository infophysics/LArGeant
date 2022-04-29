/**
 * @file LArGeantActionInitialization.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include "G4VUserActionInitialization.hh"

#include "LArGeantPrimaryGeneratorAction.hh"
#include "LArGeantGenerator.hh"
#include "LArGeantRunAction.hh"
#include "LArGeantEventAction.hh"
#include "LArGeantSteppingAction.hh"

namespace largeant
{
    class LArGeantActionInitialization : public G4VUserActionInitialization
    {
    public:
        LArGeantActionInitialization();
        ~LArGeantActionInitialization();

        virtual void Build() const;

    private:
        LArGeantPrimaryGeneratorAction* fGenerator;
        LArGeantRunAction* fRunAction;
        LArGeantEventAction* fEventAction;
        LArGeantSteppingAction* fSteppingAction;
    };
}