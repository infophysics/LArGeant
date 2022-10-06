/**
 * @file Argon.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-30
 */
#pragma once
#include <memory>

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4MaterialPropertiesTable.hh"

namespace LArGeant
{
    /// constant values for the mass/mol of Argon isotopes
    static constexpr G4double Ar36MassMol = 35.967*g/mole;
    static constexpr G4double Ar38MassMol = 37.962*g/mole;
    static constexpr G4double Ar40MassMol = 39.962*g/mole;

    class Argon
    {
    public:
        /**
         * @brief Construct a new Argon object
         * The default parameters are Set to LAr with natural
         * argon.
         * 
         * @param temperature 
         * @param pressure 
         * @param Ar36Ratio 
         * @param Ar38Ratio 
         * @param Ar40Ratio 
         */
        Argon(
            G4double temperature=85.8 * kelvin, 
            G4double pressure   =0.952 * atmosphere,
            G4double Ar36Ratio  =0.334, 
            G4double Ar38Ratio  =0.063, 
            G4double Ar40Ratio  =99.603
        );
        ~Argon();

        void SetTemperature(G4double temperature);
        void SetPressure(G4double pressure);
        void SetRatios(
            G4double Ar36Ratio, 
            G4double Ar38Ratio, 
            G4double Ar40Ratio
        );

        void SetRefractiveIndexEnergies(std::vector<G4double>);
        void SetRefractiveIndexSpectrum(std::vector<G4double>);
        void SetAbsorptionLengthEnergies(std::vector<G4double>);
        void SetAbsorptionLengthSpectrum(std::vector<G4double>);
        void SetRayleighScatteringEnergies(std::vector<G4double>);
        void SetRayleighScatteringSpectrum(std::vector<G4double>);
        
        G4double GetTemperature() const   { return mTemperature; }
        G4double GetPressure() const      { return mPressure; }
        G4double GetAr36Ratio() const     { return mAr36Ratio; }
        G4double GetAr38Ratio() const     { return mAr38Ratio; }
        G4double GetAr40Ratio() const     { return mAr40Ratio; }
        G4double GetAr36Density() const   { return mAr36Density; }
        G4double GetAr38Density() const   { return mAr38Density; }
        G4double GetAr40Density() const   { return mAr40Density; }
        G4double GetAverageMassMol() const { return mAverageMassMol; }
        G4double GetAverageDensity() const { return mAverageDensity; }

        std::shared_ptr<G4Isotope> GetIAr36() const { return mIAr36; }
        std::shared_ptr<G4Isotope> GetIAr38() const { return mIAr38; }
        std::shared_ptr<G4Isotope> GetIAr40() const { return mIAr40; }
        std::shared_ptr<G4Element> GetArIsotopes() const { return mArIsotopes; }

        std::vector<G4double> GetRefractiveIndexEnergies() const { return mRefractiveIndexEnergies; }
        std::vector<G4double> GetRefractiveIndexSpectrum() const { return mRefractiveIndexSpectrum; }
        std::vector<G4double> GetAbsorptionLengthEnergies() const { return mAbsorptionLengthEnergies; }
        std::vector<G4double> GetAbsorptionLengthSpectrum() const { return mAbsorptionLengthSpectrum; }
        std::vector<G4double> GetRayleighScatteringEnergies() const { return mRayleighScatteringEnergies; }
        std::vector<G4double> GetRayleighScatteringSpectrum() const { return mRayleighScatteringSpectrum; }

        std::shared_ptr<G4Material> GetLAr() const { return mLAr; }

        void DefineMaterials();
        void PrintProperties();

    private:
        G4double mTemperature;
        G4double mPressure;

        G4double mAr36Ratio, mAr38Ratio, mAr40Ratio;
        G4double mAr36Density, mAr38Density, mAr40Density;

        std::vector<G4double> mRefractiveIndexEnergies;
        std::vector<G4double> mRefractiveIndexSpectrum;

        std::vector<G4double> mAbsorptionLengthEnergies;
        std::vector<G4double> mAbsorptionLengthSpectrum;

        std::vector<G4double> mRayleighScatteringEnergies;
        std::vector<G4double> mRayleighScatteringSpectrum;

        std::vector<G4double> mFastScintillationEnergies;
        std::vector<G4double> mFastScintillationSpectrum;
        std::vector<G4double> mSlowScintillationEnergies;
        std::vector<G4double> mSlowScintillationSpectrum;

        G4double mAverageMassMol;
        G4double mAverageDensity;
        G4double mNaturalArDensity;

        std::shared_ptr<G4Isotope> mIAr36 = {nullptr};
        std::shared_ptr<G4Isotope> mIAr38 = {nullptr};
        std::shared_ptr<G4Isotope> mIAr40 = {nullptr};
        std::shared_ptr<G4Element> mArIsotopes = {nullptr};
        std::shared_ptr<G4Material> mLAr = {nullptr};
    };
}