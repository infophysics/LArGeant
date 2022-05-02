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
        
        G4double getTemperature() const   { return fTemperature; }
        G4double getPressure() const      { return fPressure; }
        G4double getAr36Ratio() const     { return fAr36Ratio; }
        G4double getAr38Ratio() const     { return fAr38Ratio; }
        G4double getAr40Ratio() const     { return fAr40Ratio; }
        G4double getAr36Density() const   { return fAr36Density; }
        G4double getAr38Density() const   { return fAr38Density; }
        G4double getAr40Density() const   { return fAr40Density; }
        G4double getAverageMassMol() const { return fAverageMassMol; }
        G4double getAverageDensity() const { return fAverageDensity; }

        std::shared_ptr<G4Isotope> getIAr36() const { return fIAr36; }
        std::shared_ptr<G4Isotope> getIAr38() const { return fIAr38; }
        std::shared_ptr<G4Isotope> getIAr40() const { return fIAr40; }
        std::shared_ptr<G4Element> getArIsotopes() const { return fArIsotopes; }

        std::shared_ptr<G4Material> getLAr() const { return fLAr; }

        void DefineMaterials();

    private:
        G4double fTemperature;
        G4double fPressure;

        G4double fAr36Ratio, fAr38Ratio, fAr40Ratio;
        G4double fAr36Density, fAr38Density, fAr40Density;

        G4double fAverageMassMol;
        G4double fAverageDensity;
        G4double fNaturalArDensity;

        std::shared_ptr<G4Isotope> fIAr36 = {nullptr};
        std::shared_ptr<G4Isotope> fIAr38 = {nullptr};
        std::shared_ptr<G4Isotope> fIAr40 = {nullptr};
        std::shared_ptr<G4Element> fArIsotopes = {nullptr};
        std::shared_ptr<G4Material> fLAr = {nullptr};
    };
}