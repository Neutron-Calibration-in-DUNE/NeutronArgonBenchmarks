#! /bin/bash

# check if certain packages are installed
LIST_OF_APPS="cmake gcc g++ libexpat1-dev qt5-default libxmu-dev libmotif-dev"
sudo apt-get install $LIST_OF_APPS -y

# get the directory where this script is stored
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
NEUTRON_ARGON_BENCHMARK_DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null 2>&1 && pwd )/../"

INSTALL_DIRECTORY="/home/$USER/physics/geant4/"
DOWNLOAD_PAGE="http://cern.ch/geant4-data/releases/"
VERSION="10.07.p02"
DL_FILE="geant4."$VERSION".tar.gz"
DL_LINK=$DOWNLOAD_PAGE$DL_FILE

mkdir -p $INSTALL_DIRECTORY
cd $INSTALL_DIRECTORY

wget $DL_LINK
tar xvzf $DL_FILE
rm $DL_FILE

cd $NEUTRON_ARGON_BENCHMARK_DIR
