# Edger White Board version 1

![](white-board-with-additional-LEDS.jpg)

The version 1 White Board contains an inexpensive ESP32 dev board such as the one described below.  Beware wimpy 3.3v regulators (read the one and two star reviews!). Strive for 500mA.

https://smile.amazon.com/gp/product/B08MFCC4SR/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1

![esp32pinout.pdf](esp32pinout.pdf) shows the pinout for the board above. See [ForgeFPGA-SLG47004V-DIP.jpg](../../doc/contrib/dialog/datasheets/SLG47004V_DIP_Proto_Board_Quick_Start_Guide.pdf) for FPGA connection details.

Specific wiring details:

1. ESP32 IO19 to FPGA "Pin 10" (SCL)"
2. ESP32 IO18 to FPGA "Pin 11" (SDA)"
3. ESP32 GND to FPGA "Pin 2" (GND)"
4. ESP32 3.3V to FPGA "Pin 1" (VDD)"
5. FPGA "Pin 15" (GPIO) to FPFA "Pin 16" (GPIO)
6. FPGA "pin 18" (GPIO) to LED anode
7. LED anode to 1k resistor
8. 1k resistor to "pin 2" (GND)

