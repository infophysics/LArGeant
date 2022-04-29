/**
 * @file LArGeantActionInitialization.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "LArGeantActionInitialization.hh"

namespace largeant
{
    LArGeantActionInitialization::LArGeantActionInitialization()
    {
        fGenerator = new LArGeantPrimaryGeneratorAction(
             50, "neutron"
        );
    }

    LArGeantActionInitialization::~LArGeantActionInitialization()
    {
    }

    void LArGeantActionInitialization::Build() const
    {
        // LArGeantPrimaryGeneratorAction* generator = new LArGeantPrimaryGeneratorAction(
        //     50, "neutron"
        // );
        SetUserAction(fGenerator);

        LArGeantRunAction *runAction = new LArGeantRunAction();
        SetUserAction(runAction);
        
        LArGeantEventAction *eventAction = new LArGeantEventAction(runAction);
        SetUserAction(eventAction);
    }
}