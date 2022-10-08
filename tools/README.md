# Build and other support tools

## linux/installit is a bash script that automates installation of the Espressif IDF, nvm, node.js, ESP32 app repo que_ant, portable web application que_aardvark and necessary prerequisit linux packages. After completing installation and most build operations final steps for flashing the target ESP32, setting an IP address in que_aardvark and running both to allow programming the FPGA are displayed.

## A "user manual" for the current system can be found at https://triembed.org/edger-user-manual/

## Instructions for building a dev system by combining an ESP32 dev board with a Dialog SLG47004V-DIP FPGA board or (with a subset of steps) a que_m80 dev board version 0.60 or 0.70.

Here's how to connect  a resistor and LED to the Dialog FPGA DIP board and that board to an ESP32. This will give you the same setup as Dawn Trembath and Jaimie Johnsen have or an equivalent with a que_m80 dev board such as Rob Mackie has (just wiring the DIP board). Refer to your ESP32 docs for the pinouts. You need the 3.3v power, ground, pin 18, and pin 19. For the Dialog board refer to the following document. NOTE WELL that you're to pay attention to the pin labels in the colored boxes and use that to correlate with the actual pin of the DIP board to physically connect. https://github.com/TriEmbed/que_m80/blob/main/doc/contrib/dialog/datasheets/SLG47004V_DIP_Proto_Board_Quick_Start_Guide.pdf .  Connect ESP32 ground to the DIP board "GND". Connect ESP32 pin "3.3v" to DIP board "Vdd". Connect ESP32 pin "I/O 18" to Dialog DIP board pin 11 and pin "I/O 19" to DIP board pin 10. Jumper Dialog pins 15 and 16 together. Connect the annode (longer lead) of an LED to Dialog DIP board pin 18 and the cathode to an uncommited place on the breadboard. Then use an approximately 1k (500-1500 should work) resistor from that same place on the breadboard to the DIP board ground. Now you're ready to use aardvark to load "greekpak-test1.aap" into the FPGA and run it to cause the LED to blink. The .aap file is here: https://github.com/TriEmbed/League/tree/main/CAD.

See the echo commands at the bottom of linux/installit for instructions for flashing the image in the-ant, running it in the esp32, getting it's IP address and pasting that into aardvark, building and running it as a web host.

## Follow the instructions for running aardvark by pointing your browser to thetools/aardvark/index.html

 

