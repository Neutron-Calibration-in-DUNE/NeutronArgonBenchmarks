from dataclasses import dataclass
import numpy as np
import csv
import matplotlib.pyplot as plt


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



class NeutronResults:

    def __init__(self,
        input_file: str=''
    ):
        self.input_file = input_file
        self.neutrons = []
        # construct list of neutrons
        with open(input_file, "r") as file:
            reader = csv.reader(file, delimiter=",")
            for row in reader:
                if (int(row[3]) == 2112):
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
    
    def hist_lifetimes(self,
        num_bins:       int=100,
        only_captures:  bool=True,
    ):
        """
        Plot lifetime of the neutrons
        """
        lifetimes = []
        for neutron in self.neutrons:
            if only_captures:
                if neutron.proc_f == 'nCapture' and neutron.parent_id == 0:
                    lifetimes.append(neutron.tf - neutron.t0)
        fig, axs = plt.subplots()
        axs.hist(
            lifetimes, 
            bins=num_bins, 
            histtype='step', 
        )
        axs.set_xlabel("lifetime [tf - t0] (ns)")
        axs.set_title("Neutron lifetime in LAr - {}".format(self.input_file))
        plt.savefig("lifetime.png")
        #plt.show()
    
    def hist_final_energy(self,
        num_bins:       int=100,
        only_captures:  bool=True,
    ):
        """
        Plot lifetime of the neutrons
        """
        final_energy = []
        for neutron in self.neutrons:
            if only_captures:
                if neutron.proc_f == 'nCapture' and neutron.parent_id == 0:
                    final_energy.append(np.log10(neutron.kf))
        fig, axs = plt.subplots()
        axs.hist(
            final_energy, 
            bins=num_bins, 
            histtype='step', 
        )
        extra_ticks = [np.log10(.057)]
        axs.set_xticks(axs.get_xticks() + extra_ticks)
        axs.set_xlabel(r"$\log_{10}(KE)$ (keV)")
        axs.set_title("Neutron capture energy in LAr - {}".format(self.input_file))
        plt.savefig("final_energy.png")
        #plt.show()


if __name__ == "__main__":
    neutron_results = NeutronResults("output.csv")
    neutron_results.hist_lifetimes()
    neutron_results.hist_final_energy()
