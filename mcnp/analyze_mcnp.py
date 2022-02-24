"""

"""
import numpy as np
import scipy as sp
from matplotlib import pyplot as plt
import csv
from parse_mcnp import *
plt.style.use("bmh")
plt.rcParams.update({'font.size': 14})
plt.rcParams.update({'figure.figsize': (10,8)})
plt.rcParams.update({'lines.linewidth': 3})
plt.rcParams.update({'legend.fontsize': 14})
hist_options = {'histtype':'stepfilled', 'alpha':0.8, 'density':True, 'color':'mediumpurple'}

ANTIRESONANCE = .057
LOG10_ANTIRESONANCE = np.log10(ANTIRESONANCE)
LOG10_ANTIRESONANCE_STR = r"$\log_{10}(.057)$"
# anti-resonance resonance peaks
ANTIRESONANCE_PEAKS = [.01181, .017974]
LOG10_ANTIRESONANCE_PEAKS = [np.log10(peak) for peak in ANTIRESONANCE_PEAKS]
LOG10_ANTIRESONANCE_PEAKS_STR = [r"$\log_{10}(.011)$",r"$\log_{10}(.017)$"]

parse = False
input_file = "AllTracks-mod-MCNP-100m-cube-LAr.txt"
output_file = "mcnp_statistics.csv"

if parse == True:
    parse_mcnp(input_file, output_file)

final_ke, displacement, distance, lifetime = import_statistics(output_file)
final_ke = np.log10(final_ke)
energy_cut = [energy for energy in final_ke if (energy < LOG10_ANTIRESONANCE_PEAKS[0])]
gauss_mean = round(np.mean(energy_cut))
gauss_std  = np.std(energy_cut)


fig, axs = plt.subplots()
axs.hist(final_ke, bins=100, **hist_options, label='MCNP')
axs.set_xlabel(r'$KE_{\mathrm{final}}$ (MeV)')
axs.set_title("MCNP Neutron Capture final kE (MeV) in LAr")
ticks = list(axs.get_xticks())
new_ticks = [gauss_mean, np.amin(ticks),np.amax(ticks),LOG10_ANTIRESONANCE]+LOG10_ANTIRESONANCE_PEAKS
new_tick_labels = [str(gauss_mean), str(round(np.amin(ticks))),str(round(np.amax(ticks))),LOG10_ANTIRESONANCE_STR]+LOG10_ANTIRESONANCE_PEAKS_STR
axs.set_xticks(new_ticks)
axs.set_xticklabels(new_tick_labels,rotation='45',horizontalalignment='right')
axs.set_xlabel(r"$\log_{10}(KE) [\log_{10}(MeV)]$")
axs.set_xlim(-7.25,0.75)
plt.legend()
plt.tight_layout()
plt.savefig("final_ke_mcnp.png")
plt.show()