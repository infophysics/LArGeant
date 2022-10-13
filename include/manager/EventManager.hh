/**
 * @file EventManager.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @details 
 *  Change log:
 *      2022/09/20 - Initial creation of the file.
 * @date 2022-09-20
 */
#pragma once
#include <memory>
#include <mutex>

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIterminal.hh"
#include "G4UIExecutive.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4VModularPhysicsList.hh"

#include "PhysicsList.hh"
#include "Core.hh"
#include "Detector.hh"
#include "DetectorComponent.hh"
#include "Particle.hh"
#include "EnergyDeposit.hh"

class Detector;
class DetectorComponent;
class PhysicsList;

namespace LArGeant
{
    struct Tuple
    {
        G4String name = "none";
        G4int index = -1;
        Tuple(G4String n, G4int i)
        : name(n), index(i)
        {}
    };
#ifdef LARGEANT_PROFILING
    struct Profile
    {
        G4int calls = 0;
        G4int time = 0;

        Profile(){}
    };
#endif

    /**
     * @brief Class for handling generation of source primaries,
     * input/output, data saving, messenging, etc.
     */
    class EventManager
    {
    public:
        EventManager(EventManager const&) = delete;
        void operator=(EventManager const&) = delete;
		EventManager();
        ~EventManager();

        // setters for various objects
        static void SetPhysicsList(PhysicsList*);
		static void SetParticle(G4String);

        // get the event manager
        static std::shared_ptr<EventManager>& GetEventManager() 
        { 	
            std::lock_guard<std::mutex> lock(sMutex);
			if (sInstance == nullptr) {
				sInstance = std::make_shared<EventManager>();
                sTuples.clear();
                sCurrentTupleIndex = -1;
			}return sInstance; 
		}

        // Tuple related functions
        G4String OutputFileName()       { return sOutputFileName; }
        G4int GetIndex(G4String);

        G4bool SavePrimaryInfo()        { return sSavePrimaryInfo; }
        G4bool SaveParticleInfo()       { return sSaveParticleInfo; }
        G4bool SaveEnergyDepositions()  { return sSaveEnergyDepositions; }
        
        G4bool SaveOpticalPhotons()     { return sSaveOpticalPhotons; }
        G4bool SaveThermalElectrons()   { return sSaveThermalElectrons; }
        G4bool SaveNEST()               { return sSaveNEST; }

        G4bool SaveHits()               { return sSaveHits; }

#ifdef LARGEANT_PROFILING
        std::map<G4String, Profile> GetFunctionProfiles()     { return sFunctionProfiles; }
        void EndFunctionProfile(G4String func)   { 
            sFunctionProfiles[func].calls += 1; 
            sFunctionProfiles[func].time += (GetTimeInMilliseconds() - sProfilingTime.back()); 
            sProfilingTime.pop_back();
        }
        void StartFunctionProfile() {
            sProfilingTime.emplace_back(GetTimeInMilliseconds());
        }
        void ResetProfiling() {
            sProfilingTime.clear();
            sFunctionProfiles.clear();
        }
#else 
        void EndFunctionProfile(G4String func){}
        void StartFunctionProfile(){}
#endif

        void OutputFileName(G4String name)      { sOutputFileName = name; }

        // Options to save various data to root files.
        void SavePrimaryInfo(G4bool save)       { sSavePrimaryInfo = save; }
        void SaveParticleInfo(G4bool save)      { sSaveParticleInfo = save; }
        void SaveEnergyDepositions(G4bool save) { sSaveEnergyDepositions = save; }
        
        void SaveOpticalPhotons(G4bool save)    { sSaveOpticalPhotons = save; }
        void SaveThermalElectrons(G4bool save)  { sSaveThermalElectrons = save; }
        void SaveNEST(G4bool save)              { sSaveNEST = save; }

        void SaveHits(G4bool save)              { sSaveHits = save; }

        // Event level maps to keep track of particle ids,
        // parent ids, ancestor ids and pdg codes.
        inline static thread_local void ClearParticleInfo()
        {
            mParticleName.clear();
            mParticlePDG.clear();
            mParticleParentTrackID.clear();
            mParticleAncestorTrackID.clear();
            mScintillationParentTrackID.clear();
            mPrimaryData.clear();
        }
        inline static thread_local void AddParticleName(G4int track_id, G4String name)                 
        { mParticleName[track_id] = name; }
        inline static thread_local void AddParticlePDG(G4int track_id, G4int pdg)
        { mParticlePDG[track_id] = pdg; }
        inline static thread_local void AddParticleParentTrackID(G4int track_id, G4int parent_track_id)
        { mParticleParentTrackID[track_id] = parent_track_id; }
        inline static thread_local void AddParticleAncestorTrackID(G4int track_id, G4int ancestor_track_id) 
        { mParticleAncestorTrackID[track_id] = ancestor_track_id; }
        inline static thread_local void AddScintillationParentTrackID(G4int track_id, G4int scintillation_track_id)
        { mScintillationParentTrackID[track_id] = scintillation_track_id; }

        inline static thread_local void AddPrimaryData(PrimaryData primary) { mPrimaryData.emplace_back(primary); }
        inline static thread_local G4int GetNumberOfPrimaries()             { return mPrimaryData.size(); }
        inline static thread_local std::vector<PrimaryData> GetPrimaries()  { return mPrimaryData; }
        inline static thread_local PrimaryData& GetPrimaryData(G4int track_id)
        {
            for(size_t ii = 0; ii < mPrimaryData.size(); ii++)
            {
                if(mPrimaryData[ii].track_id == track_id) {
                    return mPrimaryData[ii];
                }
            }
            throw std::invalid_argument(
                "No PrimaryData object matches track_id: " + std::to_string(track_id)
            );
        }

        inline static thread_local G4int GetNumberOfParticles()                      { return mParticleName.size(); }
        inline static thread_local G4String GetParticleName(G4int track_id)          { return mParticleName[track_id]; }
        inline static thread_local G4int GetParticlePDG(G4int track_id)              { return mParticlePDG[track_id]; }
        inline static thread_local G4int GetParticleParentTrackID(G4int track_id)    { return mParticleParentTrackID[track_id]; }
        inline static thread_local G4int GetParticleAncestorTrackID(G4int track_id)  { return mParticleAncestorTrackID[track_id]; }
        inline static thread_local G4int GetScintillationParentTrackID(G4int track_id) { return mScintillationParentTrackID[track_id]; }
        inline static thread_local G4int GetNumberOfSimulatedParticles()               { return mParticleName.size(); }

        // Scintillation stacking physics
        G4String NESTBasis()            { return sNESTBasis; }
        G4bool TrackOpticalPhotons()    { return sTrackOpticalPhotons; }
        G4bool TrackThermalElectrons()  { return sTrackThermalElectrons; }

        // event max time
        G4double EventMaxTime() { return sEventMaxTime; }
        void EventMaxTime(G4double maxTime) { sEventMaxTime = maxTime; }

        void NESTBasis(G4String basis)          { sNESTBasis = basis; }
        void TrackOpticalPhotons(G4bool track)  { sTrackOpticalPhotons = track; }
        void TrackThermalElectrons(G4bool track){ sTrackThermalElectrons = track; }

        // Detector related functions
        static void AddComponent(std::shared_ptr<DetectorComponent> component);
        static std::shared_ptr<DetectorComponent> GetComponent(G4int index)
        {
            return sDetectorComponents[index];
        }
        static G4int GetNumberOfComponents() { return sDetectorComponents.size(); }
        static void SetComponentCopyNumber(G4int copyNumber, G4int component) { sComponentCopyNumber[copyNumber] = component; }
        static std::shared_ptr<DetectorComponent> GetComponentFromCopyNumber(G4int index)
        {
            return sDetectorComponents[sComponentCopyNumber[index]];
        }

        // Generate primaries
        std::vector<PrimaryGeneration> GeneratePrimaryList();

    private:
        static std::shared_ptr<EventManager> sInstance;
        static std::mutex sMutex;

        inline static std::shared_ptr<PhysicsList> sPhysicsList = {nullptr};
        inline static std::vector<std::shared_ptr<DetectorComponent>> sDetectorComponents = {nullptr};

        inline static G4String sOutputFileName = "default";
        inline static G4int sCurrentTupleIndex = 0;

        // Options to save various data to root files.
        inline static std::vector<Tuple> sTuples;
        inline static G4bool sSavePrimaryInfo = true;
        inline static G4bool sSaveParticleInfo = true;
        inline static G4bool sSaveEnergyDepositions = true;
    
        inline static G4bool sSaveOpticalPhotons = true;
        inline static G4bool sSaveThermalElectrons = true;
        inline static G4bool sSaveNEST = true;
        
        inline static G4bool sSaveHits = true;

        // Scintillation stacking physics
        inline static G4String sNESTBasis = "local_energy";
        inline static G4bool sTrackOpticalPhotons = true;
        inline static G4bool sTrackThermalElectrons = false;

        inline static G4double sEventMaxTime = 1.e19 * ns;

        inline static std::map<G4int, G4int> sComponentCopyNumber;

        // Event level maps to keep track of particle ids,
        // parent ids, ancestor ids and pdg codes.
        inline static thread_local std::map<G4int, G4String>   mParticleName;
        inline static thread_local std::map<G4int, G4int>      mParticlePDG;
        inline static thread_local std::map<G4int, G4int>      mParticleParentTrackID;
        inline static thread_local std::map<G4int, G4int>      mParticleAncestorTrackID;
        inline static thread_local std::map<G4int, G4int>      mScintillationParentTrackID;
        inline static thread_local std::vector<PrimaryData>    mPrimaryData;

        inline static thread_local std::vector<Particle>       mParticles;
        inline static thread_local std::vector<EnergyDeposit>  mEnergyDeposits;

#ifdef LARGEANT_PROFILING
        inline static thread_local std::map<G4String, Profile> sFunctionProfiles = {};
        inline static thread_local std::vector<G4int> sProfilingTime = {};
#endif
        
    };
}