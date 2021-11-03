# Argon40 and natural Argon Calibration Benchmarks

Here we will conduct a proof of principle simulation in which our "detector" consists of an effectively infinite sphere of LAr, either with pure Argon40 or Argon36/38/40 in their respective abundances.  We will simulate neutrons one at a time (i.e. one neutron per event), with some initial kinetic energy, using the Geant4 framework with some modifications to the physics processes.  We will then analyze the results using some custom software which is used throughout the rest of the calibration framework.  The simulation consists of several steps, 

1.  Constructing the input file to the generator (HEPevt format)
2.  Running a Geant4 simulation step
3.  Various python scripts for analyzing the results and making plots

