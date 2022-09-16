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
        LArGeantEventAction(std::shared_ptr<LArGeantRunAction> runAction);
        ~LArGeantEventAction();
        
        virtual void BeginOfEventAction(const G4Event*);
        virtual void EndOfEventAction(const G4Event*);

        void AddEdep(G4double edep) { fEdep += edep; }

        G4double GetElectricField() { return fRunAction->GetElectricField(); }

        void SetParticleName(G4int track_id, G4String name)                 
        { fParticleName[track_id] = name; }
        void SetParticleParentName(G4int track_id, G4String name)           
        { fParticleParentName[track_id] = name; }
        void SetParticleParentTrackID(G4int track_id, G4int parent_track_id)
        { fParticleParentTrackID[track_id] = parent_track_id; }
        void SetParticleAncestorName(G4int track_id, G4String name)         
        { fParticleAncestorName[track_id] = name; }
        void SetParticleAncestorTrackID(G4int track_id, G4int ancestor_track_id) 
        { fParticleAncestorTrackID[track_id] = ancestor_track_id; }

        G4String GetParticleName(G4int track_id)        { return fParticleName[track_id]; }
        G4String GetParticleParentName(G4int track_id)  { return fParticleParentName[track_id]; }
        G4int GetParticleParentTrackID(G4int track_id)  { return fParticleParentTrackID[track_id]; }
        G4String GetParticleAncestorName(G4int track_id){ return fParticleAncestorName[track_id]; }
        G4int GetParticleAncestorTrackID(G4int track_id){ return fParticleAncestorTrackID[track_id]; }

    private:
        std::shared_ptr<LArGeantRunAction> fRunAction;
        G4double fEdep;

        // various particle maps
        std::map<G4int, G4String> fParticleName;
        std::map<G4int, G4String> fParticleParentName;
        std::map<G4int, G4int> fParticleParentTrackID;
        std::map<G4int, G4String> fParticleAncestorName;
        std::map<G4int, G4int> fParticleAncestorTrackID;
    };
}