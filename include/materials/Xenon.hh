/**
 * @file Xenon.hh
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
    /// constant values for the mass/mol of Xenon isotopes
    static constexpr G4double Xe128MassMol = 127.903*g/mole;
    static constexpr G4double Xe129MassMol = 128.904*g/mole;
    static constexpr G4double Xe130MassMol = 129.903*g/mole;
    static constexpr G4double Xe131MassMol = 130.905*g/mole;
    static constexpr G4double Xe132MassMol = 131.904*g/mole;
    static constexpr G4double Xe134MassMol = 133.905*g/mole;
    static constexpr G4double Xe136MassMol = 135.907*g/mole;

    class Xenon
    {
    public:

        /**
         * @brief Construct a new Xenon object
         * The default parameters are Set to LXe with natural
         * Xenon.
         * 
         * @param temperature 
         * @param pressure 
         * @param Xe128Ratio 
         * @param Xe129Ratio 
         * @param Xe130Ratio 
         * @param Xe131Ratio 
         * @param Xe132Ratio 
         * @param Xe134Ratio 
         * @param Xe136Ratio 
         * @details
         * 	Change log:
         * 		2022-10-06 - Initial creation of file.
         */
        Xenon(
            G4double temperature=161.4 * kelvin, 
            G4double pressure   =0.952 * atmosphere,
            G4double Xe128Ratio  =0.0191, 
            G4double Xe129Ratio  =0.26401,
            G4double Xe130Ratio  =0.04071,
            G4double Xe131Ratio  =0.21232,
            G4double Xe132Ratio  =0.26909,
            G4double Xe134Ratio  =0.10436,
            G4double Xe136Ratio  =0.08857
        );
        ~Xenon();

        void SetTemperature(G4double temperature);
        void SetPressure(G4double pressure);
        void SetRatios(
            G4double Xe128Ratio, 
            G4double Xe129Ratio,
            G4double Xe130Ratio,
            G4double Xe131Ratio,
            G4double Xe132Ratio,
            G4double Xe134Ratio,
            G4double Xe136Ratio
        );

        void SetRefractiveIndexEnergies(std::vector<G4double>);
        void SetRefractiveIndexSpectrum(std::vector<G4double>);
        void SetAbsorptionLengthEnergies(std::vector<G4double>);
        void SetAbsorptionLengthSpectrum(std::vector<G4double>);
        void SetRayleighScatteringEnergies(std::vector<G4double>);
        void SetRayleighScatteringSpectrum(std::vector<G4double>);
        
        G4double GetTemperature() const   { return mTemperature; }
        G4double GetPressure() const      { return mPressure; }
        G4double GetXe128Ratio() const     { return mXe128Ratio; }
        G4double GetXe129Ratio() const     { return mXe129Ratio; }
        G4double GetXe130Ratio() const     { return mXe130Ratio; }
        G4double GetXe131Ratio() const     { return mXe131Ratio; }
        G4double GetXe132Ratio() const     { return mXe132Ratio; }
        G4double GetXe134Ratio() const     { return mXe134Ratio; }
        G4double GetXe136Ratio() const     { return mXe136Ratio; }
        G4double GetXe128Density() const   { return mXe128Density; }
        G4double GetXe129Density() const   { return mXe129Density; }
        G4double GetXe130Density() const   { return mXe130Density; }
        G4double GetXe131Density() const   { return mXe131Density; }
        G4double GetXe132Density() const   { return mXe132Density; }
        G4double GetXe134Density() const   { return mXe134Density; }
        G4double GetXe136Density() const   { return mXe136Density; }
        G4double GetAverageMassMol() const { return mAverageMassMol; }
        G4double GetAverageDensity() const { return mAverageDensity; }

        std::shared_ptr<G4Isotope> GetIXe128() const { return mIXe128; }
        std::shared_ptr<G4Isotope> GetIXe129() const { return mIXe129; }
        std::shared_ptr<G4Isotope> GetIXe130() const { return mIXe130; }
        std::shared_ptr<G4Isotope> GetIXe131() const { return mIXe131; }
        std::shared_ptr<G4Isotope> GetIXe132() const { return mIXe132; }
        std::shared_ptr<G4Isotope> GetIXe134() const { return mIXe134; }
        std::shared_ptr<G4Isotope> GetIXe136() const { return mIXe136; }
        std::shared_ptr<G4Element> GetXeIsotopes() const { return mXeIsotopes; }

        std::vector<G4double> GetRefractiveIndexEnergies() const { return mRefractiveIndexEnergies; }
        std::vector<G4double> GetRefractiveIndexSpectrum() const { return mRefractiveIndexSpectrum; }
        std::vector<G4double> GetAbsorptionLengthEnergies() const { return mAbsorptionLengthEnergies; }
        std::vector<G4double> GetAbsorptionLengthSpectrum() const { return mAbsorptionLengthSpectrum; }
        std::vector<G4double> GetRayleighScatteringEnergies() const { return mRayleighScatteringEnergies; }
        std::vector<G4double> GetRayleighScatteringSpectrum() const { return mRayleighScatteringSpectrum; }

        std::shared_ptr<G4Material> GetLXe() const { return mLXe; }

        void DefineMaterials();
        void PrintProperties();

    private:
        G4double mTemperature;
        G4double mPressure;

        G4double mXe128Ratio, mXe129Ratio, mXe130Ratio, mXe131Ratio, mXe132Ratio, mXe134Ratio, mXe136Ratio;
        G4double mXe128Density, mXe129Density, mXe130Density, mXe131Density, mXe132Density, mXe134Density, mXe136Density;

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
        G4double mNaturalXeDensity;

        std::shared_ptr<G4Isotope> mIXe128 = {nullptr};
        std::shared_ptr<G4Isotope> mIXe129 = {nullptr};
        std::shared_ptr<G4Isotope> mIXe130 = {nullptr};
        std::shared_ptr<G4Isotope> mIXe131 = {nullptr};
        std::shared_ptr<G4Isotope> mIXe132 = {nullptr};
        std::shared_ptr<G4Isotope> mIXe134 = {nullptr};
        std::shared_ptr<G4Isotope> mIXe136 = {nullptr};
        std::shared_ptr<G4Element> mXeIsotopes = {nullptr};
        std::shared_ptr<G4Material> mLXe = {nullptr};
    };
}