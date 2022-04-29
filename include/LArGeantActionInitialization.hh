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
    };
}