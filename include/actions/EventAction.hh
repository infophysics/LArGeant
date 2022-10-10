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

        void AddParticleName(G4int track_id, G4String name)                 
        { mParticleName[track_id] = name; }
        void AddParticlePDG(G4int track_id, G4int pdg)
        { mParticlePDG[track_id] = pdg; }
        void AddParticleParentTrackID(G4int track_id, G4int parent_track_id)
        { mParticleParentTrackID[track_id] = parent_track_id; }
        void AddParticleAncestorTrackID(G4int track_id, G4int ancestor_track_id) 
        { mParticleAncestorTrackID[track_id] = ancestor_track_id; }

        void AddPrimaryData(PrimaryData primary) { mPrimaryData.emplace_back(primary); }
        PrimaryData& GetPrimaryData(G4int track_id);

        G4String GetParticleName(G4int track_id)      { return mParticleName[track_id]; }
        G4int GetParticlePDG(G4int track_id)          { return mParticlePDG[track_id]; }
        G4int GetParticleParentTrackID(G4int track_id)    { return mParticleParentTrackID[track_id]; }
        G4int GetParticleAncestorTrackID(G4int track_id)  { return mParticleAncestorTrackID[track_id]; }
        G4int GetNumberOfSimulatedParticles() const   { return mParticleName.size(); }

    private:
        std::shared_ptr<RunAction> mRunAction;

        // various particle info
        std::map<G4int, G4String>   mParticleName;
        std::map<G4int, G4int>      mParticlePDG;
        std::map<G4int, G4int>      mParticleParentTrackID;
        std::map<G4int, G4int>      mParticleAncestorTrackID;

        std::vector<PrimaryData> mPrimaryData;
    };
}