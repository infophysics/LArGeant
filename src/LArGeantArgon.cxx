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
        fAverageMassMol = Ar36MassMol * fAr36Ratio + Ar38MassMol * fAr38Ratio + Ar40MassMol * fAr40Ratio;

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

        fIAr36.reset( 
            new G4Isotope(
                "Ar36",         // name
                18.,            // atomic number
                36,             // atomic mass
                Ar36MassMol     // molar mass
            )
        );  
        
        fIAr38.reset( 
            new G4Isotope(
                "Ar38",         // name
                18.,            // atomic number
                38,             // atomic mass
                Ar38MassMol     // molar mass
            )
        );  

        fIAr40.reset( 
            new G4Isotope(
                "Ar40",         // name
                18.,            // atomic number
                40,             // atomic mass
                Ar40MassMol     // molar mass
            )
        );  

        // add the isotopes to the definition of Ar
        fArIsotopes.reset(
            new G4Element("ArIsotopes", "Ar", 3)
        );
        fArIsotopes->AddIsotope(fIAr36.get(), fAr36Ratio * perCent);
        fArIsotopes->AddIsotope(fIAr38.get(), fAr38Ratio * perCent);
        fArIsotopes->AddIsotope(fIAr40.get(), fAr40Ratio * perCent);

        // need now the definition of LAr with the composition
        fLAr.reset(
            new G4Material(
                "LAr",          // name
                18.0,           // number
                fAverageMassMol,// # of components
                fAverageDensity,// density
                kStateLiquid,   // state
                fTemperature,   // temperature
                fPressure       // pressure
            )
        );     
        // std::cout << "ElectronDensity: " << fLAr->GetElectronDensity() << std::endl;
        // std::cout << "Radlen: " << fLAr->GetRadlen() << std::endl;
        // std::cout << "NuclearInterLength: " << fLAr->GetNuclearInterLength() << std::endl;
        // std::cout << "Temperature: " << fLAr->GetTemperature() << std::endl;
        // std::cout << "Pressure: " << fLAr->GetPressure() << std::endl;
        // // fLAr.reset(
        // //     nist->FindOrBuildMaterial("G4_lAr")
        // // );
        // G4Material* LAr = nist->FindOrBuildMaterial("G4_lAr");
        // std::string name;
        // //std::cout << "ElectronDensity: " << table->GetConstProperty("ElectronDensity");
        // std::cout << "ElectronDensity: " << LAr->GetElectronDensity() << std::endl;
        // std::cout << "Radlen: " << LAr->GetRadlen() << std::endl;
        // std::cout << "NuclearInterLength: " << LAr->GetNuclearInterLength() << std::endl;
        // std::cout << "Temperature: " << LAr->GetTemperature() << std::endl;
        // std::cout << "Pressure: " << LAr->GetPressure() << std::endl;
        // std::cin >> name;
        //fLAr->AddElement(fArIsotopes.get(), 1);
    }
}