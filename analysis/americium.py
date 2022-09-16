from enum import unique
import uproot
from matplotlib import pyplot as plt
import numpy as np

input_file = "output_1.root"
bins = 250

extra_x_ticks = [5869, 5486, 5830, 7066, 6341, ]


arrays = uproot.open(input_file)['NEST'].arrays(library="np")
dE = arrays['dE']
dx = arrays['dx']
dP = arrays['dP']
dQ = arrays['dQ']
particle = arrays['particle_name']
parent = arrays['parent_name']

unique_particles = ['alpha']
print(particle[(parent == 'Pa233')])

fig, axs = plt.subplots()
for name in unique_particles:
    parents = parent[(particle == name)]
    unique_parents = np.unique(parents)
    hist, bin_edges = np.histogram(dE[(particle == name)], bins=bins)
    for p in unique_parents:
        x = dE[(particle == name)][(parents == p)]
        axs.hist(x, bins=bin_edges, label=p, stacked=True, density=True, histtype='step')
axs.set_xlabel("energy deposition [keV]")
#axs.set_yscale("log")
axs.set_xticks(list(axs.get_xticks()) + extra_x_ticks)
axs.set_xticklabels(axs.get_xticks(), rotation=45, ha='right')
axs.set_xlim([4000,max(dE)])
plt.legend(loc='upper left')
plt.tight_layout()
plt.show()
