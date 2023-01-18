#!/bin/bash -x
# This script is UNTESTED
#
# Create an Edger environment on an Ubuntu Linux system
# Carl: the Espressif docs have the recipes for package installs for other Linux flavors
# First Set up the home directory. The actual directory paths should be parametric later
cd $HOME
# Perhaps $HOME/.local/bin is the kosher place?
# An available bin directory should be specified by the user if possible
mkdir bin workspace workspace/esp32 esp
echo "if $HOME/bin is not in your search rules you must add it in .bashrc with export PATH=\"$PATH:$HOME/bin\""
echo "type enter to continue"
read ans
# Add user to dialout
sudo addgroup $USER dialout
echo "after this script runs you must log out and back in to be sure to be in the dialout group"
echo "type enter to continue"
read ans
# Fetch the Espressif IDF
cd $HOME/esp
git clone -b v4.4 --recursive https://github.com/espressif/esp-idf.git
# Fetch the Edger repo
cd $HOME/workspace/esp32
git clone https://github.com/TriEmbed/edger.git
# Add desktop icons and scripts
cd $HOME
cp -r $HOME/workspace/esp32/edger/tools/thumbdrive/home/bin .
cp -r $HOME/workspace/esp32/edger/tools/thumbdrive/home/Desktop .
# Install the prerequsite packages
sudo apt-get install -y git wget flex bison gperf python3 python3-venv cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0 python3-pip curl
# Install javascript stuff
curl https://raw.githubusercontent.com/creationix/nvm/master/install.sh 2>/dev/null | bash >/tmp/log 2>&1
cat /tmp/log
export NVM_DIR=$HOME/.nvm
. $NVM_DIR/nvm.sh
nvm install 14
npm add -g @pnpm/exe
cd $HOME/workspace/esp32/edger/aardvark
echo "You may have to arrange search rules for pnpm here"
echo "type enter to continue"
read ans
pnpm install
# install the IDF
cd $HOME/esp/esp-idf
. ./install.sh
echo "confirm the install worked OK then type enter"
read ans
# set up build environment
. ./export.sh
echo "confirm the export worked OK then type enter"
read ans
echo "Now log out and back in as needed to get into the dialout group."
echo "Then use the change wifi icon to customize your dev board. It must be plugged in for this."
echo "Then use the start aardvark icon followed by the start browser icon to run Edger"
