#!/bin/bash 
# Hack Edger onto a Linux system. Currently just handles Ubuntu.

# Here is an important todo: This script needs to detect changes to the edger repo and rebuild ant and aardvark as needed

# The Espressif docs have the recipes for package installs for other Linux flavors

usage() {
  cat <<EOF
$0 [--with-edger=<edger-path>] [--with-esp-idf=<esp-idf-path>]
  --with-edger=<edger-path> (optional) 
    use/install edger at this (absolute) path
  --with-esp-idf=<esp-idf-path> (optional) 
    use/install esp-idf at this (absolute) path
EOF
}

# Process command line arguments
while [[ $# -gt 0 ]]; do
  case $1 in
    --with-edger=*)
      EDGER_DIR=${1#*=} 
      echo "edger = $EDGER_DIR"
      ;;
    --with-esp-idf=*)
      ESP_IDF_DIR=${1#*=} 
      echo "esp-idf = $ESP_IDF_DIR"
      ;;
    --esp-idf-branch=*)
      ESP_IDF_BRANCH=${1#*=}
      echo "esp-idf-branch = $ESP_IDF_BRANCH"
      ;;
    --help)
      usage
      exit 0
      ;;
    *)
      echo "unrecognized argument '$1'" 
      ;;
  esac
  shift
done

# Check arguments and set defaults if needed
if [[ -z $EDGER_DIR ]]; then
  EDGER_DIR=$HOME/workspace/esp32/edger
else
  if [[ ${EDGER_DIR:0:1} != "/" ]]; then
    echo "--with-edger must be specified as absolute path"
    arg_error=1
  fi
fi
if [[ -z $ESP_IDF_DIR ]]; then
  ESP_IDF_DIR=$HOME/esp/esp-idf
else
  if [[ ${ESP_IDF_DIR:0:1} != "/" ]]; then
    echo "--with-esp-idf must be specified as absolute path"
    arg_error=1
  fi
fi
if [[ -z $ESP_IDF_BRANCH ]]; then
  ESP_IDF_BRANCH=v4.4
fi

if [[ $arg_error = "1" ]]; then
  echo "encountered errors processing argument(s)"
  exit 1
fi

echo "==> Install the prerequisite packages"

sudo apt-get install -q -q -y git wget flex bison gperf python3 python3-venv cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0 python3-pip curl

echo "==> checking/cloning Edger repo"
# Fetch the Edger repo
if [[ -d $EDGER_DIR ]] ; then
  cd $EDGER_DIR
  branch=$(git branch)
  if [[ $? -eq 0 ]]; then
    echo "edger dir $EDGER_DIR is a git repo on branch $branch"
  fi
  if [[ ! -d $EDGER_DIR/ant ]]; then
    echo "edger dir $EDGER_DIR has no ant subdirectory. Is it an edger repo?"
  fi
else
  mkdir -p $(dirname $EDGER_DIR)
  if [[ ! -w $(dirname $EDGER_DIR) ]]; then
    echo "$(dirname $EDGER_DIR) is not writeable. Unable to clone edger to $EDGER_DIR"
    exit 1
  fi
  echo "==> Fetch edger repo"
  cd $(dirname $EDGER_DIR)
  git clone https://github.com/TriEmbed/edger.git $(basename $EDGER_DIR)
  if [[ $? -ne 0 ]]; then
    echo "nonzero exit cloning edger repo to $EDGER_DIR"
    exit 1
  fi
fi

# Copy bin scripts
echo "==> creating/updating scripts in $HOME/bin"
mkdir -p $HOME/bin
for script in $(ls $EDGER_DIR/tools/thumbdrive/home/bin); do
  echo "  $script"
  sed "s:\$HOME/workspace/esp32/edger:$EDGER_DIR:
       s:\$HOME/esp/esp-idf:$ESP_IDF_DIR:" \
    $EDGER_DIR/tools/thumbdrive/home/bin/$script > $HOME/bin/$script
  chmod u+x $HOME/bin/$script
done

# Copy desktop files
if [[ -d $HOME/Desktop ]]; then
  echo "==> copying icons to $HOME/Desktop"
  sed -e"s@\$HOME@$HOME@" <$EDGER_DIR/tools/thumbdrive/home/Desktop/changewifi.desktop >$HOME/Desktop/changewifi.desktop
  sed -e"s@\$HOME@$HOME@" <$EDGER_DIR/tools/thumbdrive/home/Desktop/startaardvark.desktop >$HOME/Desktop/startaardvark.desktop
  sed -e"s@\$HOME@$HOME@" <$EDGER_DIR/tools/thumbdrive/home/Desktop/startbrowser.desktop >$HOME/Desktop/startbrowser.desktop
else
  echo "$HOME/Desktop does not exist. Skipping addition of desktop icons."
  echo "Use $HOME/bin/changewifi and $HOME/bin/startaardvark directly" 
  echo "Once running, aardvark can be accessed at port 8080 on the local device"
fi
  
echo "==> checking for $HOME/bin in path"
grep '$HOME/bin' $HOME/.bashrc | grep -q PATH=
if [ $? -ne 0 ] ; then
  echo '$HOME/bin is not in your search rules in .bashrc: adding PATH=\"$PATH:$HOME/bin'
  echo 'export PATH="$PATH:$HOME/bin"' >>$HOME/.bashrc
fi

echo $PATH | grep -q "$USER/bin"
if [ $? -ne 0 ] ; then
  export PATH=$PATH:$HOME/bin
fi

echo "==> checking if user is in dialout group"
groups | grep -q "dialout" 
if [ $? -ne 0 ] ; then
  echo "==> Adding $USER to dialout group"
  sudo addgroup $USER dialout
  echo "log out and back in to be in dialout group and then run this script again"
  exit 0
fi

echo "==> checking/cloning esp-idf repo"
if [[ -d $ESP_IDF_DIR ]] ; then
  cd $ESP_IDF_DIR
  branch=$(git branch)
  if [[ $? -eq 0 ]]; then
    echo "esp-idf dir $ESP_IDF_DIR is a git repo on branch $branch"
  fi
  if [[ ! -f $ESP_IDF_DIR/install.sh ]]; then
    echo "esp-idf dir $ESP_IDF_DIR has no install.sh. Is it an esp-idf repo?"
  fi
else
  mkdir -p $(dirname $ESP_IDF_DIR)
  if [[ ! -w $(dirname $ESP_IDF_DIR) ]]; then
    echo "$(dirname $ESP_IDF_DIR) is not writeable. Unable to clone esp-idf to $ESP_IDF_DIR"
    exit 1
  fi
  echo "==> Fetch the Espressif IDF"
  cd $(dirname $ESP_IDF_DIR)
  git clone -b $ESP_IDF_BRANCH --recursive https://github.com/espressif/esp-idf.git $(basename $ESP_IDF_DIR)
fi

echo "==> checking/installing nvm/npm/pnpm"
if [ ! -d $HOME/.nvm ] ; then
  curl https://raw.githubusercontent.com/creationix/nvm/master/install.sh 2>/dev/null | bash >/tmp/log 2>&1
  cat /tmp/log
  export NVM_DIR=$HOME/.nvm
  . $NVM_DIR/nvm.sh
  nvm install 14
  if [ $? -ne 0 ] ; then
    echo "fatal error: nvm/npm install failed"
    echo "type return to continue"
    exit 1
  fi
  npm add -g @pnpm/exe
  if [ $? -ne 0 ] ; then
    echo "fatal error: pnpm install failed"
    echo "type return to continue"
    exit 1
  fi
fi

export NVM_DIR="$HOME/.nvm"
[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"  # This loads nvm
[ -s "$NVM_DIR/bash_completion" ] && \. "$NVM_DIR/bash_completion"  # This loads nvm bash_completion

# Aardvark is currently prepared from scratch every time so it picks up 
# changes to the repo. This could be made smarter by comparing the git log
# with file mod dates.

cd $EDGER_DIR/aardvark
echo "==> building aardvark"
pnpm install
if [ $? -ne 0 ] ; then
  echo "pnpm install failed"
  exit 1
fi
pnpm run build
if [ $? -ne 0 ] ; then
  echo "pnpm build failed"
  exit 1
fi

# install the IDF
cd $ESP_IDF_DIR
if [ ! -d $HOME/.espressif ] ; then
  echo "==> Installing ESP IDF"
  bash ./install.sh
fi

echo "==> checking for $ESP_IDF_DIR in .bashrc"
grep -q 'idfexport()' $HOME/.bashrc
if [ $? -eq 0 ]; then
  grep 'idfexport()' $HOME/.bashrc | grep -q $ESP_IDF_DIR/export.sh
  if [ $? -ne 0 ]; then
    echo "==> updating idf export path in $HOME/.bashrc"
    sed -i "s:^idfexport.*:idfexport() { source $ESP_IDF_DIR/export.sh; }:" $HOME/.bashrc
  fi
else
  echo "==> adding ESP export.sh to .bashrc"
  echo "idfexport() { source $ESP_IDF_DIR/export.sh; }" >> $HOME/.bashrc
fi

if [[ $_ != $0 ]]; then
  sourced=1
  . $ESP_IDF_DIR/export.sh
fi

echo "Now use the change wifi icon to customize your dev board. It must be plugged in for this."
echo "Then use the start aardvark icon followed by the start browser icon to run Edger"
if [[ "$sourced" != "1" ]]; then
  echo "idf.py can be used directly in this shell. To use in future first type idfexport"
else
  echo "To use idf.py directly, first launch a new shell and type idfexport"
fi
