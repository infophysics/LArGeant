/**
 * @file NESTInterface.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.0
 * @date 2022-10-05
 */
#pragma once
#include <memory>

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4Step.hh"
#include "G4Track.hh"

#include "VDetector.hh"
#include "LArDetector.hh"
#include "LArNEST.hh"
#include "NEST.hh"
#include "ThermalElectron.hh"

namespace LArGeant
{
    /**
     * @brief 
     * 
     * @details
     * 	Change log:
     * 		2022-10-11 - Initial creation of file.
     */
    struct NESTInterfaceResult
    {
        G4int number_of_photons = 0;
        G4int number_of_electrons = 0;
        G4String particle = "none";
        G4ThreeVector init_position = {0,0,0};
        G4ThreeVector final_position = {0,0,0};
        G4double init_time = 0;
        G4double energy = 0;
        G4double efield = 0;
        G4double density = 0;
        G4double electron_kinetic_energy = 0;
        G4ThreeVector efield_direction = {0,0,0};
        G4int parent_track_id = 0;
    };

    /**
     * @brief 
     * 
     * @details Lineages are used in NESTs LXe
     * calculation when using the energy basis.
     * This approach must be used because all 
     * the energy from a particle interaction needs 
     * to be summed up to find the <N_gamma> and 
     * <N_e-> values.
     * 
     * 	Change log:
     * 		2022-10-11 - Initial creation of file.
     */
    struct Lineage
    {
        NEST::INTERACTION_TYPE type;
        G4double density = -1;
        G4int Z = 0;
        G4int A = 0;
        NEST::NESTresult result;
        bool result_calculated = false;

        Lineage(NEST::INTERACTION_TYPE _type)
        : type(_type)
        {
        }
    };

    class NESTInterface
    {
    public:
        NESTInterface(NESTInterface& other) = delete;
        void operator=(const NESTInterface &) = delete;
        static std::shared_ptr<NESTInterface> GetInterface();

        void SetLArDetector(std::shared_ptr<VDetector> detector);
        void SetLXeDetector(std::shared_ptr<VDetector> detector);
        void SetLXeNEST(std::shared_ptr<NEST::NESTcalc> LXeNEST);
        void SetLArNEST(std::shared_ptr<NEST::LArNEST> LArNEST);
        void SetCalculationMode(G4bool);
        void SetCalculationMode(G4String);

        std::shared_ptr<VDetector> GetLArDetector() { return mLArDetector; }
        std::shared_ptr<VDetector> GetLXeDetector() { return mLXeDetector; }
        std::shared_ptr<NEST::NESTcalc> GetLXeNEST() { return mLXeNEST; }
        std::shared_ptr<NEST::LArNEST> GetLArNEST() { return mLArNEST; }
        G4bool GetCalculationMode() { return mCalculationMode; }

        // LXe specific code
        Lineage GetChildType(const G4Track* parent, const G4Track* child) const;
        void PopulateLineages(const G4Track& track, const G4Step& step);

        NESTInterfaceResult Calculate(
            G4String particle,
            G4ThreeVector init_position,
            G4ThreeVector final_position,
            G4double init_time,
            G4double energy,
            G4double density,
            G4int parent_track_id
        );

    private:
        static std::shared_ptr<NESTInterface> sNESTInterface;

        NESTInterface()
        {
            mLArDetector = std::make_shared<LArDetector>();
            mLArNEST = std::make_shared<NEST::LArNEST>(mLArDetector.get());
            mLXeDetector = std::make_shared<VDetector>();
            mLXeNEST = std::make_shared<NEST::NESTcalc>(mLXeDetector.get());
        }

        std::shared_ptr<VDetector> mLArDetector;
        std::shared_ptr<VDetector> mLXeDetector;
        std::shared_ptr<NEST::NESTcalc> mLXeNEST;
        std::shared_ptr<NEST::LArNEST> mLArNEST;

        std::vector<Lineage> mLineages;
        std::vector<Lineage> mPreviousLineages;

        G4bool mCalculationMode = {1}; 

        NESTInterfaceResult CalculateLAr(
            G4String particle,
            G4ThreeVector init_position,
            G4ThreeVector final_position,
            G4double init_time,
            G4double energy,
            G4double density,
            G4int parent_track_id
        );
        NESTInterfaceResult CalculateLXe(
            G4String particle,
            G4ThreeVector init_position,
            G4ThreeVector final_position,
            G4double init_time,
            G4double energy,
            G4double density,
            G4int parent_track_id
        );

    };
}