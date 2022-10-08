# Greenpack synthesis design files
### greenpak.test1.aap - output a slow square wave on pin 18 to implement a "blinky"
An internal clock is divided down to a few times a second rate and fed to pin 18. The design requires a jumper on the SLG47004V-DIP board between two pins. Tested using the Dialog I2C serial debug dongle at 5v. Testing with a 3.3V ESP32-powered system TBD.
