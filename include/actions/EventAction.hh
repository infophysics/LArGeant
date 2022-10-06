/**
 * @file EventAction.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#pragma once
#include <memory>
#include <map>
#include "globals.hh"
#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventManager.hh"

namespace LArGeant
{
    /**
     * @brief All detector MC must be managed here at the
     * event level!
     * 
     * @details
     * 	Change log:
     * 		2022-09-22 - Initial creation of file.
     */
    class EventAction : public G4UserEventAction
    {
    public:
        EventAction(std::shared_ptr<RunAction> RunAction);
        ~EventAction();
        
        virtual void BeginOfEventAction(const G4Event*);
        virtual void EndOfEventAction(const G4Event*);

        void SetParticleName(G4String name)                 
        { mParticleName.emplace_back(name); }
        void SetParticlePDG(G4int pdg)
        { mParticlePDG.emplace_back(pdg); }
        void SetParticleParentTrackID(G4int parent_track_id)
        { mParticleParentTrackID.emplace_back(parent_track_id); }
        void SetParticleAncestorTrackID(G4int ancestor_track_id) 
        { mParticleAncestorTrackID.emplace_back(ancestor_track_id); }

        G4String GetParticleName(G4int track_id)        { return mParticleName[track_id-1]; }
        G4int GetParticlePDG(G4int track_id)            { return mParticlePDG[track_id-1]; }
        G4int GetParticleParentTrackID(G4int track_id)  { return mParticleParentTrackID[track_id-1]; }
        G4int GetParticleAncestorTrackID(G4int track_id){ return mParticleAncestorTrackID[track_id-1]; }

    private:
        std::shared_ptr<RunAction> mRunAction;

        // various particle info
        std::vector<G4String>   mParticleName;
        std::vector<G4int>      mParticlePDG;
        std::vector<G4int>      mParticleParentTrackID;
        std::vector<G4int>      mParticleAncestorTrackID;
    };
}