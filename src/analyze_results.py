from dataclasses import dataclass
import numpy as np
import csv
import matplotlib.pyplot as plt
from typing import List
from scipy.optimize import curve_fit

plt.style.use("bmh")
plt.rcParams.update({'font.size': 14})
plt.rcParams.update({'figure.figsize': (10,8)})
plt.rcParams.update({'lines.linewidth': 3})
plt.rcParams.update({'legend.fontsize': 14})
hist_options = {'histtype':'step', 'alpha':0.8, 'density':True}

# constants
ANTIRESONANCE = .057
LOG10_ANTIRESONANCE = np.log10(ANTIRESONANCE)
LOG10_ANTIRESONANCE_STR = r"$\log_{10}(.057)$"
# anti-resonance resonance peaks
ANTIRESONANCE_PEAKS = [.01181, .017974]
LOG10_ANTIRESONANCE_PEAKS = [np.log10(peak) for peak in ANTIRESONANCE_PEAKS]
LOG10_ANTIRESONANCE_PEAKS_STR = [r"$\log_{10}(.011)$",r"$\log_{10}(.017)$"]


ENDF_energy = []
ENDF_xsec = []
with open("../data/ENDF-6_Kawano.txt","r") as file:
    reader = csv.reader(file, delimiter=",")
    for row in reader:
        ENDF_energy.append(float(row[0]))
        ENDF_xsec.append(float(row[1]))

def gauss(x, H, A, x0, sigma):
    return H + A * np.exp(-(x - x0) ** 2 / (2 * sigma ** 2))

def gauss_fit(x, y):
    mean = sum(x * y) / sum(y)
    sigma = np.sqrt(sum(y * (x - mean) ** 2) / sum(y))
    popt, pcov = curve_fit(gauss, x, y, p0=[min(y), max(y), mean, sigma])
    return popt

@dataclass
class Neutron:
    """Class for storing neutron info from G4"""
    event:      int
    track_id:   int
    parent_id:  int
    t0:         float
    x0:         float
    y0:         float
    z0:         float
    k0:         float
    tf:         float
    xf:         float
    yf:         float
    zf:         float
    kf:         float
    track_l:    float=0
    vol0:       str=''
    volf:       str=''
    proc_f:     str=''
    num_inel:   int=0



class NeutronResults:
    """
    Class to hold and manipulate results from Geant4
    """
    def __init__(self,
        input_file: str='',
        consolidate_inelastics: bool=True,   # wether to combine inelastics with primaries
        mcnp_data:  str='',
    ):
        self.input_file = input_file
        self.neutrons = []
        self.number_of_primaries = 0
        self.number_of_inelastics = 0
        self.consolidated = consolidate_inelastics
        self.mcnp = False
        if mcnp_data != '':
            self.mcnp = True
            self.mcnp_lifetimes = []
            self.mcnp_displacement = []
            self.mcnp_distance = []
            self.mcnp_final_ke = []
            with open(mcnp_data, "r") as file:
                reader = csv.reader(file,delimiter=",")
                for row in reader:
                    self.mcnp_final_ke.append(float(row[0]))
                    self.mcnp_displacement.append(float(row[1]))
                    self.mcnp_distance.append(float(row[2]))
                    self.mcnp_lifetimes.append(float(row[3])) 
        self.map = {}
        # construct list of neutrons
        with open(input_file, "r") as file:
            reader = csv.reader(file, delimiter=",")
            for row in reader:
                if (int(row[3]) == 2112):
                    if (int(row[2]) == 0):
                        self.number_of_primaries += 1
                    else:
                        self.number_of_inelastics += 1
                    self.map[int(row[0]),int(row[1])] = len(self.neutrons)
                    self.neutrons.append(
                        Neutron(
                            event       = float(row[0]),
                            track_id    = float(row[1]),
                            parent_id   = float(row[2]),
                            t0  = float(row[6]),
                            x0  = float(row[7]),
                            y0  = float(row[8]),
                            z0  = float(row[9]),
                            k0  = float(row[10]),
                            tf  = float(row[13]),
                            xf  = float(row[14]),
                            yf  = float(row[15]),
                            zf  = float(row[16]),
                            kf  = float(row[17]),
                            track_l = float(row[18]),
                            vol0    = str(row[5]),
                            volf    = str(row[12]),
                            proc_f  = str(row[11])
                        )
                    )
        print("Loaded {} neutrons from file: {}".format(len(self.neutrons),self.input_file))
        print("{}/{} neutrons are primaries...".format(self.number_of_primaries,len(self.neutrons)))
        print("{}/{} neutrons are inelastics...".format(self.number_of_inelastics,len(self.neutrons)))
        if (self.consolidated):
            print("Consolidating inelastic scatter trajectories")
            consistency_check = 0
            for i in range(len(self.neutrons)):
                if self.neutrons[i].parent_id != 0:
                    # check for parent id
                    parent_index = self.find_neutron(self.neutrons[i].event, self.neutrons[i].parent_id)
                    if (parent_index != -1 and self.neutrons[parent_index].parent_id == 0):
                        consistency_check += 1
                        self.neutrons[parent_index].num_inel += 1
                        # add track information to primary
                        self.neutrons[parent_index].tf += (self.neutrons[i].tf - self.neutrons[i].t0)
                        self.neutrons[parent_index].track_l += self.neutrons[i].track_l
                        if (self.neutrons[parent_index].kf > self.neutrons[i].kf):
                            self.neutrons[parent_index].xf = self.neutrons[i].xf
                            self.neutrons[parent_index].yf = self.neutrons[i].yf
                            self.neutrons[parent_index].zf = self.neutrons[i].zf
                            self.neutrons[parent_index].kf = self.neutrons[i].kf
                            self.neutrons[parent_index].volf = self.neutrons[i].volf
                            self.neutrons[parent_index].proc_f = self.neutrons[i].proc_f
            print("Found {}/{} primaries for inelastics.".format(
                consistency_check,self.number_of_inelastics
            ))
    #----------------------------------------------------------------------
    def find_neutron(self,
        event_id:   int,
        track_id:   int,
    ):
        return self.map[event_id,track_id]
    #----------------------------------------------------------------------
    def hist_lifetimes(self,
        num_bins:       int=100,
        only_captures:  bool=True,
        energy_cut:     List[float]=[-1,-1],
        track_cut:      List[float]=[-1,-1],
        lifetime_cut:   List[float]=[-1,-1],
        title:          str='',
        save_plot:      str='',
        show_plot:      bool=False,
        mcnp:           bool=False,
    ):
        """
        Plot lifetime of the neutrons
        """
        new_title = "Neutron Capture lifetime (ns) in LAr"
        if (
            energy_cut[0] != -1 or 
            energy_cut[1] != -1 or 
            track_cut[0] != -1 or
            track_cut[1] != -1 or
            lifetime_cut[0] != -1 or
            lifetime_cut[1] != -1
        ): 
            new_title += "\n:"
        if (energy_cut[0] != -1): new_title += ">{}cm:".format(energy_cut[0])
        if (energy_cut[1] != -1): new_title += "<{}cm:".format(energy_cut[1])
        if (track_cut[0] != -1): new_title += ">{}cm:".format(track_cut[0])
        if (track_cut[1] != -1): new_title += "<{}cm:".format(track_cut[1])
        if (lifetime_cut[0] != -1): new_title += ">{}ns:".format(lifetime_cut[0])
        if (lifetime_cut[1] != -1): new_title += "<{}ns:".format(lifetime_cut[1])
        lifetimes = []
        for neutron in self.neutrons:
            if only_captures:
                if neutron.proc_f == 'nCapture' and neutron.parent_id == 0:
                    # check cuts
                    if (
                        (energy_cut[0] != -1 and neutron.kf < energy_cut[0]) or
                        (energy_cut[1] != -1 and neutron.kf > energy_cut[1]) or
                        (track_cut[0] != -1 and neutron.track_l < track_cut[0]) or
                        (track_cut[1] != -1 and neutron.track_l > track_cut[1]) or
                        (lifetime_cut[0] != -1 and (neutron.tf - neutron.t0) < lifetime_cut[0]) or
                        (lifetime_cut[1] != -1 and (neutron.tf - neutron.t0) > lifetime_cut[1])
                    ):
                        continue
                    lifetimes.append(neutron.tf - neutron.t0)
        fig, axs = plt.subplots()
        axs.hist(
            lifetimes, 
            bins=num_bins, 
            **hist_options, 
            label=self.input_file
        )
        if mcnp:
            axs.hist(
                self.mcnp_lifetimes, 
                bins=num_bins, 
                **hist_options, 
                label='mcnp'
            )
        axs.set_xlabel("lifetime [tf - t0] (ns)")
        if title == '':
            axs.set_title(new_title)
        else:
            axs.set_title(title)
        plt.legend()
        plt.tight_layout()
        if save_plot != '':
            plt.savefig(save_plot)
        if show_plot:
            plt.show()
        return lifetimes
    #----------------------------------------------------------------------
    def hist_tracklengths(self,
        num_bins:       int=100,
        only_captures:  bool=True,
        energy_cut:     List[float]=[-1,-1],
        track_cut:      List[float]=[-1,-1],
        lifetime_cut:   List[float]=[-1,-1],
        title:          str='',
        save_plot:      str='',
        show_plot:      bool=False,
        mcnp:           bool=False,
    ):
        """
        Plot lifetime of the neutrons
        """
        new_title = "Neutron Capture track length (cm) in LAr"
        if (
            energy_cut[0] != -1 or 
            energy_cut[1] != -1 or 
            track_cut[0] != -1 or
            track_cut[1] != -1 or
            lifetime_cut[0] != -1 or
            lifetime_cut[1] != -1
        ): 
            new_title += "\n:"
        if (energy_cut[0] != -1): new_title += ">{}cm:".format(energy_cut[0])
        if (energy_cut[1] != -1): new_title += "<{}cm:".format(energy_cut[1])
        if (track_cut[0] != -1): new_title += ">{}cm:".format(track_cut[0])
        if (track_cut[1] != -1): new_title += "<{}cm:".format(track_cut[1])
        if (lifetime_cut[0] != -1): new_title += ">{}ns:".format(lifetime_cut[0])
        if (lifetime_cut[1] != -1): new_title += "<{}ns:".format(lifetime_cut[1])
        tracklengths = []
        for neutron in self.neutrons:
            if only_captures:
                if neutron.proc_f == 'nCapture' and neutron.parent_id == 0:
                    if (
                        (energy_cut[0] != -1 and neutron.kf < energy_cut[0]) or
                        (energy_cut[1] != -1 and neutron.kf > energy_cut[1]) or
                        (track_cut[0] != -1 and neutron.track_l < track_cut[0]) or
                        (track_cut[1] != -1 and neutron.track_l > track_cut[1]) or
                        (lifetime_cut[0] != -1 and (neutron.tf - neutron.t0) < lifetime_cut[0]) or
                        (lifetime_cut[1] != -1 and (neutron.tf - neutron.t0) > lifetime_cut[1])
                    ):
                        continue
                    tracklengths.append(neutron.track_l)
        fig, axs = plt.subplots()
        axs.hist(
            tracklengths, 
            bins=num_bins, 
            **hist_options, 
            label=self.input_file
        )
        if mcnp:
            axs.hist(
                self.mcnp_distance, 
                bins=num_bins, 
                **hist_options, 
                label='mcnp'
            )
        axs.set_xlabel("track length (cm)")
        if title == '':
            axs.set_title(new_title)
        else:
            axs.set_title(title)
        plt.legend()
        plt.tight_layout()
        if save_plot != '':
            plt.savefig(save_plot)
        if show_plot:
            plt.show()
        return tracklengths
    #----------------------------------------------------------------------
    def hist_final_energy(self,
        num_bins:       int=100,
        only_captures:  bool=True,
        energy_cut:     List[float]=[-1,-1],
        track_cut:      List[float]=[-1,-1],
        lifetime_cut:   List[float]=[-1,-1],
        title:          str='',
        save_plot:      str='',
        show_plot:      bool=False,
        mcnp:           bool=False,
    ):
        """
        Plot lifetime of the neutrons
        """
        new_title = "Neutron Capture final kE (MeV) in LAr"
        if (
            energy_cut[0] != -1 or 
            energy_cut[1] != -1 or 
            track_cut[0] != -1 or
            track_cut[1] != -1 or
            lifetime_cut[0] != -1 or
            lifetime_cut[1] != -1
        ): 
            new_title += "\n:"
        if (energy_cut[0] != -1): new_title += ">{}cm:".format(energy_cut[0])
        if (energy_cut[1] != -1): new_title += "<{}cm:".format(energy_cut[1])
        if (track_cut[0] != -1): new_title += ">{}cm:".format(track_cut[0])
        if (track_cut[1] != -1): new_title += "<{}cm:".format(track_cut[1])
        if (lifetime_cut[0] != -1): new_title += ">{}ns:".format(lifetime_cut[0])
        if (lifetime_cut[1] != -1): new_title += "<{}ns:".format(lifetime_cut[1])
        final_energy = []
        for neutron in self.neutrons:
            if only_captures:
                if neutron.proc_f == 'nCapture' and neutron.parent_id == 0:
                    if (
                        (energy_cut[0] != -1 and neutron.kf < energy_cut[0]) or
                        (energy_cut[1] != -1 and neutron.kf > energy_cut[1]) or
                        (track_cut[0] != -1 and neutron.track_l < track_cut[0]) or
                        (track_cut[1] != -1 and neutron.track_l > track_cut[1]) or
                        (lifetime_cut[0] != -1 and (neutron.tf - neutron.t0) < lifetime_cut[0]) or
                        (lifetime_cut[1] != -1 and (neutron.tf - neutron.t0) > lifetime_cut[1])
                    ):
                        continue
                    final_energy.append(np.log10(neutron.kf))
        fig, axs = plt.subplots()
        axs.hist(
            final_energy, 
            bins=num_bins, 
            **hist_options, 
            label=self.input_file
        )
        if mcnp:
            axs.hist(
                np.log10(self.mcnp_final_ke), 
                bins=num_bins, 
                **hist_options, 
                label='mcnp'
            )
        energy_cut = [energy for energy in final_energy if (energy < LOG10_ANTIRESONANCE_PEAKS[0])]
        gauss_mean = round(np.mean(energy_cut))
        gauss_std  = np.std(energy_cut)
       
        ticks = list(axs.get_xticks())
        new_ticks = [gauss_mean,np.amin(ticks),np.amax(ticks),LOG10_ANTIRESONANCE]+LOG10_ANTIRESONANCE_PEAKS
        new_tick_labels = [str(gauss_mean),str(round(np.amin(ticks))),str(round(np.amax(ticks))),LOG10_ANTIRESONANCE_STR]+LOG10_ANTIRESONANCE_PEAKS_STR
        axs.set_xticks(new_ticks)
        axs.set_xticklabels(new_tick_labels,rotation='45',horizontalalignment='right')
        axs.set_xlabel(r"$\log_{10}(KE) [\log_{10}(MeV)]$")
        axs.set_xlim(-7.25,0.75)
        if title == '':
            axs.set_title(new_title)
        else:
            axs.set_title(title)
        plt.legend()
        plt.tight_layout()
        if save_plot != '':
            plt.savefig(save_plot)
        if show_plot:
            plt.show()
        return final_energy
     #----------------------------------------------------------------------

if __name__ == "__main__":
    neutron_results = NeutronResults(
        "output.csv",
        consolidate_inelastics=True,
        mcnp_data = 'mcnp_statistics.csv'
    )

    neutron_results.hist_lifetimes(
        save_plot="../plots/lifetimes_compare.png",
        mcnp=True
    )
    neutron_results.hist_tracklengths(
        save_plot="../plots/tracklength_compare.png",
        mcnp=True
    )
    total_energy = neutron_results.hist_final_energy(
        save_plot="../plots/final_energy_compare.png",
        mcnp=True
    )

    # neutron_results.hist_final_energy(
    #     track_cut = [-1,50000],
    #     save_plot="../plots/final_energy<50m.png"
    # )

    # neutron_results.hist_final_energy(
    #     track_cut = [50000,-1],
    #     save_plot="../plots/final_energy>50m.png"
    # )

    # neutron_results.hist_lifetimes(
    #     track_cut = [-1,50000],
    #     save_plot="../plots/lifetimes<50m.png"
    # )

    # neutron_results.hist_lifetimes(
    #     track_cut = [50000,-1],
    #     save_plot="../plots/lifetimes>50m.png"
    # )

    # neutron_results.hist_final_energy(
    #     track_cut = [175000,-1],
    #     save_plot="../plots/final_energy>175m.png"
    # )

    # energy_cut = neutron_results.hist_final_energy(
    #     track_cut = [-1, 175000],
    #     save_plot="../plots/final_energy<175m.png"
    # )
