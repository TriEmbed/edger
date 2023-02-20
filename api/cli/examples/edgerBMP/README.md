# edgerBMP
This directory contains an application that periodically prints temperature measurements to a terminal session. It does not need aardvark, just the USB connection of a "white" dev board to the computer to be displaying temperatures from an I2C breakout board and a shared WIFI connection with the dev board.

The sensor breakout board must have a Bosh BMP085 or BMP180 I2C barometer/temperature sensor connected to the default I2C bus of the dev board.

## How to connect the hardware

###White board rev 2 instructions
1. Connect the breakout VCC/3.3v pin to the red stripe power rail of the breadboard.
2. Connect the breakout GND pin to the blue stripe ground rail of the breadboard.
3. Connect breakout SCL pin to the FPGA holes holding the orange wire.
4. Connect the breakout SDA pin to the FPGA holes holding the yellow wire.

###White board rev 1 instructions
This is the same except SCL is pin 18 of the ESP32 board and SDA is pin 19 of the ESP32 board plugged into the solderless breadboard.

## How to use the script
Connect the PC's USB to the white board's little dev board mounted on the solderless breadboard. Don't use the sideways mounted USB connector: that's for special debugging.
Make sure the WIFI is configured and the same as the PC WIFI connection.
Enter "edgerBMP" to get a temperature reading every five seconds. This interval can be changed by editing the edgerBMP script and changing constant "WAIT_TIME".
To stop the script press the control key, hold it and press the C key.

## About the script.
This script is written in Bash shell language and it uses curl to access the Edger dev board via WIFI and Awk to do arithmetic. This is not optimal: a Python equivalent should be created to demonstrate how much easier this is to do, most especially by hiding the dirty details of HTTP Patch requests and handling of JSON strings. Pythonistas needed!

##BUG
1. Using control-C may cause files to accumulate in /tmp with names like temp.12345 but this will be fixed in the near future. If somebody knows how to use trap handlers in Bash scripts they are welcome to fix this but otherwise Pete will get to it.

