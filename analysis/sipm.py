from enum import unique
import uproot
from matplotlib import pyplot as plt
import numpy as np
import csv

input_file = "default_0.root"
eff_file = "VUV4-50ump.dat"

wlen = []
eff = []
with open(eff_file, "r") as file:
    reader = csv.reader(file, delimiter=" ")
    for row in reader:
        wlen.append(float(row[0]))
        eff.append(float(row[1]))

arrays = uproot.open(input_file)['Hits'].arrays(library="np")
t = arrays['t_event']
x = arrays['x_particle']
z = arrays['z_particle']
energy = arrays['E_particle'] * 1e6
wavelength = 1239.8 / energy

det = arrays['detected']
energy_det = energy[(det == 1)]
wavelength_det = wavelength[(det == 1)]

energy_hist, energy_bin_edges = np.histogram(energy, bins=100)
wavelength_bin_edges = 1239.8 / energy_bin_edges
detect_hist, detect_bin_edges = np.histogram(energy_det, bins=energy_bin_edges)

energy_hist = np.array(energy_hist)
energy_bin_edges = np.array(energy_bin_edges)
detect_hist = np.array(detect_hist)

efficiency = np.nan_to_num(detect_hist / energy_hist)
energy_bins = np.nan_to_num([energy_bin_edges[i] + (energy_bin_edges[i+1] - energy_bin_edges[i])/2 for i in range(len(energy_bin_edges)-1)])
wavelength_bins = 1239.8 / energy_bins

fig, axs = plt.subplots(figsize=(12,6))
axs.plot(wavelength_bins, efficiency, label="simulation")
axs.plot(wlen, eff, label="datasheet")
axs.set_xlabel("Photon wavelength [nm]")
axs.set_ylabel("Quantum Efficiency")
plt.title("VUV4-50ump Quantum Efficiency vs. Wavelength [nm]")
plt.legend()
plt.savefig("eff_wavelength.png")

# fig, axs = plt.subplots(figsize=(10,6))
# axs.hist(energy, bins=100, label="all", histtype='step', stacked=True)
# axs.hist(energy[det == 1], bins=100, label="detected", histtype='step', stacked=True)
# axs.set_xlabel("Energy [eV]")
# plt.title("Energy of incident photons [eV]")
# plt.legend()
# plt.savefig(".png")


wlen_mean = round(np.mean(wavelength),2)
wlen_height = 0
for i in range(len(energy_bin_edges) - 1):
    if (wlen_mean <= wavelength_bin_edges[i]) and (wlen_mean > wavelength_bin_edges[i+1]):
        wlen_height = round(energy_hist[i],2)
det_mean = round(np.mean(wavelength_det),2)
det_height = 0
for i in range(len(energy_bin_edges) - 1):
    if (det_mean <= wavelength_bin_edges[i]) and (det_mean > wavelength_bin_edges[i+1]):
        det_height = round(detect_hist[i],2)

extra_ticks = [wlen_mean, det_mean]
fig, axs = plt.subplots(figsize=(12,6))
axs.hist(wavelength, bins=100, label="all", histtype='step', stacked=True, color='k')
axs.hist(wavelength_det, bins=100, label="detected", histtype='step', stacked=True, color='r')
axs.axvline(wlen_mean, ymin=0, ymax=(wlen_height/axs.get_ylim()[1]), linestyle='--', label=r"$\langle$all$\rangle$: "+f"{wlen_mean:.2f}", c='k')
axs.axvline(det_mean, ymin=0, ymax=(det_height/axs.get_ylim()[1]), linestyle='--', label=r"$\langle$detected$\rangle$: "+f"{det_mean:.2f}", c='r')
axs.set_xlabel("Wavelength [nm]")

# axs.set_xticks(list(axs.get_xticks()) + extra_ticks)
# axs.set_xticklabels(list(axs.get_xticks()), rotation=45, ha='right')
plt.title("Wavelength of incident photons [nm]")
plt.tight_layout()
plt.legend()
plt.savefig("wavelength.png")