from enum import unique
import uproot
from matplotlib import pyplot as plt
import numpy as np

input_file = "output_0.root"
arrays = uproot.open(input_file)['OpticalPhotons'].arrays(library="np")
energy = arrays['energy'] * 1e6
wavelength = 1239.8/energy
track_length = arrays['track_length']

fig, axs = plt.subplots()
axs.hist(track_length[(wavelength <= 200)], bins=100, density=True, histtype='step')
axs.set_xlabel("Track length [cm]")
axs.set_yscale("log")
plt.title("LAr Scintillation Optical Photon Track Length.")
plt.tight_layout()
plt.savefig("optical_photon_track_length.png")

fig, axs = plt.subplots()
axs.hist(energy, bins=100, density=True, histtype='step')
axs.set_xlabel("Energy [eV]")
plt.title("LAr Scintillation Optical Photon Energy.")
plt.tight_layout()
plt.savefig("optical_photon_energy.png")

fig, axs = plt.subplots()
axs.hist(wavelength[(wavelength <= 200)], bins=100, density=True, histtype='step')
axs.set_xlabel(r"$\lambda$" + " [nm]")
plt.title("LAr Scintillation Optical Photon Wavelength.")
plt.tight_layout()
plt.savefig("optical_photon_wavelength.png")

fig, axs = plt.subplots()
axs.scatter(wavelength[(wavelength <= 140)], track_length[(wavelength <= 140)])
axs.set_xlabel(r"$\lambda$" + " [nm]")
axs.set_ylabel("Track length [cm]")
plt.title("LAr Scintillation Optical Photon Track Length vs. Wavelength.")
plt.tight_layout()
plt.savefig("optical_photon_track_length_vs_wavelength.png")