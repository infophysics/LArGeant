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
        LArGeantPrimaryGeneratorAction& primaryGeneratorAction,
        G4double efield
    )
    {
        fGenerator = std::make_shared<LArGeantPrimaryGeneratorAction>(primaryGeneratorAction);
        fRunAction = std::make_shared<LArGeantRunAction>(efield);
        fEventAction = std::make_shared<LArGeantEventAction>(fRunAction);
        fSteppingAction = std::make_shared<LArGeantSteppingAction>(fEventAction);
        fStackingAction = std::make_shared<LArGeantStackingAction>();
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
        SetUserAction(fStackingAction.get());
    }

    void LArGeantActionInitialization::BuildForMaster() const
    {
        SetUserAction(fRunAction.get());
    }
}