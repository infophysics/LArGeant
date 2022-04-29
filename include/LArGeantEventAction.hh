/**
 * @file LArGeantEventAction.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"

#include "LArGeantRunAction.hh"

namespace largeant
{
    class LArGeantEventAction : public G4UserEventAction
    {
    public:
        LArGeantEventAction(LArGeantRunAction*);
        ~LArGeantEventAction();
        
        virtual void BeginOfEventAction(const G4Event*);
        virtual void EndOfEventAction(const G4Event*);
            
    private:

    };
}