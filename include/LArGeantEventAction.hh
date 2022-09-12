/**
 * @file LArGeantEventAction.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include <memory>
#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"

#include "LArGeantRunAction.hh"

namespace largeant
{
    class LArGeantEventAction : public G4UserEventAction
    {
    public:
        LArGeantEventAction(std::shared_ptr<LArGeantRunAction> runAction);
        ~LArGeantEventAction();
        
        virtual void BeginOfEventAction(const G4Event*);
        virtual void EndOfEventAction(const G4Event*);

        void AddEdep(G4double edep) { fEdep += edep; }

    private:
        G4double fEdep;
    };
}