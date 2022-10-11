from enum import unique
import uproot
from matplotlib import pyplot as plt
import numpy as np

input_file = "default_0.root"
bins = 50

extra_x_ticks = [5486, 5442, 5388]

arrays = uproot.open(input_file)['EnergyDepositions'].arrays(library="np")
events = arrays['event']
edeps = arrays['edep']
particle = arrays['particle']

event_total = [sum(edeps[(events == i) & (particle == 'alpha')]) for i in np.unique(events)]
print(event_total)

fig, axs = plt.subplots()
axs.hist(event_total, bins=25)
plt.show()

arrays = uproot.open(input_file)['NEST'].arrays(library="np")
dE = arrays['energy']
dP = arrays['number_of_photons']
dQ = arrays['number_of_electrons']
particle = arrays['particle']

unique_particles = ['alpha']

fig, axs = plt.subplots()
for name in unique_particles:
    axs.hist(dE[(particle == name)], bins=bins, stacked=True, label=name, density=True, histtype='step')
axs.set_xlabel("Energy Deposition [keV]")
axs.set_yscale("log")
axs.set_xticklabels(axs.get_xticks(), rotation=45, ha='right')
axs.set_title("Distribution of energy deposits [keV]")
plt.legend(loc='upper left')
plt.tight_layout()
plt.savefig("energy_depositions.png")

fig, axs = plt.subplots()
for name in unique_particles:
    axs.scatter(dE[(particle == name)], dP[(particle == name)]/dE[(particle == name)], label=name)
axs.set_xlabel("Energy Deposition [keV]")
axs.set_ylabel(r"Light Yield [$\gamma$/keV]")
axs.set_xticklabels(axs.get_xticks(), rotation=45, ha='right')
axs.set_title("Light Yield vs. Energy Deposition [keV]")
plt.legend(loc='upper left')
plt.tight_layout()
plt.savefig("light_yield.png")

fig, axs = plt.subplots()
for name in unique_particles:
    axs.scatter(dE[(particle == name)], dQ[(particle == name)]/dE[(particle == name)], label=name)
axs.set_xlabel("Energy Deposition [keV]")
axs.set_ylabel(r"Charge Yield [$e^-$/keV]")
axs.set_xticklabels(axs.get_xticks(), rotation=45, ha='right')
axs.set_title("Charge Yield vs. Energy Deposition [keV]")
plt.legend(loc='upper left')
plt.tight_layout()
plt.savefig("charge_yield.png")
