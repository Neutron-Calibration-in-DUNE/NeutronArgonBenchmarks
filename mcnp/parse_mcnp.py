"""
Code for analyzing MCNP output
"""
import numpy as np
import scipy as sp
import matplotlib.pyplot as plt
import math
import csv

mass_neutron = 939.56542052
def parse_mcnp(
    input_file,
    output_file,
):
    data = []
    with open(input_file, "r") as file:
        reader = csv.reader(file, delimiter=" ")
        next(reader)
        next(reader)
        event = -1
        dist = 0
        x, y, z, t = 0, 0, 0, 0
        px, py, pz = 0, 0, 0
        xi, yi, zi, ti = 0, 0, 0, 0
        pxi, pyi, pzi = 0, 0, 0
        track_id = 0
        end = False
        for row in reader:
            if (int(row[7]) == 2112):
                if (int(row[8]) == event):
                    if (int(row[9]) == track_id):
                        dist += np.sqrt(
                            (x - float(row[0]))*(x - float(row[0])) + 
                            (y - float(row[1]))*(y - float(row[1])) + 
                            (z - float(row[2]))*(z - float(row[2]))
                        )
                    else:
                        print(f'Different track ids: {track_id} and {int(row[9])}')
                else:
                    end = False
                    event = int(row[8])
                    track_id = int(row[9])
                    dist = 0
                    xi, yi, zi, ti = float(row[0]), float(row[1]), float(row[2]), float(row[6])
                    pxi, pyi, pzi = float(row[3]), float(row[4]), float(row[5])
                x, y, z, t = float(row[0]), float(row[1]), float(row[2]), float(row[6])
                px, py, pz = float(row[3]), float(row[4]), float(row[5])
            else:
                if end == False:
                    end = True
                    displacement = np.sqrt(
                            (xi - x)*(xi - x) + 
                            (yi - y)*(yi - y) + 
                            (zi - z)*(zi - z)
                    )
                    final_ke = (.5/mass_neutron)*(px*px + py*py + pz*pz)
                    data.append([final_ke, displacement, dist, t])

    with open(output_file, "w") as file:
        writer = csv.writer(file, delimiter=",")
        writer.writerows(data)

def import_statistics(
    input_file
):
    lifetime = []
    displacement = []
    distance = []
    final_ke = []
    with open(input_file, "r") as file:
        reader = csv.reader(file,delimiter=",")
        for row in reader:
            final_ke.append(float(row[0]))
            displacement.append(float(row[1]))
            distance.append(float(row[2]))
            lifetime.append(float(row[3]))
    return final_ke, displacement, distance, lifetime
