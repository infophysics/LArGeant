/**
 * @file Xenon.cxx
 * @author Nicholas Carrara [nmcarrara@ucdavis.edu]
 * @brief 
 * @version 0.1
 * @date 2022-04-30
 */
#include "Xenon.hh"

namespace LArGeant
{
    Xenon::Xenon(
        G4double temperature, G4double pressure,
        G4double Xe128Ratio, G4double Xe129Ratio, 
        G4double Xe130Ratio, G4double Xe131Ratio, 
        G4double Xe132Ratio, G4double Xe134Ratio, 
        G4double Xe136Ratio
    )
    : mTemperature(temperature)
    , mPressure(pressure)
    , mXe128Ratio(Xe128Ratio)
    , mXe129Ratio(Xe129Ratio)
    , mXe130Ratio(Xe130Ratio)
    , mXe131Ratio(Xe131Ratio)
    , mXe132Ratio(Xe132Ratio)
    , mXe134Ratio(Xe134Ratio)
    , mXe136Ratio(Xe136Ratio)
    {

        mAverageDensity = 2.942*g/cm3;
        mNaturalXeDensity = 2.942*g/cm3;
        mAverageMassMol = Xe128MassMol * mXe128Ratio + 
            Xe129MassMol * mXe129Ratio +
            Xe130MassMol * mXe130Ratio +
            Xe131MassMol * mXe131Ratio +
            Xe132MassMol * mXe132Ratio +
            Xe134MassMol * mXe134Ratio +
            Xe136MassMol * mXe136Ratio;

        DefineMaterials();
    }

    Xenon::~Xenon()
    {
    }

    void Xenon::SetTemperature(G4double temperature)
    {
        mTemperature = temperature;
        DefineMaterials();
    }
    void Xenon::SetPressure(G4double pressure)
    {
        mPressure = pressure;
        DefineMaterials();
    }
    void Xenon::SetRatios(
        G4double Xe128Ratio, 
        G4double Xe129Ratio,
        G4double Xe130Ratio,
        G4double Xe131Ratio,
        G4double Xe132Ratio,
        G4double Xe134Ratio,
        G4double Xe136Ratio
    )
    {
        mXe128Ratio = Xe128Ratio;
        mXe129Ratio = Xe129Ratio;
        mXe130Ratio = Xe130Ratio;
        mXe131Ratio = Xe131Ratio;
        mXe132Ratio = Xe132Ratio;
        mXe134Ratio = Xe134Ratio;
        mXe136Ratio = Xe136Ratio;
        DefineMaterials();
    }

    void Xenon::SetRefractiveIndexEnergies(std::vector<G4double> energies)
    {
        mRefractiveIndexEnergies = energies;
    }
    void Xenon::SetRefractiveIndexSpectrum(std::vector<G4double> spectrum)
    {
        mRefractiveIndexSpectrum = spectrum;
    }
    void Xenon::SetAbsorptionLengthEnergies(std::vector<G4double> energies)
    {
        mAbsorptionLengthEnergies = energies;
    }
    void Xenon::SetAbsorptionLengthSpectrum(std::vector<G4double> spectrum)
    {
        mAbsorptionLengthSpectrum = spectrum;
    }
    void Xenon::SetRayleighScatteringEnergies(std::vector<G4double> energies)
    {
        mRayleighScatteringEnergies = energies;
    }
    void Xenon::SetRayleighScatteringSpectrum(std::vector<G4double> spectrum)
    {
        mRayleighScatteringSpectrum = spectrum;
    }

    void Xenon::DefineMaterials()
    {
        mIXe128.reset( 
            new G4Isotope(
                "Xe128",         // name
                54.,            // atomic number
                128,             // atomic mass
                Xe128MassMol     // molar mass
            )
        );
        mIXe129.reset( 
            new G4Isotope(
                "Xe129",         // name
                54.,            // atomic number
                129,             // atomic mass
                Xe129MassMol     // molar mass
            )
        );
        mIXe130.reset( 
            new G4Isotope(
                "Xe130",         // name
                54.,            // atomic number
                130,             // atomic mass
                Xe130MassMol     // molar mass
            )
        );
        mIXe131.reset( 
            new G4Isotope(
                "Xe131",         // name
                54.,            // atomic number
                131,             // atomic mass
                Xe131MassMol     // molar mass
            )
        );
        mIXe132.reset( 
            new G4Isotope(
                "Xe132",         // name
                54.,            // atomic number
                132,             // atomic mass
                Xe132MassMol     // molar mass
            )
        );
        mIXe134.reset( 
            new G4Isotope(
                "Xe134",         // name
                54.,            // atomic number
                134,             // atomic mass
                Xe134MassMol     // molar mass
            )
        );
        mIXe136.reset( 
            new G4Isotope(
                "Xe136",         // name
                54.,            // atomic number
                136,             // atomic mass
                Xe136MassMol     // molar mass
            )
        );
        // add the isotopes to the definition of Ar
        mXeIsotopes.reset(
            new G4Element("XeIsotopes", "Xe", 7)
        );
        mXeIsotopes->AddIsotope(mIXe128.get(), mXe128Ratio * perCent);
        mXeIsotopes->AddIsotope(mIXe129.get(), mXe129Ratio * perCent);
        mXeIsotopes->AddIsotope(mIXe130.get(), mXe130Ratio * perCent);
        mXeIsotopes->AddIsotope(mIXe131.get(), mXe131Ratio * perCent);
        mXeIsotopes->AddIsotope(mIXe132.get(), mXe132Ratio * perCent);
        mXeIsotopes->AddIsotope(mIXe134.get(), mXe134Ratio * perCent);
        mXeIsotopes->AddIsotope(mIXe136.get(), mXe136Ratio * perCent);    

        // need now the definition of LAr with the composition
        mLXe.reset(
            new G4Material(
                "LXe",          // name
                54.0,           // number
                mAverageMassMol,// # of components
                mAverageDensity,// density
                kStateLiquid,   // state
                mTemperature,   // temperature
                mPressure       // pressure
            )
        );    

        // mRefractiveIndexEnergies = {
        //     1.18626 * eV, 1.68626 * eV, 2.18626 * eV, 
        //     2.68626 * eV, 3.18626 * eV, 3.68626 * eV, 
        //     4.18626 * eV, 4.68626 * eV, 5.18626 * eV, 
        //     5.68626 * eV, 6.18626 * eV, 6.68626 * eV, 
        //     7.18626 * eV, 7.68626 * eV, 8.18626 * eV, 
        //     8.68626 * eV, 9.18626 * eV, 9.68626 * eV, 
        //     10.1863 * eV, 10.6863 * eV, 11.1863 * eV 
        // };
        // mRefractiveIndexSpectrum = {
        //     1.24664, 1.2205, 1.22694, 
        //     1.22932, 1.23124, 1.23322, 
        //     1.23545, 1.23806, 1.24116, 
        //     1.24489, 1.24942, 1.25499, 
        //     1.26197, 1.2709, 1.28263, 
        //     1.29865, 1.32169, 1.35747, 
        //     1.42039, 1.56011, 2.16626 
        // };

        // // Set up absorption length defaults for LAr.  These values also
        // // taken from LArSoft.
        // mAbsorptionLengthEnergies = {
        //     4, 5, 6, 
        //     7, 8, 9, 
        //     10, 11
        // };
        // mAbsorptionLengthSpectrum = {
        //     2000. * cm, 2000. * cm, 2000. * cm, 
        //     2000. * cm, 2000. * cm, 2000. * cm,
        //     2000. * cm, 2000. * cm
        // };

        // // Rayleigh scattering length also taken from LArSoft, Units are (cm) @ 90K as a function of energy (eV) 
        // // Also taken from LArSoft, original paper: from arXiv:2002.09346
        // mRayleighScatteringEnergies = {
        //     1.18626 * eV, 1.68626 * eV, 2.18626 * eV, 
        //     2.68626 * eV, 3.18626 * eV, 3.68626 * eV, 
        //     4.18626 * eV, 4.68626 * eV, 5.18626 * eV, 
        //     5.68626 * eV, 6.18626 * eV, 6.68626 * eV, 
        //     7.18626 * eV, 7.68626 * eV, 8.18626 * eV, 
        //     8.68626 * eV, 9.18626 * eV, 9.68626 * eV, 
        //     10.1863 * eV, 10.6863 * eV, 11.1863 * eV
        // };
        // mRayleighScatteringSpectrum = {
        //     1200800 * cm, 390747 * cm, 128633 * cm, 
        //     54969.1 * cm, 27191.8 * cm, 14853.7 * cm, 
        //     8716.9 * cm, 5397.42 * cm, 3481.37 * cm, 
        //     2316.51 * cm, 1577.63 * cm, 1092.02 * cm, 
        //     763.045 * cm, 534.232 * cm, 371.335 * cm, 
        //     252.942 * cm, 165.38 * cm, 99.9003 * cm, 
        //     51.2653 * cm, 17.495 * cm, 0.964341 * cm
        // };

        // // Taken from LArSoft, Fast and slow scintillation emission spectra, from [J Chem Phys vol 91 (1989) 1469]
        // mFastScintillationEnergies = {
        //     7.2,  7.9,  8.3,  
        //     8.6,  8.9,  9.1,  
        //     9.3,  9.6,  9.7,  
        //     9.8,  10,  10.2,  
        //     10.3,  10.6,  11,  
        //     11.6,  11.9
        // };
        // mSlowScintillationEnergies = {
        //     7.2,  7.9,  8.3,  
        //     8.6,  8.9,  9.1,  
        //     9.3,  9.6,  9.7,  
        //     9.8,  10,  10.2,  
        //     10.3,  10.6,  11,  
        //     11.6,  11.9
        // };
        // mFastScintillationSpectrum = {
        //     0.0,  0.04, 0.12, 
        //     0.27, 0.44, 0.62, 
        //     0.80, 0.91, 0.92, 
        //     0.85, 0.70, 0.50, 
        //     0.31, 0.13, 0.04,  
        //     0.01, 0.0
        // };
        // mSlowScintillationSpectrum = {
        //     0.0,  0.04, 0.12, 
        //     0.27, 0.44, 0.62, 
        //     0.80, 0.91, 0.92, 
        //     0.85, 0.70, 0.50, 
        //     0.31, 0.13, 0.04,  
        //     0.01, 0.0
        // };

        // G4double RadiationLength = 19.55 * g/(cm * cm);

        // // Add refractive index, absorption length and rayleigh scattering
        // // to a new properties table.
        // G4MaterialPropertiesTable* mptLXe = new G4MaterialPropertiesTable();
        // mptLXe->AddProperty(
        //     "RINDEX", 
        //     mRefractiveIndexEnergies, 
        //     mRefractiveIndexSpectrum, 
        //     mRefractiveIndexEnergies.size()
        // );
        // mptLXe->AddProperty(
        //     "ABSLENGTH", 
        //     mAbsorptionLengthEnergies, 
        //     mAbsorptionLengthSpectrum, 
        //     mAbsorptionLengthEnergies.size()
        // );
        // mptLXe->AddProperty(
        //     "RAYLEIGH", 
        //     mRayleighScatteringEnergies, 
        //     mRayleighScatteringSpectrum, 
        //     mRayleighScatteringEnergies.size()
        // );
        // mLXe->SetMaterialPropertiesTable(mptLXe);
    }

    void Xenon::PrintProperties()
    {
        mLXe->GetMaterialPropertiesTable()->DumpTable();
        G4cout << "Nuclear interaction length: " << mLXe->GetNuclearInterLength() << G4endl;
        G4cout << "ElectronDensity: " << mLXe->GetElectronDensity() << G4endl;
        G4cout << "Radiation lenth: " << mLXe->GetRadlen() << G4endl;
        G4cout << "Temperature:     " << mLXe->GetTemperature() << G4endl;
        G4cout << "Pressure:        " << mLXe->GetPressure() << G4endl;
    }
}