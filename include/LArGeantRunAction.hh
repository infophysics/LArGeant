/**
 * @file LArGeantRunAction.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4AnalysisManager.hh"

namespace largeant
{
    class LArGeantRunAction : public G4UserRunAction
    {
    public:
        LArGeantRunAction();
        ~LArGeantRunAction();
        
        virtual void BeginOfRunAction(const G4Run*);
        virtual void EndOfRunAction(const G4Run*);
    };
}