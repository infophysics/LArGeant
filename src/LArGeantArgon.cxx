/**
 * @file LArGeantArgon.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-30
 */
#include "LArGeantArgon.hh"

namespace largeant
{
    LArGeantArgon::LArGeantArgon(
        G4double temperature, G4double pressure,
        G4double Ar36Ratio, G4double Ar38Ratio, G4double Ar40Ratio
    )
    : fTemperature(temperature)
    , fPressure(pressure)
    , fAr36Ratio(Ar36Ratio)
    , fAr38Ratio(Ar38Ratio)
    , fAr40Ratio(Ar40Ratio)
    {

        fAverageDensity = 1.406*g/cm3;
        fNaturalArDensity = 1.3973*g/cm3;

        DefineMaterials();
    }

    LArGeantArgon::~LArGeantArgon()
    {
    }

    void LArGeantArgon::setTemperature(G4double temperature)
    {
        fTemperature = temperature;
        DefineMaterials();
    }
    void LArGeantArgon::setPressure(G4double pressure)
    {
        fPressure = pressure;
        DefineMaterials();
    }
    void LArGeantArgon::setRatios(
        G4double Ar36Ratio, 
        G4double Ar38Ratio, 
        G4double Ar40Ratio
    )
    {
        fAr36Ratio = Ar36Ratio;
        fAr38Ratio = Ar38Ratio;
        fAr40Ratio = Ar40Ratio;
        DefineMaterials();
    }

    void LArGeantArgon::DefineMaterials()
    {
        G4NistManager *nist = G4NistManager::Instance();

        fIAr36 = std::make_shared<G4Isotope>(
            "Ar36",         // name
            18.,            // atomic number
            36,             // atomic mass
            Ar36MassMol     // molar mass
        );  
        
        fIAr38 = std::make_shared<G4Isotope>(
            "Ar38",         // name
            18.,            // atomic number
            38,             // atomic mass
            Ar38MassMol     // molar mass
        );  

        fIAr40 = std::make_shared<G4Isotope>(
            "Ar40",         // name
            18.,            // atomic number
            40,             // atomic mass
            Ar40MassMol     // molar mass
        );  

        // add the isotopes to the definition of Ar
        fArIsotopes = std::make_shared<G4Element>("ArIsotopes", "Ar", 3);
        fArIsotopes->AddIsotope(fIAr36.get(), fAr36Ratio * perCent);
        fArIsotopes->AddIsotope(fIAr38.get(), fAr38Ratio * perCent);
        fArIsotopes->AddIsotope(fIAr40.get(), fAr40Ratio * perCent);

        // need now the definition of LAr with the composition
        fLAr = std::make_shared<G4Material>(
            "LAr",          // name
            fAverageDensity,// density
            1,              // # of components
            kStateLiquid,   // state
            fTemperature,   // temperature
            fPressure       // pressure
        );     
        
        fLAr->AddElement(fArIsotopes.get(), 1);
    }
}