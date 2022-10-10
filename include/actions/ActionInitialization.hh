/**
 * @file ActionInitialization.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include "G4VUserActionInitialization.hh"

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "StackingAction.hh"
#include "TrackingAction.hh"

namespace LArGeant
{
    class ActionInitialization : public G4VUserActionInitialization
    {
    public:
        ActionInitialization(
            PrimaryGeneratorAction* PrimaryGeneratorAction
        );
        ~ActionInitialization();

        virtual void Build() const;
        virtual void BuildForMaster() const;

    private:
        std::shared_ptr<PrimaryGeneratorAction> mGenerator;

    };
}