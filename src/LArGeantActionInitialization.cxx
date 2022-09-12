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
    LArGeantActionInitialization::LArGeantActionInitialization(LArGeantPrimaryGeneratorAction& primaryGeneratorAction)
    {
        fGenerator = std::make_shared<LArGeantPrimaryGeneratorAction>(primaryGeneratorAction);
        fRunAction = std::make_shared<LArGeantRunAction>();
        fEventAction = std::make_shared<LArGeantEventAction>(fRunAction);
        fSteppingAction = std::make_shared<LArGeantSteppingAction>(fEventAction);
    }

    LArGeantActionInitialization::~LArGeantActionInitialization()
    {
    }

    void LArGeantActionInitialization::Build() const
    {
        SetUserAction(fGenerator.get());
        SetUserAction(fRunAction.get());
        SetUserAction(fEventAction.get());
        SetUserAction(fSteppingAction.get());
    }

    void LArGeantActionInitialization::BuildForMaster() const
    {
        SetUserAction(fRunAction.get());
    }
}