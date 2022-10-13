from enum import unique
import uproot
from matplotlib import pyplot as plt
import numpy as np
import csv

input_file = "default_0.root"

hits = uproot.open(input_file)['Hits'].arrays(library="np")
particles = uproot.open(input_file)['Particle'].arrays(library="np")

hit_event = hits['event']
particle_event = particles['event']

hit_track_id = hits['track_id']
hit_parent_track_id = hits['parent_id']
particle_track_id = particles['track_id']
parent_track_id = particles['parent_track_id']
scint_track_id = particles['scintillation_parent_track_id']

t = hits['t_event']
x = hits['x_particle']
z = hits['z_particle']
energy = hits['E_particle'] * 1e6
wavelength = 1239.8 / energy
det = hits['detected']


particle = particles['particle']
alpha_ids = particle_track_id[(particle == 'alpha')]
alpha_t = []

unique_events = np.unique(particle_event)
print(unique_events)
for event in unique_events:
    part = particle[(particle_event == event)]
    track_id = particle_track_id[(particle_event == event)]
    scint = scint_track_id[(particle_event == event)]
    scint = scint[(scint != 0)]
    p_scint = np.concatenate([part[(track_id == id)] for id in scint])
    print(p_scint[(p_scint == 'alpha')])
    









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