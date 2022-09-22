from enum import unique
import uproot
from matplotlib import pyplot as plt
import numpy as np

input_file = "output_0.root"
bins = 250

extra_x_ticks = [5486, 5442, 5388]


arrays = uproot.open(input_file)['NEST'].arrays(library="np")
dE = arrays['dE']
dx = arrays['dx']
dEdx = arrays['dEdx']
dP = arrays['LightYield']
dQ = arrays['ChargeYield']
particle = arrays['particle_name']
parent = arrays['parent_name']

unique_particles = ['mu-']



fig, axs = plt.subplots()
for name in unique_particles:
    x = dEdx[(particle == name)] / 100
    q = dQ[(particle == name)] / 100
    axs.scatter(x,q)
axs.set_xlim([0,20])
axs.set_xlabel("dE/dx [MeV/cm]")
axs.set_ylabel("dQ/dx [ke-/cm]")
plt.legend()
plt.tight_layout()
plt.show()


fig, axs = plt.subplots()
for name in unique_particles:
    x = dEdx[(particle == name) & (dEdx < 1000)]
    q = dQ[(particle == name) & (dEdx < 1000)]
    hist, bin_edges = np.histogram(x, bins=bins)
    dQdx = [np.mean(q[(x > bin_edges[ii]) & (x >= bin_edges[ii+1])]) for ii in range(len(bin_edges)-1)]
    meandEdx = [(bin_edges[ii] + bin_edges[ii+1])/2 for ii in range(len(bin_edges)-1)]
    axs.scatter(meandEdx, dQdx)
axs.set_xlabel("dE/dx [keV/mm]")
plt.legend(loc='upper left')
plt.tight_layout()
plt.show()
