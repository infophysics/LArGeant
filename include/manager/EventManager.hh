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
#include <filesystem>
#include <functional>

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
#include "OpticalPhotonData.hh"
#include "ThermalElectronData.hh"
#include "EnergyDeposit.hh"
#include "Hit.hh"
#include "NESTInterface.hh"

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
        inline static thread_local G4int EventID() { return G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID(); }

        // Tuple related functions
        G4String OutputFileName()           { return sOutputFileName; }
        void OutputFileName(G4String name)  { sOutputFileName = name; }
        void OpenOutputFile(G4int RunID);
        void CloseOutputFile(G4int RunID);
        G4int GetIndex(G4String);

        // event max time
        const G4double& EventMaxTime()             { return sEventMaxTime; }
        void EventMaxTime(G4double maxTime) { sEventMaxTime = maxTime; }
        inline static thread_local G4int GetNumberOfParticles()         { return mParticleName.size(); }
        inline static thread_local G4int GetNumberOfSimulatedParticles(){ return mParticleName.size(); }

        //*************************************************************************************************//
        // Options to save various data to root files.
        void SaveParticleMaps(G4bool save)      { sSaveParticleMaps = save; }
        void SavePrimaryInfo(G4bool save)       { sSavePrimaryInfo = save; }
        void SaveParticleInfo(G4bool save)      { sSaveParticleInfo = save; }
        void SaveEnergyDeposits(G4bool save)    { sSaveEnergyDeposits = save; }
        void SaveOpticalPhotons(G4bool save)    { sSaveOpticalPhotons = save; }
        void SaveThermalElectrons(G4bool save)  { sSaveThermalElectrons = save; }
        void SaveNESTResults(G4bool save)       { sSaveNESTResults = save; }
        void SaveHits(G4bool save)              { sSaveHits = save; }

        G4bool SaveParticleMaps()       { return sSaveParticleMaps; }
        G4bool SavePrimaryInfo()        { return sSavePrimaryInfo; }
        G4bool SaveParticleInfo()       { return sSaveParticleInfo; }
        G4bool SaveEnergyDeposits()     { return sSaveEnergyDeposits; }
        G4bool SaveOpticalPhotons()     { return sSaveOpticalPhotons; }
        G4bool SaveThermalElectrons()   { return sSaveThermalElectrons; }
        G4bool SaveNESTResults()        { return sSaveNESTResults; }
        G4bool SaveHits()               { return sSaveHits; }

        void CreateTuples();
        void FillParticleMaps(G4int EventID = -1);
        void FillPrimaryInfo(G4int EventID = -1);
        void FillParticleInfo(G4int EventID = -1);
        void FillEnergyDeposits(G4int EventID = -1);
        void FillOpticalPhotons(G4int EventID = -1);
        void FillThermalElectrons(G4int EventID = -1);
        void FillNESTResults(G4int EventID = -1);
        void FillHits(G4int EventID = -1);
        inline static thread_local void ClearEventData()
        {
            mParticleTrackID.clear();
            mParticleName.clear();
            mParticlePDG.clear();
            mParticleParentTrackID.clear();
            mParticleAncestorTrackID.clear();
            mScintillationAncestorTrackID.clear();
            mScintillationAncestorPDG.clear();
            mPrimaryData.clear();
            mParticles.clear();
            mEnergyDeposits.clear();
            mOpticalPhotonData.clear();
            mThermalElectronData.clear();
            mNESTInterfaceResults.clear();
            mHits.clear();
        }
        //*************************************************************************************************//

        //*************************************************************************************************//
        // Event level maps to keep track of particle ids,
        // parent ids, ancestor ids and pdg codes.
        // Add Particle info
        void AddParticleMapsFromTrack(const G4Track* track);

        inline static thread_local void AddParticleTrackID(G4int track_id, G4int location)
        { mParticleTrackID[track_id] = location; }
        inline static thread_local void AddParticleName(G4int track_id, G4String name)                 
        { mParticleName[track_id] = name; }
        inline static thread_local void AddParticlePDG(G4int track_id, G4int pdg)
        { mParticlePDG[track_id] = pdg; }
        inline static thread_local void AddParticleParentTrackID(G4int track_id, G4int parent_track_id)
        { mParticleParentTrackID[track_id] = parent_track_id; }
        inline static thread_local void AddParticleAncestorTrackID(G4int track_id, G4int ancestor_track_id) 
        { mParticleAncestorTrackID[track_id] = ancestor_track_id; }
        inline static thread_local void AddScintillationAncestorTrackID(G4int track_id, G4int scintillation_track_id)
        { mScintillationAncestorTrackID[track_id] = scintillation_track_id; }
        inline static thread_local void AddScintillationAncestorPDG(G4int track_id, G4int scintillation_pdg)
        { mScintillationAncestorPDG[track_id] = scintillation_pdg; }

        inline static thread_local const G4int& GetParticleTrackID(G4int track_id)          { return mParticleTrackID[track_id]; }
        inline static thread_local const G4String& GetParticleName(G4int track_id)          { return mParticleName[track_id]; }
        inline static thread_local const G4int& GetParticlePDG(G4int track_id)              { return mParticlePDG[track_id]; }
        inline static thread_local const G4int& GetParticleParentTrackID(G4int track_id)    { return mParticleParentTrackID[track_id]; }
        inline static thread_local const G4int& GetParticleAncestorTrackID(G4int track_id)  { return mParticleAncestorTrackID[track_id]; }
        inline static thread_local const G4int& GetScintillationAncestorTrackID(G4int track_id) { return mScintillationAncestorTrackID[track_id]; }
        inline static thread_local const G4int& GetScintillationAncestorPDG(G4int track_id)     { return mScintillationAncestorPDG[track_id]; }

        // Get the entire map object
        inline static thread_local const std::map<G4int, G4int>&    GetParticleTrackIDMap()        { return mParticleTrackID; }               
        inline static thread_local const std::map<G4int, G4String>& GetParticleNameMap()           { return mParticleName; }
        inline static thread_local const std::map<G4int, G4int>&    GetParticlePDGMap()            { return mParticlePDG; }
        inline static thread_local const std::map<G4int, G4int>&    GetParticleParentTrackIDMap()  { return mParticleParentTrackID; }
        inline static thread_local const std::map<G4int, G4int>&    GetParticleAncestorTrackIDMap(){ return mParticleAncestorTrackID; }
        inline static thread_local const std::map<G4int, G4int>&    GetScintillationAncestorTrackIDMap() { return mScintillationAncestorTrackID; }
        inline static thread_local const std::map<G4int, G4int>&    GetScintillationAncestorPDGMap()     { return mScintillationAncestorPDG; }
        //*************************************************************************************************//

        //*************************************************************************************************//
        // Event level primary info to keep track of
        void AddPrimaryInfoFromTrackBegin(const G4Track* track);
        void AddPrimaryInfoFromTrackEnd(const G4Track* track);
        inline static thread_local void AddPrimaryData(PrimaryData primary) { mPrimaryData.emplace_back(primary); }
        inline static thread_local G4int GetNumberOfPrimaries()             { return mPrimaryData.size(); }
        inline static thread_local const std::vector<PrimaryData>& GetPrimaries()  { return mPrimaryData; }
        
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
        // Generate primaries
        std::vector<PrimaryGeneration> GeneratePrimaryList();
        //*************************************************************************************************//

        //*************************************************************************************************//
        // Particle level info to keep track of
        void AddParticleInfoFromTrackBegin(const G4Track* track);
        void AddParticleInfoFromTrackEnd(const G4Track* track);
        void AddParticleInfoFromStep(const G4Step* step);
        inline static thread_local const std::vector<Particle>& GetParticles()     { return mParticles; }
        //*************************************************************************************************//

        //*************************************************************************************************//
        // Energy deposit level info to keep track of
        void AddEnergyDepositInfoFromStep(const G4Step* step);
        inline static thread_local const std::vector<EnergyDeposit>& GetEnergyDeposits() { return mEnergyDeposits; }
        //*************************************************************************************************//

        //*************************************************************************************************//
        // Optical photon level info to keep track of
        void AddOpticalPhotonInfoFromTrackEnd(const G4Track* track);
        inline static thread_local const std::vector<OpticalPhotonData>& GetOpticalPhotonData() { return mOpticalPhotonData; }
        //*************************************************************************************************//

        //*************************************************************************************************//
        // Thermal electron level info to keep track of
        void AddThermalElectronInfoFromTrackEnd(const G4Track* track);
        inline static thread_local const std::vector<ThermalElectronData>& GetThermalElectronData() { return mThermalElectronData; }
        //*************************************************************************************************//

        //*************************************************************************************************//
        // NEST level info to keep track of
        void AddNESTResultFromStep(NESTInterfaceResult result);
        inline static thread_local const std::vector<NESTInterfaceResult>& GetNESTInterfaceResults() { return mNESTInterfaceResults; }
        //*************************************************************************************************//

        //*************************************************************************************************//
        // Hit level info to keep track of
        void AddHitInfoFromStep(G4Step* step, G4TouchableHistory* history);
        inline static thread_local const std::vector<Hit>& GetHits() { return mHits; }
        //*************************************************************************************************//

        //*************************************************************************************************//
        // Scintillation stacking physics
        const G4String& NESTBasis()            { return sNESTBasis; }
        const G4bool& TrackOpticalPhotons()    { return sTrackOpticalPhotons; }
        const G4bool& TrackThermalElectrons()  { return sTrackThermalElectrons; }

        void NESTBasis(G4String basis)          { sNESTBasis = basis; }
        void TrackOpticalPhotons(G4bool track)  { sTrackOpticalPhotons = track; }
        void TrackThermalElectrons(G4bool track){ sTrackThermalElectrons = track; }
        //*************************************************************************************************//

        //*************************************************************************************************//
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
        //*************************************************************************************************//

        //*************************************************************************************************//
        // Analysis related functions
        void EvaluateEvent();
        void AddAnalysisFunction(std::function<void()> func) { mAnalysisFunctions.emplace_back(func); }
        //*************************************************************************************************//

#ifdef LARGEANT_PROFILING
        std::map<G4String, Profile> GetFunctionProfiles()     { return sFunctionProfiles; }
        inline void EndFunctionProfile(G4String func)   { 
            sFunctionProfiles[func].calls += 1; 
            sFunctionProfiles[func].time += (GetTimeInMilliseconds() - sProfilingTime.back()); 
            sProfilingTime.pop_back();
        }
        inline void StartFunctionProfile() {
            sProfilingTime.emplace_back(GetTimeInMilliseconds());
        }
        inline void ResetProfiling() {
            sProfilingTime.clear();
            sFunctionProfiles.clear();
        }
#else 
        inline void EndFunctionProfile(G4String func){}
        inline void StartFunctionProfile(){}
        inline void ResetProfiling(){}
#endif

    private:
        static std::shared_ptr<EventManager> sInstance;
        static std::mutex sMutex;

        inline static std::shared_ptr<PhysicsList> sPhysicsList = {nullptr};
        inline static G4double sEventMaxTime = 2.e19 * ns;

        inline static std::map<G4int, G4int> sComponentCopyNumber;
        inline static std::vector<std::shared_ptr<DetectorComponent>> sDetectorComponents = {nullptr};

        inline static G4String sOutputFileName = "default";
        inline static G4int sCurrentTupleIndex = 0;

        // Options to save various data to root files.
        inline static std::vector<Tuple> sTuples;
        inline static G4bool sSaveParticleMaps = true;
        inline static G4bool sSavePrimaryInfo = true;
        inline static G4bool sSaveParticleInfo = true;
        inline static G4bool sSaveEnergyDeposits = true;
        inline static G4bool sSaveOpticalPhotons = true;
        inline static G4bool sSaveThermalElectrons = true;
        inline static G4bool sSaveNESTResults = true;
        inline static G4bool sSaveHits = true;

        // Scintillation stacking physics
        inline static G4String sNESTBasis = "local_energy";
        inline static G4bool sTrackOpticalPhotons = true;
        inline static G4bool sTrackThermalElectrons = false;

        // Event level maps to keep track of particle ids,
        // parent ids, ancestor ids and pdg codes.
        inline static thread_local std::map<G4int, G4int>      mParticleTrackID;
        inline static thread_local std::map<G4int, G4String>   mParticleName;
        inline static thread_local std::map<G4int, G4int>      mParticlePDG;
        inline static thread_local std::map<G4int, G4int>      mParticleParentTrackID;
        inline static thread_local std::map<G4int, G4int>      mParticleAncestorTrackID;
        inline static thread_local std::map<G4int, G4int>      mScintillationAncestorTrackID;
        inline static thread_local std::map<G4int, G4int>      mScintillationAncestorPDG;

        inline static thread_local std::vector<PrimaryData>    mPrimaryData;
        inline static thread_local std::vector<Particle>       mParticles;
        inline static thread_local std::vector<EnergyDeposit>  mEnergyDeposits;
        inline static thread_local std::vector<OpticalPhotonData>   mOpticalPhotonData;
        inline static thread_local std::vector<ThermalElectronData> mThermalElectronData;
        inline static thread_local std::vector<NESTInterfaceResult> mNESTInterfaceResults;
        inline static thread_local std::vector<Hit> mHits;

        // Analysis functions
        std::vector<std::function<void()>> mAnalysisFunctions;

#ifdef LARGEANT_PROFILING
        inline static thread_local std::map<G4String, Profile> sFunctionProfiles = {};
        inline static thread_local std::vector<G4int> sProfilingTime = {};
#endif
        
    };
}