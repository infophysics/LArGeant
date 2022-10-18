from enum import unique
import uproot
from matplotlib import pyplot as plt
import numpy as np

input_file = "default_0.root"
bins = 50

extra_x_ticks = [5486, 5442, 5388]

arrays = uproot.open(input_file)['EnergyDeposits'].arrays(library="np")
events = arrays['event']
edeps = arrays['edep']
particle = arrays['particle']

event_total = [sum(edeps[(events == i) & (particle == 'alpha')]) for i in np.unique(events)]

arrays = uproot.open(input_file)['NEST'].arrays(library="np")
dE = arrays['energy']
dP = arrays['number_of_photons']
dQ = arrays['number_of_electrons']
particle_nest = arrays['particle']

unique_particles = ['alpha']

fig, axs = plt.subplots()
for name in unique_particles:
    event_total = [sum(edeps[(events == i) & (particle == name)]) for i in np.unique(events)]
    axs.hist(event_total, bins=bins, stacked=True, label=name, density=True, histtype='step')
axs.set_xlabel("Energy Deposition [keV]")
axs.set_yscale("log")
axs.set_xticklabels(axs.get_xticks(), rotation=45, ha='right')
axs.set_title("Distribution of energy deposits [keV]")
plt.legend(loc='upper left')
plt.tight_layout()
plt.savefig("energy_depositions.png")

fig, axs = plt.subplots()
for name in unique_particles:
    axs.scatter(dE[(particle_nest == name)], dP[(particle_nest == name)]/dE[(particle_nest == name)], label=name)
axs.set_xlabel("Energy Deposition [keV]")
axs.set_ylabel(r"Light Yield [$\gamma$/keV]")
axs.set_xticklabels(axs.get_xticks(), rotation=45, ha='right')
axs.set_title("Light Yield vs. Energy Deposition [keV]")
plt.legend(loc='upper left')
plt.tight_layout()
plt.savefig("light_yield.png")

fig, axs = plt.subplots()
for name in unique_particles:
    axs.scatter(dE[(particle_nest == name)], dQ[(particle_nest == name)]/dE[(particle_nest == name)], label=name)
axs.set_xlabel("Energy Deposition [keV]")
axs.set_ylabel(r"Charge Yield [$e^-$/keV]")
axs.set_xticklabels(axs.get_xticks(), rotation=45, ha='right')
axs.set_title("Charge Yield vs. Energy Deposition [keV]")
plt.legend(loc='upper left')
plt.tight_layout()
plt.savefig("charge_yield.png")
