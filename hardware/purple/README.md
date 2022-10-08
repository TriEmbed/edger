# Triangle Embedded Interest Group (TriEmbed) purple standalone ESP32/FPGA board
![Repo logo](/images/purple.png)
## purple: Making Dialog Semiconductor mixed signal FPGA chips more accessible

v1.00: Stretch of 0.70
Requirements:

Purple 1.00 requirements (could somebody restore the bullet numbers please?)

    To demonstrate a Renesas ForgeFPGA (formerly Dialog Greenpak formerly Sylogics) chip application, the 1.00 purple board will feature an FPGA chip to provide the 3.3v rail for the board, assuming there is no thermal constraint prevenging this. We're 99.99% sure the regulator in the chip (e.g. the nnnnnn) is a switching type (Pete is too busy to look this up).  The QFN package will be on a daughter board that attaches to the purple board via standard 1/10" center male header soldered in. The nnnnnn chip, it's inductor and caps and other passives will be on the daugherboard and this will be manufactured in bulk and made as close to freely available as possible so we can stick to the "can be hand-soldered" big rule as much as possible. an alternative strategy would be to have the component footprints for the current .5A regulator cicuit on the board and just have two schemes for assembly: one for hand-soldering using the linear reg and the other for reflow using the FPGA switching reg.
    Mini->C USB connector if the support parts for C not onerous. NOTE that the board to host USB connection has to be to >= USB rev X to get more than 500mA (vanilla 3 or greater?)
    Test points to make hand-soldering USB connector easier (MADE MOOT by the presence of a QFN on the board: very few people can hand solder the board if it has a fine-pitch QFN)
    Small JTAG connector for use with ESP-PROG or the like P L U S separate USB-micro socket for the USB serial JTAG interface to the C3. It isn't clear yet (and would make a wonderful issue for somebody to research) whether or not there is any advantage to the board having the two flavors of JTAG interface. For more details see here: https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/api-guides/jtag-debugging/index.html
    Test 0.70 SMD adapter
    Bring all 20 SMD adapter pins out to male headers
    Thoroughly test reset and boot circuits with 0.70 + breadboard parts (i.e. partially depopulate and tie bodge wires to male header pins on breadboard)
    Widen the board for more silkscreen room. Just one row of breadboard positions NO: we will not do this initially 8. Add Digilent Pmod (AMP MODUL MTE) eight position I2C connector. No support for 6/12pos Pmod connectors, only I2C for now.
    Provide conditioned 5v rail, (4, 8?) channel I2C compatible level shifter. This will further lengthen the board
    Tesselated C3 module, not the one that fits in a slot. (Still AI-Thinker or Expressif? Dawn?)
    Power switch between VBUS and rest of board
    Conditioned VBUS available as 5V source
    500mA supply for ESP32 or more if its datasheet calls for it as worst case and we have USB C with its higher current support.
    Supply regulator must not burn finger at max current (finger guard?). Made moot by use of ForgeFPGA regulator circuit (watch the current loop design!)
    Settle on power and user LEDs NO: we will not do this as the tesselated module has beaucoup pins 16. Jumper to disable user LED
    Dependency on decent 830 position solderless breadboard or else board is "insert only" and inexperienced people will not be able to get board out of breadboard without breaking a trace, breaking the board, bending pins, etc. Recommend Global Specialties or Twin Industries. This is a simple fact of life because of the large number of pins. You get what you pay for. We should be careful about provenance wrt breadboards we source.


![v0.70 Prototype with FPGA DIP Adapter](/images/v0.70-DIP-adapter.jpg)
![v0.70 Prototype with SMD Adapter](/images/v0.70-SMD-adapter.jpg)

1. A stand alone, easy to use Espressif ESP32 C3-based dev board that can be a programmer for the Dialog SLGxxxxx ICs and also run applications with an onboard SLG47004V-DIP.
2. Cache of Dialog info resources
   1. Only resources specifically relevant to the board design and application
   2. Local copies of frequently used docs
   3. Additional cookbooks for getting to blinky with Dialog FPGAs 

This board series is designed in concert with Nick Edgington's "aardvark" node.js web application and the "ant" ESP32 firmware. The ant firmware is designed to allow the ESP32 to directly program a Dialog FPGA without the need for the synthesis tool. This allows change of FPGA personality under programmatic control. An I2C expansion connector allows bus connection of additional FPGAs.

## Project Status

Purple versions 0.60 and 0.70 were released and are still in a testing phase. The board silkscreen labels the boards with the name "Aardvark". This will change with future versions.

A version 1.00 board is being planned for TBD release with the following changes:
1. Name change to "TBD"
2. ECO elimination
3. Updates to reset and boot circuitry
4. Provision for a strap to tie the USB connector down and reduce the chance of broken solder joints.
5. Considering connection of SMD adapter pins to Dialog DIP pins if a compatible set of connections can be worked out. This would allow an SMD part in the adapter to be used for in circuit testing and not just programming.

### Release notes

**0.70** 2/11/2022 - Second prototype. Pure superset of 0.60:

1. Set of female headers to accept a Dialog programming adapter. The adapter and the SLG4704V-DIP cannot be on the board at the same time.
2. CH340C USB interface chip that obviates the need for a dongle for programming via USB.
3. User-controlled LED connected to GPIO2 (pin 14?)
4. Reset pushbutton with a 4.7uF cap across it to stretch time it takes enable to return to a high level
5. Aluminum electrolytic cap for regulator output
6. Four position header for connecting ESP32's I2C bus to an outboard Dialog chip for programming. 
7. C3 module firmware creating access point to provide a wireless web interface for fetching FPGA images from the aarvark application.
8. Shorter and narrower slot for C3 board to fit more tightly
9. Dialog board cannot be plugged in backwards
10. There is a SEVERE issue with mechanical integrity of solder joints with the mini-USB connector on the board. Numerous fractured joints have taken place with normal usage. 

**0.60 Initial prototype 1/24/2022

1. 3.3v regulated supply with power on LED, 
2. AI Thinker ESP32-C3-M1 single RISCV core + WIFI + Bluetooth
3. Socket for Dialog SLG47004V-DIP for programming purposes and/or to combine with the ESP32 for applications
4. Requires USB to serial logic cable with both RTS and DTR control for programming
5. Designed to work with firmware that makes the system a general purpose Dialog chip programmer

## Bug/Todo/Wish Lists

   Note: Many items become easy when we can go with a MUCH larger board that follows the big rule of providing a generous area for people to put their own circuitry in place in a straight forward manner. But this requires a supplier other than OSH Park for significantly lower cost. Guestimate can switch to cheaper supplier for rev 0.80 assuming no major problems with 0.70. So with this in mind expect the "wish" items to become "todo" with the larger board.
   "*" means fixed/implemented for next release (in next rev Eagle files) 
   "^" means item to be fixed/implemented when board size increased

### Version 0.80 Bug/Todo/wish list.

1. Thorough design review(s)
2. Switch to USB micro connector? This needs group discussion. There is an inexpensive supply of USB mini sockets in hand while getting inexpensive micros is on the wrong side of Chinese New Year. Need to hash this out for version 0.80.
2. Bug: Silkscreen hard to read. Need booth library and board layout changes.
3. Wish: Silkscreen should include SLG47004V-DIP pin labels, not just numbers.
4. Todo: Need pushbutton for boot? 
5. Todo: Provision for additional Dialog chips?
6. Wish: Pet peripheral footprints
7. Bug: No consensus about licensing. 0.70 board has "CC By-SA 2.00" on underside but we need concensus for 0.80 as it will be released to the public unless it requires a lot of bodge wires. 
8. Relocate female headers to prevent interference between DIP board and C3 module.

### Version 0.70 bug/todo list: 

1. Bug: Decoupling/bypass cap for CH340C overlooked: 100nF cap has to be added on top of the USB chip manually as part of assembly.
2. Todo: The reset circuit is incomplete. Have to cut the traces from !DTR (pin 14) and !RTS (pin 13) under the chip them through a pair of NPN resistors to EN and GP9. A boot switch will be added by gluing it to the board.
3. DIP board comes too close to C3 module, requiring a notch to be cut into the edge of the DIP board.

### Version 0.60 Bug/Todo/wish list.

1. *U G H  B U G: Not clear which way to plug in the SLG47004V-DIP. Plugging it in backwards might be very bad. Make it physically impossible to plug the board in backwards. Arranging for interference with the C3 or other tall part if backwards should make this relatively trivial to do. REMEMBER the board footprint has to be turned 180 degrees to accomplish this. The "long" end of the SLG has to point the other way so the short end can be close to the C3, making it impossible to insert the SLG the wrong way.
2. Bug: No review process for this board version. Could have avoided some mistakes and gotten some ideas for improvement. Let's not send an update to fab without a couple days open for peer review.
3. *Bug: Center to center distance of header pairs off slightly. Need to line up to work well with breadboard. Wish: Consider cutouts to make use of breadboard feasible even when the board is larger such as 10x10cm. Side would be cut out from edge, then have rectangular cutouts along other side for access to the breadboard jumper sites.
4. *Bug: Name on underside of board is misspelled.
5. Bug: No license on silkscreen, only in the Eagle schematic (and that was unilateral: not the will of the FPGA working group.
6. *Bug: Schematic doesn't match build:
    1. Reg output caps 2x68uF but cap used is 100uF
    2. Through hole caps instead of SMD
    3. Pullups not specified resistance

## Parts issues

As of 6/2/2022 most parts are in hand to build about a dozen more que_m80 boards. Notable exception is the pushbutton switches. Hopefully other parts availability improving with the lessening pandemic crisis in China. 


