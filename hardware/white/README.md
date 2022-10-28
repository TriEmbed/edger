# "White" Edger development board ('cause solderless breadboards are white)
The "white board" is an Edger subsystem that can be made with off the shelf parts:
1. An ESP32 dev board such as the one described below
2. An SLG47004V-DIP Renesas "FORGEFPGA" mixed signal FPGA on DIP breakout board
3. A long (830 point) solderless breadboard
4. An LED (Vf < 3.3v)
5. A 1k resistor
6. Jumper wires
7. Other parts as needed such as additional LEDs

## Version 2
This version is similar to the r1 subsystem but with the following changes:
1. An Espressif ESP32-C3-DEVKITC-02 dev board
2. An Adafruit mini-USB breakout for serial JTAG
3. Twin Industries breadboard instead of the crummy boards from Amazon

See the r2 directory for details.

## Version 1

![](r1/white-with-LEDs.jpg)

Here is a suitable ESP32 "WROOM" board but beware wimpy 3.3v regulators:

https://smile.amazon.com/gp/product/B08MFCC4SR/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1

r1/esp32pinout.odt is a LibreOffice file with board pinout labels for the WROOM above
r1/white-with-LEDs.jpg and r1/board-top-view.jpg  show the connections. The label "CONN" on the cheat sheet is in the position of the board's USB micro connector.

File SLG47004V_DIP_Proto_Board_Quick_Start_Guide.pdf shows a diagram of the FPGA DIP board. Note well that while "Pin 1 (vcc)" is in position 1 of the 20 position DIP connector most (possibly all) other pins have labels that DO NOT reflect their connector positions. One must look at the actual connector pin position using the red or blue connection line to orient a connection.

Specific wiring details:

1. ESP32 IO19 to FPGA "Pin 10" (SCL)"
2. ESP32 IO18 to FPGA "Pin 11" (SDA)"
3. ESP32 GND to FPGA "Pin 2" (GND)"
4. ESP32 3.3V to FPGA "Pin 1" (VDD)"
5. FPGA "Pin 15" (GPIO) to FPFA "Pin 16" (GPIO)
6. FPGA "pin 18" (GPIO) to LED anode
7. LED anode to 1k resistor
8. 1k resistor to "pin 2" (GND)
