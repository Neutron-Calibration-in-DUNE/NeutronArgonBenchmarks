#! /bin/bash
# number of cores to use for installing
NUM_CORES=16

# check if certain packages are installed
for DEP in "cmake" "gcc" "g++" "libexpat1-dev" "qt5-default" "libxmu-dev" "libmotif-dev"
do
    echo "Checking for $DEP package..."
    if [ $(dpkg-query -W -f='${Status}' $DEP 2>/dev/null | grep -c "ok installed") -eq 0 ];
    then
        echo "$DEP not installed, please re-run scripts/'prepare_geant4_installation.sh'"
        return;
    fi
done

# get the directory where this script is stored
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
NEUTRON_ARGON_BENCHMARK_DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )/../"

# directory for the installation
VERSION="10.07.p02"
DL_DIRECTORY="/home/$USER/physics/geant4/geant4."$VERSION
BUILD_DIRECTORY=$DL_DIRECTORY"/build"
INSTALL_DIRECTORY=$DL_DIRECTORY"-install"

# create the build and installation directories
mkdir -p $INSTALL_DIRECTORY
mkdir -p $BUILD_DIRECTORY
cd $BUILD_DIRECTORY

# run cmake with the required arguments
cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_DIRECTORY -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_GDML=ON -DGEANT4_USE_QT=ON -DGEANT4_USE_SYSTEM_EXPAT=ON ..
# run make with as many cores as you can
make -j $NUM_CORES
# install 
make install
# source the geant4 install script
INSTALL_SCRIPT=$INSTALL_DIRECTORY"/share/Geant4*/geant4make/"
source $INSTALL_SCRIPT"geant4make.sh"

cd $NEUTRON_ARGON_BENCHMARK_DIR