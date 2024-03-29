#!/bin/bash 

# Read the temperature from a BMP085/BMP180 sensor connected to the I2C bus of 
# an Edger development board via its HTTP endpoint for I2C bus access and
# display it as fahrenheit every $WAIT_TIME seconds (delay can be changed 
# below). The Edger dev board only needs power: it can be powered via a USB
# connection for convenience: it is not dependent on any communication with
# the PC (that is handled via WIFI).

# Note the code is slightly simplified to use a mode of "ultra low power" so
# the raw temp data can be fetched as a short vs as a three byte integer
# that has to be shifted down to average from the oversampling.

# Tested with an Ubuntu 20.04.04 laptop and a rev 2 "white" dev board. NOTE
# that the dev board has to be set up for shared WIFI connection to the PC.

# This shell version was written for fun. It's the wrong language for this
# kind of application. See the .py files for better alternatives.

# TheBMP180/085 turned out to be a poor choice, as it is no longer made.
# Worse, the 180 example used with this code appears to be defective with
# respect to pressure measurements. 

# LIMITATIONS
# 1) The URLs are hardwired to reference the first dev board by default. To 
#    work around this pass a specific dev board name in as an argument to the 
#    command (e.g. "edgerBMP esp-home2").
# 2) The networking code in the ESP32 is brittle and doesn't tolerate abrupt
#    power cycles. They can cause curl errors and have been seen to cause the 
#    system to hang so nothing comes out after. The workaround is to follow 
#    the advice of the IT Crowd and "turn it off and back one again".

# pete@soper.us Feb 18, 2023 Creative Commons CC0 1.0 Universal license

# Coding conventions: 
#  Constants are all upper case with underscore word delimiters.
#  Variable and parameter names are lower case with underscore word delimeters.
#  Functions names use camelCase.

###### Start user customization section
# Seconds between temp displays

WAIT_TIME=5
###### End ordinary customization section

# Check for an alternate name from the command line

if [ $# -gt 0 ] ; then
  host="$1"
else
  host="esp-home"
fi

# Sensor configuration. These values are BMP085/180 specific.

# To customize this for a different sensor the sensor's datasheet or an 
# existing example of drive code has to be consulted to determine the I2C
# register set and the semantics of interaction with the device. These details
# will determine the pattern of HTTP set and get operations (done as a "patch")
# to accomodate the specific application or prototype/demo functionality.

# The Sensor I2C addresses

# The default I2C address of the sensor

BMP_I2C_ADDR=119          # 0x77

# Sensor register addresses/offsets relevant to temperature

CAL_AC5_REG=178           # 0xB2
CAL_AC6_REG=180           # 0xB4
CAL_MC_REG=188            # 0xBC
CAL_MD_REG=190            # 0xBE
BMP_CONTROL=244           # 0xF4
BMP_TEMP_DATA=246         # 0xF6

# Sensor commands

BMP_READ_TEMP=46          # 0x2E

# Define a trap handler for catching process interrupts via control-C

trap controlC INT

# Handle a keyboard control-C for stopping script execution

controlC() {
  rm -f /tmp/{tmp,curl}.$$
  exit 0
}

# Fatal error handler. If the first arg $1 is numeric it is a process exit 
# status from curl or the script and this may give more details about what 
# went wrong. It puts out any other arguments to the terminal and 
# terminates the shell process running the script with an abnormal exit value.
# Exit status values starting at 200 indicate errors in this script that 
# should be reported with an issue. Those at or below 92 come from curl.

fatal() { 
    # Put out any other info parameters first
    if [ $# -gt 1 ] ; then
      echo $2
    fi

  case $1 in
    6 ) msg="This usually means the dev board is not plugged in,"
	msg="$msg the WIFI parameters are wrong or the WIFI is not"
	msg="$msg working, or the dev board build/flash failed to"
	msg="$msg prepare the board with the right firmware (i.e. plain"
	msg="$msg esp32 or C3 esp32, etc). Additional clues might be" 
	msg="$msg found by running 'idf.py monitor'."
	shift
	;;
    7 ) msg="This can  be caused by unplugging and plugging the USB"
	msg="$msg connector in and putting the WIFI into a bad state."
	shift
	;;
    # Additional tea leaves for curl codes go here as they are seen. They
    # currently go up to 92.
    200)msg="This is a development error. File an issue."
	shift
	;;
    130)msg="Script stopped with control-C. This doesn't come out reliably."
	msg="$msg See the instructions below for testing for this if it"
	msg="$msg would be useful. Termination with control-C may also"
	msg="$msg leak files in /tmp named curl.<process id #>. If this"
	msg="$msg gets to be a problem a trap handler can"
	msg="$msg fix it."
	shift
	;;
    *)  msg="Undefined exit code $status. See curl output and its man page."
	shift
	;;
  esac

  echo $msg

  # This exit call immediately stops the script execution with an exit 
  # value that can be tested by a containing script like this:
  # edgerBMP
  # status=$?
  # if [ $status -ne 0 ] ;then
  #   echo "something bad happened as edgerBMP exited with: $status"
  # fi
  exit $exitstatus
}

# Do a set within an HTTP Patch request to the Edger Ant I2C endpoint URL. In
# this application the I2C address is parameter $1, the register address is $2
# and the sensor command byte is $3.

patchSetByte() {
  if [ $# -ne 3 ] ; then
    fatal 200 "patchSetByte expected three arguments, got: $@"
  fi

  # Send the byte to the I2C peripheral using an HTTP request over WIFI to Ant

  curl --no-progress-meter --location -g --request PATCH \
  "http://$host.local/api/v1/i2c?set={\"address\":$1,\"index\":$2,\"length\":1,\"data\":[$3]}"\
   --header 'Content-TType: text/plain' --data-raw 'asdasd' >/tmp/tmp.$$
  status=$?
  rm /tmp/tmp.$$
  if [ $status -ne 0 ] ; then
    fatal $status
  fi
}

# Do a get within an HTTP Patch to the Edger Ant I2C endpoint URL. In this 
# application the I2C # address is parameter $1, the register address is $2
# an expected return length of two bytes is hard-wired.
# Notice the bytes are in big endian order. The curl value is returned via
# global/reference parameter $3.

patchGetUnsignedShort() {
  if [ $# -ne 3 ] ; then
    fatal 200 "patchGetUnsignedShort expected three arguments"
  fi

  # Fetch two bytes from the I2C peripheral using an HTTP request 
  # over WIFI to the Ant firmware in the dev system
  #echo "address " $1 " index: " $2 " length 2"
  # 119 178 2
  curl --no-progress-meter --location -g --request PATCH "http://$host.local/api/v1/i2c?get={\"address\":$1,\"index\":$2,\"length\":2}" --header 'Content-Type: text/plain' --data-raw 'asdasd' >/tmp/curl.$$

  status=$?

  if [ $status -ne 0 ] ; then
    fatal $status
  fi

  # Extract the two byte values and form an unsigned integer with them

  grep get /tmp/curl.$$ | sed -e's@.*\[@@' | sed -e's@\].*@@' >/tmp/tmp.$$
  rm /tmp/curl.$$

  high=`cut -d"," -f1 /tmp/tmp.$$`
  low=`cut -d"," -f2 /tmp/tmp.$$`
  rm /tmp/tmp.$$

  # The declare -g effectively converts the actual parameter name of the arg 
  # passed in as $3 from call by name to call by reference. It actually 
  # converts the parameter into a global variable so it's value change here
  # becomes visible to the caller.

  # It took me many hours to figure this out. The chatGPT was less than 
  # worthless for this, as were many Stack Exchange entries. After a lot of 
  # digging I spotted an example of declare -g usage that made the light go 
  # on. All explanations of declare -g found on the Internet simply stink.
  # Seeing the use cases below will hopefully make it clear how powerful
  # this feature is and how it makes what is otherwise a HEAD BANGING problem
  # quite straight forward.

  declare -g $3=$(awk "BEGIN{print $high * 256 + $low}")
}

# Convert an unsigned short integer value in parameter $1 to a signed integer 
# value. The high order bit (bit 15) is tested by seeing if the unsigned value 
# is greater than the largest possible positive value. If it is set then 
# arithmetic is used to convert to a twos complement negative value. The
# computed value is returned via global/reference parameter $2.

unsignedShortToShort() {
  if [ $# -ne 2 ] ; then
    fatal 200 "unsignedShortToShort expected two argument$"
    exit 1
  fi

  if [ $1 -ge 32768 ] ; then
    declare -g $2=$(awk "BEGIN{print (65536 - $1) * -1}")
  else
    declare -g $2=$1
  fi
}

# Do a get within an HTTP Patch to the Edger Ant I2C endpoint URL with I2C 
# address $1, register address $2 and received length 2. The received unsigned
# short is converted from unsigned to signed by unsignedShortToShort and
# returned via reference/global parameter $3. 

getSignedShort() {
  if [ $# -ne 3 ] ; then
    fatal 200 "unsignedShortToShort expected three arguments"
  fi
  patchGetUnsignedShort $1 $2 val1
  unsignedShortToShort $val1 val2
  declare -g $3=$val2
}

# Main code

# Fetch the sensor calibration register values

patchGetUnsignedShort $BMP_I2C_ADDR $CAL_AC5_REG ac5

patchGetUnsignedShort $BMP_I2C_ADDR $CAL_AC6_REG ac6

getSignedShort $BMP_I2C_ADDR $CAL_MC_REG mc

getSignedShort $BMP_I2C_ADDR $CAL_MD_REG md

# Endless loop

while [ 1 -eq 1 ] ; do

  # Request the temperature

  patchSetByte $BMP_I2C_ADDR $BMP_CONTROL $BMP_READ_TEMP

  # Fetch the raw temp as an unsigned short.

  patchGetUnsignedShort $BMP_I2C_ADDR $BMP_TEMP_DATA ut

  # Adjust with calibration values. See the BMP085 or 180 datasheet for details.
  # The cal and other var names and arithmetic operations are right out of the 
  # datasheet.

  x1=$(awk "BEGIN{print (($ut - $ac6) * $ac5) / 32768}")
  x2=$(awk "BEGIN{print int(($mc * 2048) / ($x1 + $md))}")
  b5=$(awk "BEGIN{print (($x1 + $x2) + 8) / 16}")

  # Show temp in centigrade and fahrenheit.

  echo $(awk "BEGIN{printf \"C: %4.1f F: %4.1f\",$b5 / 10.0, ($b5 / 10.0) * 1.8 + 32.0}")

  # sleep for a while

  sleep $WAIT_TIME
done
