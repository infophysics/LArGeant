/**
 * @file LArGeantEventAction.hh
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

#include "LArGeantRunAction.hh"

namespace largeant
{
    class LArGeantEventAction : public G4UserEventAction
    {
    public:
        LArGeantEventAction(std::shared_ptr<LArGeantRunAction> RunAction);
        ~LArGeantEventAction();
        
        virtual void BeginOfEventAction(const G4Event*);
        virtual void EndOfEventAction(const G4Event*);

        void SetParticleName(G4int track_id, G4String name)                 
        { mParticleName[track_id] = name; }
        void SetParticleParentName(G4int track_id, G4String name)           
        { mParticleParentName[track_id] = name; }
        void SetParticleParentTrackID(G4int track_id, G4int parent_track_id)
        { mParticleParentTrackID[track_id] = parent_track_id; }
        void SetParticleAncestorName(G4int track_id, G4String name)         
        { mParticleAncestorName[track_id] = name; }
        void SetParticleAncestorTrackID(G4int track_id, G4int ancestor_track_id) 
        { mParticleAncestorTrackID[track_id] = ancestor_track_id; }

        G4String GetParticleName(G4int track_id)        { return mParticleName[track_id]; }
        G4String GetParticleParentName(G4int track_id)  { return mParticleParentName[track_id]; }
        G4int GetParticleParentTrackID(G4int track_id)  { return mParticleParentTrackID[track_id]; }
        G4String GetParticleAncestorName(G4int track_id){ return mParticleAncestorName[track_id]; }
        G4int GetParticleAncestorTrackID(G4int track_id){ return mParticleAncestorTrackID[track_id]; }

    private:
        std::shared_ptr<LArGeantRunAction> mRunAction;

        // various particle maps
        std::map<G4int, G4String>   mParticleName;
        std::map<G4int, G4String>   mParticleParentName;
        std::map<G4int, G4int>      mParticleParentTrackID;
        std::map<G4int, G4String>   mParticleAncestorName;
        std::map<G4int, G4int>      mParticleAncestorTrackID;
    };
}