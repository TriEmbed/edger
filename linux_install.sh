#!/bin/bash 
# Hack Edger onto a Linux system. Currently just handles Ubuntu.

# Here is an important todo: This script needs to detect changes to the edger repo and rebuild ant and aardvark as needed

# The Espressif docs have the recipes for package installs for other Linux flavors

## default values for optional arguments
EDGER_DIR_DEFAULT=$HOME/workspace/esp32/edger
IDF_DIR_DEFAULT=$HOME/esp/esp-idf
IDF_BRANCH_DEFAULT=v4.4

usage() {
  cat <<EOF
$0 --install [--edger=<edger-path>] [--idf=<esp-idf-path>] [--idf-branch=<ESP IDF branch>]
  --install (required or usage is printed and no changes made)
  --edger=<edger-path> or --edger <edger-path> (optional) 
  --idf=<esp-idf-path> or --idf <idf-path> (optional) 
  --idf-branch=<idf-git-branch> (optional)

When run with the --install argument, this script will do the following:
1. Check for (and if non-existent clone) the Edger git repo
  * in $EDGER_DIR_DEFAULT or directory specified with --edger
2. Check for (and if non-existent clone) the ESP IoT Dev Framework (IDF) repo
  * in $IDF_DIR_DEFAULT or directory specified with --idf
  * clones the $IDF_BRANCH_DEFAULT branch unless specified with --idf-branch
3. Install node.js and pnpm (in standard places)
4. Use pnpm to build aardvark
5. Run the ESP IDF Installer (creates \$HOME/.espressif)
6. Copy changewifi and startaardvark scripts to \$HOME/bin
7. Copy icon files to \$HOME/Desktop, if it exists
8. Make the following changes to \$HOME/.bashrc
  * add a line that appends \$HOME/bin to \$PATH, if not there already
  * add a function "exportidf" that will source the idf export.sh 
    (after which the idf.py command can be run)

EOF
}

# save to pass to sub-installers
ARGS=$@

# Process command line arguments
while [[ $# -gt 0 ]]; do
  case $1 in
    --edger*)
      # remove characters from $1 up to =
      EDGER_DIR=${1#*=}  
      # no = in argument - get next arg for value
      if [[ $EDGER_DIR = $1 ]]; then 
        shift
        EDGER_DIR=$1
      fi
      echo "edger = $EDGER_DIR"
      ;;
    --idf*)
      IDF_DIR=${1#*=} 
      if [[ $IDF_DIR = $1 ]]; then
        shift
        IDF_DIR=$1
      fi
      echo "esp-idf = $IDF_DIR"
      ;;
    --idf-branch*)
      IDF_BRANCH=${1#*=}
      if [[ $IDF_BRANCH = $1 ]]; then
        shift
        IDF_BRANCH=$1
      fi
      echo "esp-idf-branch = $IDF_BRANCH"
      ;;
    --install)
      INSTALL='true'
      ;;
    *)
      echo "unrecognized argument '$1'" 
      ;;
  esac
  shift
done

# Check arguments and set defaults if needed
if [[ -z $EDGER_DIR ]]; then
  EDGER_DIR=$EDGER_DIR_DEFAULT
else
  if [[ ${EDGER_DIR:0:1} != "/" ]]; then
    echo "--edger must be specified as absolute path"
    arg_error=1
  fi
fi
if [[ -z $IDF_DIR ]]; then
  IDF_DIR=$IDF_DIR_DEFAULT
else
  if [[ ${IDF_DIR:0:1} != "/" ]]; then
    echo "--idf must be specified as absolute path"
    arg_error=1
  fi
fi
if [[ -z $IDF_BRANCH ]]; then
  IDF_BRANCH=$IDF_BRANCH_DEFAULT
fi

if [[ $arg_error = "1" ]]; then
  echo "encountered errors processing argument(s)"
  echo "run without arguments to see usage info"
  exit 1
fi

if [[ $INSTALL != 'true' ]]; then
  usage
  exit 0
fi

echo "==> Install the prerequisite packages"

sudo apt-get install -q -q -y git curl
#sudo apt-get install -q -q -y git wget flex bison gperf python3 python3-venv cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0 python3-pip curl

echo "==> checking/cloning Edger repo in $EDGER_DIR"
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
       s:\$HOME/esp/esp-idf:$IDF_DIR:" \
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


echo "==> install Ant"
$EDGER_DIR/ant/linux_install.sh $ARGS
if [[ $? -ne 0 ]]; then
  exit
fi

echo "==> install Aardvark"
$EDGER_DIR/aardvark/linux_install.sh $ARGS
if [[ $? -ne 0 ]]; then
  exit
fi

## print final message
echo "Now use the change wifi icon to customize your dev board. It must be plugged in for this."
echo "Then use the start aardvark icon followed by the start browser icon to run Edger"
if [[ "$sourced" != "1" ]]; then
  echo "idf.py can be used directly in this shell. To use in future first type idfexport"
else
  echo "To use idf.py directly, first launch a new shell and type idfexport"
fi
