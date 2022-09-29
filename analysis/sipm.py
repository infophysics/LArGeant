from enum import unique
import uproot
from matplotlib import pyplot as plt
import numpy as np

input_file = "output_0.root"
arrays = uproot.open(input_file)['Hits'].arrays(library="np")
x = arrays['x_particle']
z = arrays['z_particle']

fig, axs = plt.subplots()
axs.scatter(x, z)
plt.show()