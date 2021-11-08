#! /bin/bash
# number of cores to use for installing
NUM_CORES=16

# get the directory where this script is stored
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
NEUTRON_ARGON_BENCHMARK_DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )/../"

# directory for the geant4 installation
VERSION="10.07.p02"
DL_DIRECTORY="/home/$USER/physics/geant4/geant4."$VERSION
INSTALL_DIRECTORY=$DL_DIRECTORY"-install"
# source the geant4 install script
INSTALL_SCRIPT=$INSTALL_DIRECTORY"/share/Geant4*/geant4make/"
source $INSTALL_SCRIPT"geant4make.sh"

ARGON_BUILD_DIRECTORY=$NEUTRON_ARGON_BENCHMARK_DIR"/build"
mkdir -p $ARGON_BUILD_DIRECTORY
cd $NEUTRON_ARGON_BENCHMARK_DIR"/build"
cmake ..
make -j $NUM_CORES

# cd $NEUTRON_ARGON_BENCHMARK_DIR
