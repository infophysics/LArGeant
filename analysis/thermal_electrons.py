from enum import unique
import uproot
from matplotlib import pyplot as plt
import numpy as np

input_file = "output_0.root"
arrays = uproot.open(input_file)['ThermalElectrons'].arrays(library="np")
energy = arrays['energy'] * 1e6
track_length = arrays['track_length']

fig, axs = plt.subplots()
axs.hist(track_length, bins=100, density=True, histtype='step')
axs.set_xlabel("Track length [cm]")
axs.set_yscale("log")
plt.title("LAr Scintillation Thermal Electron Track Length.")
plt.tight_layout()
plt.savefig("thermal_electron_track_length.png")

fig, axs = plt.subplots()
axs.hist(energy, bins=100, density=True, histtype='step')
axs.set_xlabel("Energy [eV]")
plt.title("LAr Scintillation Thermal Electron Energy.")
plt.tight_layout()
plt.savefig("thermal_electron_energy.png")
