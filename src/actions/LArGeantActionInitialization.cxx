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
    LArGeantActionInitialization::LArGeantActionInitialization(
        LArGeantPrimaryGeneratorAction* PrimaryGeneratorAction
    )
    {
        mGenerator = std::make_shared<LArGeantPrimaryGeneratorAction>(*PrimaryGeneratorAction);
    }

    LArGeantActionInitialization::~LArGeantActionInitialization()
    {
    }

    void LArGeantActionInitialization::Build() const
    {
        SetUserAction(mGenerator.get());
        
        auto RunAction = std::make_shared<LArGeantRunAction>();
        SetUserAction(RunAction.get());

        auto EventAction = std::make_shared<LArGeantEventAction>(RunAction);
        SetUserAction(EventAction.get());

        SetUserAction(new LArGeantSteppingAction(EventAction));
        SetUserAction(new LArGeantStackingAction);
    }

    void LArGeantActionInitialization::BuildForMaster() const
    {
        SetUserAction(new LArGeantRunAction());
    }
}