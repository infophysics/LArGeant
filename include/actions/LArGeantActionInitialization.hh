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
#include "LArGeantStackingAction.hh"

namespace largeant
{
    class LArGeantActionInitialization : public G4VUserActionInitialization
    {
    public:
        LArGeantActionInitialization(
            LArGeantPrimaryGeneratorAction& primaryGeneratorAction,
            G4double efield
        );
        ~LArGeantActionInitialization();

        virtual void Build() const;
        virtual void BuildForMaster() const;

    private:
        std::shared_ptr<LArGeantPrimaryGeneratorAction> fGenerator;
        std::shared_ptr<LArGeantRunAction> fRunAction;
        std::shared_ptr<LArGeantEventAction> fEventAction;
        std::shared_ptr<LArGeantSteppingAction> fSteppingAction;
        std::shared_ptr<LArGeantStackingAction> fStackingAction;
    };
}