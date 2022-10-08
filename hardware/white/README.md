# white dev board
The "white board" is hardware supporting Edger that can be made with off the shelf parts:
1. An ESP32 dev board such as the one described below
2. An SLG47004V-DIP FPGA 
3. A long (830 point) solderless breadboard
4. An LED (Vf < 3.3v)
5. A 1k resistor
6. Jumper wires

![](white-with-LEDs.jpg)

Here is a suitable ESP32 "WROOM" board:

https://smile.amazon.com/gp/product/B08MFCC4SR/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1

Files in this directory:

esp32pinout.odt is a LibreOffice file with board pinout labels for the WROOM above
A  in this directory shows the TOP VIEW of the pins. The lable "CONN" is in the position of the board's USB micro connector.

File "board-top-view.jpg" in this directory shows a top view of the board with wire connections for the "BLINKY" application.

File "SLG47004V_DIP_Proto_Board_Quick_Start_Guide.pdf" shows a diagram of the FPGA DIP board. Note well that while "Pin 1 (vcc)" is in position 1 of the 20 position DIP connector most (possibly) all other pins have labels that DO NOT reflect their connector positions. One must look at the actual connector pin position using the red or blue connection line to orient a connection.


Specific wiring details:
1. ESP32 IO19 to FPGA "Pin 10 (SCL)"
2. ESP32 IO18 to FPGA "Pin 11 (SDA)"
3. ESP32 GND to FPGA "Pin 2 (GND)"
4. ESP32 3.3V to FPGA "Pin 1 (Vdd)"
5. FPGA "Pin 15 (GPIO) to FPFA "Pin 16 (GPIO)"
