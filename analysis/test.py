from enum import unique
import uproot
from matplotlib import pyplot as plt
import numpy as np
import csv

input_file = "default_0.root"

hits = uproot.open(input_file)['Hits'].arrays(library="np")
hit_event = hits['event']
hit_track_id = hits['track_id']
hit_parent_track_id = hits['parent_id']
t = hits['local_time']
gt = hits['global_time']
x = hits['x_particle']
z = hits['z_particle']
energy = hits['energy'] * 1e6
wavelength = 1239.8 / energy
det = hits['detected']
hit_scint = hits['scintillation_ancestor_pdg']

particles = uproot.open(input_file)['ParticleMaps'].arrays(library="np")
particle_event = particles['event']
particle = particles['particle']
particle_track_id = particles['track_id']
parent_track_id = particles['parent_track_id']
scint_track_id = particles['scintillation_ancestor_track_id']
scint_pdg = particles['scintillation_ancestor_pdg']



unique_events = np.unique(particle_event)
for event in unique_events:
    part = particle[(particle_event == event)]

    t_event = gt[(hit_event == event)]
    t_event -= min(t_event)
    t_local = t[(hit_event == event)]
    id_event = hit_track_id[(hit_event == event)]

    track_id = particle_track_id[(particle_event == event)]
    scint = scint_pdg[(particle_event == event)]

    scint = hit_scint[(hit_event == event)]

    unique_scint = np.unique(scint)
    for ii in unique_scint:
        print(ii)
    print(len(unique_scint))
    fig, axs = plt.subplots()
    axs.hist(scint.astype(str), bins=100)
    axs.set_xticks([ii for ii in range(len(unique_scint))])
    axs.set_xticklabels(unique_scint,rotation=45,ha='right')
    axs.set_yscale("log")
    plt.tight_layout()
    plt.show()
    
    fig, axs = plt.subplots()
    axs.hist(t_local, bins=100, label='all', histtype='step', stacked=True)
    for ii in unique_scint:
        ts = t_local[(scint == ii)] + t_event[(scint==ii)]
        print(ts)
        axs.hist(ts, bins=100, label=ii, histtype='step',stacked=True)
    axs.set_xlabel("local time [ns]")
    axs.set_ylabel("sipm hits [n]")
    plt.legend()
    plt.show()

    









# for event in unique_events:
#     t_event = t[(hit_event == event)]
#     hit_id = hit_track_id[(hit_event == event)]

#     particle_id = particle_track_id[(particle_event == event)]
#     parent_id = parent_track_id[(particle_event == event)]
#     p = particle[(particle_event == event)]
#     scint_id = scint_track_id[(particle_event == event)]

#     p_scint = p[(scint_id != 0)]
#     scint = scint_id[(scint_id != 0)]
#     particle_scint = particle_id[(scint_id != 0)]
    
#     print(np.unique(p_scint))
#     print(len(p_scint[(p_scint == 'alpha')]))

#     # hit_parent_id = hit_parent_track_id[(hit_event == event)]
#     # hit_particle = np.concatenate([p_scint[(particle_scint == id)] for id in hit_parent_id])
    
#     # print(hit_particle)
    
#     # alpha_hit = hit_id[(hit_particle == 'alpha')]
#     # t_alpha = t_event[(hit_particle == 'alpha')]

#     # print(alpha_hit)

#     # alpha_t.append(list(t_alpha))

# alpha_t = np.concatenate((alpha_t), axis=0)
# print(alpha_t)

# fig, axs = plt.subplots()
# axs.hist(t, bins=100, stacked=True, histtype='step', label='all')
# axs.hist(alpha_t, bins=100, stacked=True, histtype='step', label='alpha')
# plt.legend()
# plt.show()