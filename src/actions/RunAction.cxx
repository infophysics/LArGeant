/**
 * @file RunAction.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-27
 */
#include "RunAction.hh"

namespace LArGeant
{
    RunAction::RunAction()
    : G4UserRunAction()
    {
        EventManager::GetEventManager()->CreateTuples();
    }

    RunAction::~RunAction()
    {}

    void RunAction::BeginOfRunAction(const G4Run* run)
    {
#ifdef LARGEANT_PROFILING
        EventManager::GetEventManager()->ResetProfiling();
#endif
        auto Manager = EventManager::GetEventManager();
        auto AnalysisManager = G4AnalysisManager::Instance();
        G4String OutputFileName = Manager->OutputFileName();
        G4String RunID = std::to_string(run->GetRunID());
        G4String OutputFile = OutputFileName + "_" + RunID + ".root";
        G4bool fileopen = AnalysisManager->OpenFile(OutputFile);
        if (!fileopen) {
            G4cout << "File - " + OutputFile 
                << " - not opened!" << G4endl;
        }
        else {
            G4cout << "File - " + OutputFile
                << " - opened successfully." << G4endl;
        }
    }

    void RunAction::EndOfRunAction(const G4Run* run)
    {
        auto AnalysisManager = G4AnalysisManager::Instance();
        AnalysisManager->Write();
        AnalysisManager->CloseFile();

#ifdef LARGEANT_PROFILING
        auto Manager = EventManager::GetEventManager();
        G4String RunID = std::to_string(run->GetRunID());

        std::ofstream ProfilingFile;
        ProfilingFile.open(
            Manager->OutputFileName() + "_" + RunID + ".profile",
            std::ios_base::app    
        );
        ProfilingFile << "function,number_of_calls,total_time[ms]\n"; 
        
        auto Profiles = Manager->GetFunctionProfiles();
        for(auto const& [key, val] : Profiles)
        {
            ProfilingFile << key << ","; 
            ProfilingFile << val.calls << ","; 
            ProfilingFile << val.time << "\n";
        }
        ProfilingFile.close();
#endif
    }
}