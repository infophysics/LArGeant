/**
 * @file Argon.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-30
 */
#include "Argon.hh"

namespace LArGeant
{
    Argon::Argon(
        G4double temperature, G4double pressure,
        G4double Ar36Ratio, G4double Ar38Ratio, G4double Ar40Ratio
    )
    : mTemperature(temperature)
    , mPressure(pressure)
    , mAr36Ratio(Ar36Ratio)
    , mAr38Ratio(Ar38Ratio)
    , mAr40Ratio(Ar40Ratio)
    {

        mAverageDensity = 1.406*g/cm3;
        mNaturalArDensity = 1.3973*g/cm3;
        mAverageMassMol = Ar36MassMol * mAr36Ratio + Ar38MassMol * mAr38Ratio + Ar40MassMol * mAr40Ratio;

        DefineMaterials();
    }

    Argon::~Argon()
    {
    }

    void Argon::SetTemperature(G4double temperature)
    {
        mTemperature = temperature;
        DefineMaterials();
    }
    void Argon::SetPressure(G4double pressure)
    {
        mPressure = pressure;
        DefineMaterials();
    }
    void Argon::SetRatios(
        G4double Ar36Ratio, 
        G4double Ar38Ratio, 
        G4double Ar40Ratio
    )
    {
        mAr36Ratio = Ar36Ratio;
        mAr38Ratio = Ar38Ratio;
        mAr40Ratio = Ar40Ratio;
        DefineMaterials();
    }

    void Argon::DefineMaterials()
    {
        //G4NistManager *nist = G4NistManager::Instance();

        mIAr36.reset( 
            new G4Isotope(
                "Ar36",         // name
                18.,            // atomic number
                36,             // atomic mass
                Ar36MassMol     // molar mass
            )
        );  
        
        mIAr38.reset( 
            new G4Isotope(
                "Ar38",         // name
                18.,            // atomic number
                38,             // atomic mass
                Ar38MassMol     // molar mass
            )
        );  

        mIAr40.reset( 
            new G4Isotope(
                "Ar40",         // name
                18.,            // atomic number
                40,             // atomic mass
                Ar40MassMol     // molar mass
            )
        );  

        // add the isotopes to the definition of Ar
        mArIsotopes.reset(
            new G4Element("ArIsotopes", "Ar", 3)
        );
        mArIsotopes->AddIsotope(mIAr36.get(), mAr36Ratio * perCent);
        mArIsotopes->AddIsotope(mIAr38.get(), mAr38Ratio * perCent);
        mArIsotopes->AddIsotope(mIAr40.get(), mAr40Ratio * perCent);

        // need now the definition of LAr with the composition
        mLAr.reset(
            new G4Material(
                "LAr",          // name
                18.0,           // number
                mAverageMassMol,// # of components
                mAverageDensity,// density
                kStateLiquid,   // state
                mTemperature,   // temperature
                mPressure       // pressure
            )
        );     
        // std::cout << "ElectronDensity: " << mLAr->GetElectronDensity() << std::endl;
        // std::cout << "Radlen: " << mLAr->GetRadlen() << std::endl;
        // std::cout << "NuclearInterLength: " << mLAr->GetNuclearInterLength() << std::endl;
        // std::cout << "Temperature: " << mLAr->GetTemperature() << std::endl;
        // std::cout << "Pressure: " << mLAr->GetPressure() << std::endl;
        // // mLAr.reset(
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