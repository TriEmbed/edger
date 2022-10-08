# Documentation to do with ESP32 C3+SLG47004V-DIP m80 board

## Links to important docs:

### Boot process
https://docs.espressif.com/projects/esptool/en/latest/esp32/advanced-topics/boot-mode-selection.html

### AI-Thinker C3 dev board datasheet. Schematics on pages 13 and 14
https://docs.ai-thinker.com/_media/esp32/docs/esp-c3-01m-kit-v1.0_specification.pdf

## The boot process of the C3
Initial conditions: Enable and GPIO9 high
1. Enable and GPI9 driven low
2. GPIO9 driven high again about 15ms later
3. Enable driven high and GPIO9 driven low about 100ms later
4. GPIO9 driven high about 20ms later

GPIO9 and Enable stay high during the flashing. When flashing is finished Enable is held low for 100ms (GPIO9 still high). For reasons I cannot begin to explain there is traffic on TXD and RXD *before* the initial step one above. It is as if by convention some exchange of ASCII between the devices can take place independent of the ESP32 being in programming mode.

Given a USB to serial dongle that supports RTS and DTR it should be possible for esptool.py to flash the C3 with a direct connection to the above four pins.
