#!/bin/bash
# Hack Edger onto a Linux system. Currently just handles Ubuntu.

# Here is an important todo: This script needs to detect changes to the edger repo and rebuild ant and aardvark as needed

# The Espressif docs have the recipes for package installs for other Linux flavors

echo "====="
echo $PATH $HOME/.bashrc | grep "$USER/bin"
echo "====="

if [ $? -ne 0 ] ; then
  echo '$HOME/bin is not in your search rules in .bashrc: adding PATH=\"$PATH:$HOME/bin'
  echo 'export PATH="$PATH:$HOME/bin' >>$HOME/.bashrc
  echo "run this script again"
  exit 0
fi

groups | grep " dialout" 
if [ $? -ne 0 ] ; then
  echo "====="
  echo "Adding $USER to dialout"
  echo "====="
  sudo addgroup $USER dialout
  echo "log out and back in and then run this script again to be in dialout"
  exit 0
fi

echo "====="
echo "Adding desktop icons and scripts to $HOME"
echo "====="

cd $HOME
cp -r $HOME/workspace/esp32/edger/tools/thumbdrive/home/* .
if [ ! -d $HOME/esp/esp-idf ] ; then
  echo "====="
  echo "Fetch the Espressif IDF"
  echo "====="
  cd $HOME/esp
  git clone -b v4.4 --recursive https://github.com/espressif/esp-idf.git
fi

echo "====="
echo "Install the prerequsite packages"
echo "====="

sudo apt-get install -y git wget flex bison gperf python3 python3-venv cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0 python3-pip curl
# Fetch the Edger repo
if [ ! -d $HOME/workspace/esp32/edger ] ; then
  echo "====="
  echo "Fetch edger repo"
  echo "====="
  cd $HOME/workspace/esp32
  git clone https://github.com/TriEmbed/edger.git
fi

if [ ! -d $HOME/workspace ] ; then
  echo "====="
  echo "Set up the home directory"
  echo "====="
  cd $HOME
  mkdir -p bin workspace workspace/esp32 esp
fi

if [ ! -d $HOME/.nvm ] ; then
  echo "====="
  echo "Installing nvm/npm/pnpm"
  echo "====="
  curl https://raw.githubusercontent.com/creationix/nvm/master/install.sh 2>/dev/null | bash >/tmp/log 2>&1
  cat /tmp/log
  export NVM_DIR=$HOME/.nvm
  . $NVM_DIR/nvm.sh
  nvm install 14
  npm add -g @pnpm/exe
fi

# Aardvark is currently prepared from scratch every time so it picks up 
# changes to the repo. This could be made smarter by comparing the git log
# with file mod dates.

cd $HOME/workspace/esp32/edger/aardvark
echo "====="
echo "Build aardvark"
echo "====="
pnpm install
pnpm run build
# install the IDF
cd $HOME/esp/esp-idf
if [ ! -d $HOME/.espressif ] ; then
  echo "====="
  echo "Installing IDF"
  echo "====="
  . ./install.sh
fi
echo "====="
echo "setup build environment to make sure it's ready"
echo "====="
. ./export.sh
echo "Now use the change wifi icon to customize your dev board. It must be plugged in for this."
echo "Then use the start aardvark icon followed by the start browser icon to run Edger"
