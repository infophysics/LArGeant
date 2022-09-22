/**
 * @file LArGeantArgon.hh
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-30
 */
#pragma once
#include <memory>
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include <G4MaterialPropertiesTable.hh>

namespace largeant
{
    /// constant values for the mass/mol of Argon isotopes
    static constexpr G4double Ar36MassMol = 35.967*g/mole;
    static constexpr G4double Ar38MassMol = 37.962*g/mole;
    static constexpr G4double Ar40MassMol = 39.962*g/mole;

    class LArGeantArgon
    {
    public:
        /**
         * @brief Construct a new LArGeantArgon object
         * The default parameters are set to LAr with natural
         * argon.
         * 
         * @param temperature 
         * @param pressure 
         * @param Ar36Ratio 
         * @param Ar38Ratio 
         * @param Ar40Ratio 
         */
        LArGeantArgon(
            G4double temperature=85.8 * kelvin, 
            G4double pressure   =0.952 * atmosphere,
            G4double Ar36Ratio  =0.334, 
            G4double Ar38Ratio  =0.063, 
            G4double Ar40Ratio  =99.603
        );
        ~LArGeantArgon();

        void setTemperature(G4double temperature);
        void setPressure(G4double pressure);
        void setRatios(
            G4double Ar36Ratio, 
            G4double Ar38Ratio, 
            G4double Ar40Ratio
        );
        
        G4double getTemperature() const   { return mTemperature; }
        G4double getPressure() const      { return mPressure; }
        G4double getAr36Ratio() const     { return mAr36Ratio; }
        G4double getAr38Ratio() const     { return mAr38Ratio; }
        G4double getAr40Ratio() const     { return mAr40Ratio; }
        G4double getAr36Density() const   { return mAr36Density; }
        G4double getAr38Density() const   { return mAr38Density; }
        G4double getAr40Density() const   { return mAr40Density; }
        G4double getAverageMassMol() const { return mAverageMassMol; }
        G4double getAverageDensity() const { return mAverageDensity; }

        std::shared_ptr<G4Isotope> getIAr36() const { return mIAr36; }
        std::shared_ptr<G4Isotope> getIAr38() const { return mIAr38; }
        std::shared_ptr<G4Isotope> getIAr40() const { return mIAr40; }
        std::shared_ptr<G4Element> getArIsotopes() const { return mArIsotopes; }

        std::shared_ptr<G4Material> getLAr() const { return mLAr; }

        void DefineMaterials();

    private:
        G4double mTemperature;
        G4double mPressure;

        G4double mAr36Ratio, mAr38Ratio, mAr40Ratio;
        G4double mAr36Density, mAr38Density, mAr40Density;

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