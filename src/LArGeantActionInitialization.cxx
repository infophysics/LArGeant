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
        fRunAction = new LArGeantRunAction();
        fEventAction = new LArGeantEventAction(fRunAction);
    }

    LArGeantActionInitialization::~LArGeantActionInitialization()
    {
    }

    void LArGeantActionInitialization::Build() const
    {
        SetUserAction(fGenerator.get());
        SetUserAction(fRunAction);
        SetUserAction(fEventAction);
    }
}