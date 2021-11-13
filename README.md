# Argon40 and natural Argon Calibration Benchmarks

Here we will conduct a proof of principle simulation in which our "detector" consists of an effectively infinite sphere of LAr, either with pure Argon40 or Argon36/38/40 in their respective abundances.  We will simulate neutrons one at a time (i.e. one neutron per event), with some initial kinetic energy, using the Geant4 framework with some modifications to the physics processes.  We will then analyze the results using some custom software which is used throughout the rest of the calibration framework.  The simulation consists of several steps, 

1.  Constructing the input file to the generator (HEPevt format)
2.  Running a Geant4 simulation step
3.  Various python scripts for analyzing the results and making plots

## Installation
-----------------
There are several scripts included with the git respository that will automate the installation process.  Once downloaded, go to the scripts folder
```shell
git clone https://github.com/Neutron-Calibration-in-DUNE/NeutronArgonBenchmarks.git
cd NeutronArgonBenchmarks/scripts
```
then run the Geant4 preprocessing step, which will install required packages such as *cmake, gcc, g++, expat, qt5, xmu and motif*:
```shell
source prepare_geant4_installation.sh
```
it will also download a version of geant4 and install it in the directory */home/$USER/physics/geant4/*.  The next step is to actually install geant4, which can be done with:
```shell
source install_geant4.sh
```
Finally, to compile the argon sphere code you can run the following script:
```shell
source install_argon_sphere.sh
```
### Manual Installation
-----------------------
If for some reason the installation scripts do not work, or you wish to install Geant4 manually, the following list of steps should work on Ubuntu 20.XX.
* **Install Dependencies** - First you will need a few packages, such as QT in order to use the visualization tools,
  ```shell
  sudo apt-get install cmake gcc g++ libexpat1-dev qt5-default libxmu-dev libmotif-dev
  ```
* **Get the code** - Now choose a place in which you want to download the Geant4 code.  The following should work:
  ```shell
  wget "http://cern.ch/geant4-data/releases/geat4.10.07.p02.tar.gz"
  tar xvfz geant4.10.07.p02.tar.gz
  rm geant4.10.07.p02.tar.gz
  ```
* **Install Geant4** - The easiest way to install geant4 once you have it downloaded, is to use ccmake, which you may have to download using
  ```shell
  sudo apt-get install cmake-curses-gui
  ```
  Next, you should create a build directory inside of the folder where the source code is for building the code:
  ```shell
  cd geant4.10.07.p02
  mkdir build && cd build
  ```
  Then just run ccmake, which will bring up a cmake GUI
  ```shell
  ccmake ..
  ```
  From here, there are several things that you need to enable, such as **GEANT4_INTALL_DATA=ON**, **GEANT4_USE_QT=ON** and **GEANT4_USE_SYSTEM_EXPAT=ON**.  You also need to specify the installation directory, which in the scripts is set to */home/$USER/physics/geant4.10.07.p02-install/*.  If you are not using ccmake, but instead just running cmake from the build directory, you can do this with the following:
  ```shell
  cmake -DCMAKE_INSTALL_PREFIX=<install_dir> -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_QT=ON -DGEANT4_USE_SYSTEM_EXPAT=ON ..
  ```
  If cmake is successful, you can then build the project by issuing
  ```shell
  make -j N
  make install
  ```
  where *N* is the number of cores you want to use to build.
* **Post-insallation** - Once installed, you will need to run a setup script to export the right environment variables.  This can be found in the folder *<install_dir>/share/Geant4\*/geant4make/*:
  ```shell
  source <install_dir>/share/Geant4\*/geant4make/geant4make.sh
  ```
  
