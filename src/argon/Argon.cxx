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
        // Set up refractive index with 128nm scintillation photons
        G4double RIndexEnergies[21] = {1.18626 * eV, 1.68626 * eV, 2.18626 * eV, 2.68626 * eV, 3.18626 * eV, 3.68626 * eV, 4.18626 * eV, 4.68626 * eV, 5.18626 * eV, 5.68626 * eV, 6.18626 * eV, 6.68626 * eV, 7.18626 * eV, 7.68626 * eV, 8.18626 * eV, 8.68626 * eV, 9.18626 * eV, 9.68626 * eV, 10.1863 * eV, 10.6863 * eV, 11.1863 * eV };
        G4double RIndexSpectrum[21] = {1.24664, 1.2205, 1.22694, 1.22932, 1.23124, 1.23322, 1.23545, 1.23806, 1.24116, 1.24489, 1.24942, 1.25499, 1.26197, 1.2709, 1.28263, 1.29865, 1.32169, 1.35747, 1.42039, 1.56011, 2.16626 };

        G4double AbsLengthEnergies[8] = {4 * eV, 5 * eV, 6 * eV, 7 * eV, 8 * eV, 9 * eV, 10 * eV, 11 * eV};
        G4double AbsLengthSpectrum[8] = {2000. * cm, 2000. * cm, 2000. * cm, 2000. * cm, 2000. * cm, 2000. * cm, 2000. * cm, 2000. * cm};

        // Rayleigh scattering length (cm) @ 90K as a function of energy (eV) from arXiv:2002.09346
        G4double RayleighEnergies[21] = {1.18626 * eV, 1.68626 * eV, 2.18626 * eV, 2.68626 * eV, 3.18626 * eV, 3.68626 * eV, 4.18626 * eV, 4.68626 * eV, 5.18626 * eV, 5.68626 * eV, 6.18626 * eV, 6.68626 * eV, 7.18626 * eV, 7.68626 * eV, 8.18626 * eV, 8.68626 * eV, 9.18626 * eV, 9.68626 * eV, 10.1863 * eV, 10.6863 * eV, 11.1863 * eV};
        G4double RayleighSpectrum[21] = {1200800 * cm, 390747 * cm, 128633 * cm, 54969.1 * cm, 27191.8 * cm, 14853.7 * cm, 8716.9 * cm, 5397.42 * cm, 3481.37 * cm, 2316.51 * cm, 1577.63 * cm, 1092.02 * cm, 763.045 * cm, 534.232 * cm, 371.335 * cm, 252.942 * cm, 165.38 * cm, 99.9003 * cm, 51.2653 * cm, 17.495 * cm, 0.964341 * cm};

        G4double FastScintEnergies[17] = {7.2,  7.9,  8.3,  8.6,  8.9,  9.1,  9.3,  9.6,  9.7,  9.8,  10,  10.2,  10.3,  10.6,  11,  11.6,  11.9};
        G4double SlowScintEnergies[17] = {7.2,  7.9,  8.3,  8.6,  8.9,  9.1,  9.3,  9.6,  9.7,  9.8,  10,  10.2,  10.3,  10.6,  11,  11.6,  11.9};
        G4double FastScintSpectrum[17] = {0.0,  0.04, 0.12, 0.27, 0.44, 0.62, 0.80, 0.91, 0.92, 0.85, 0.70, 0.50, 0.31, 0.13, 0.04,  0.01, 0.0};
        G4double SlowScintSpectrum[17] = {0.0,  0.04, 0.12, 0.27, 0.44, 0.62, 0.80, 0.91, 0.92, 0.85, 0.70, 0.50, 0.31, 0.13, 0.04,  0.01, 0.0};

        G4double RadiationLength = 19.55 * g/(cm * cm);

        G4MaterialPropertiesTable* mptLAr = new G4MaterialPropertiesTable();
        mptLAr->AddProperty("RINDEX", RIndexEnergies, RIndexSpectrum, 21);
        mptLAr->AddProperty("ABSLENGTH", AbsLengthEnergies, AbsLengthSpectrum, 8);
        mptLAr->AddProperty("RAYLEIGH", RayleighEnergies, RayleighSpectrum, 21);
        mLAr->SetMaterialPropertiesTable(mptLAr);

        mLAr->GetMaterialPropertiesTable()->DumpTable();
        G4cout << "ElectronDensity: " << mLAr->GetElectronDensity() << G4endl;
        G4cout << "Radlen: " << mLAr->GetRadlen() << G4endl;
        G4cout << "NuclearInterLength: " << mLAr->GetNuclearInterLength() << G4endl;
        G4cout << "Temperature: " << mLAr->GetTemperature() << G4endl;
        G4cout << "Pressure: " << mLAr->GetPressure() << G4endl;
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