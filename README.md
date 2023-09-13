# Edger
Edger is a standalone system for developing for and using the combination of a WIFI-capable ESP32 and one or more Renesas/Dialog mixed signal FPGA chips or peripheral development boards and breakout boards having I2C interfaces. The Renesas/ForgeFPGA synthesis tool used to create FPGA images has a drag and drop GUI that involves zero Verilog or VHDL. It has a built in simulator and generates files that Edger can program in place in the user's system for simple FPGA applications. These FPGAs are around one to two dollars in single quantity, even ones mounted on a DIP board that can plug into a wireless breadboard. Edger is aimed at making FPGAs available to "the rest of us" while also creating a development ecosystem to augment the Espressif IDF and Arduino IDE.

Actually that's how it started, but it has since evolved into a more generic system for communication between a web browser and a network-connected MCU or SBC.
The first device supported is the ESP32.

## Components
The major components of Edger are:

### ant
Ant is the software that runs on the embedded device and provides REST-based access to device features like GPIO pins, I2C buses, etc.

### aardvark
Aardvark is a web interface based on VUE.js that allows for discovering and communicating with boards running Ant.  

### the edger API
The general concept for the edger API is as follows:
* There is an endpoint for each type of functionality (e.g. /api/v1/i2c, /api/v1/gpio)
* These endpoints are accessed via HTTP PATCH verb
* The request payload is transmitted as one or more GET parameters, which:
  * each have the form verb={"param1":"value1", "param2":"value2", ...}
  * different verbs expect different parameters
    * some verbs don't expect parameters but still must terminate with '=', e.g. /api/v1/i2c?scan=
  * can be chained and will be processed in order for more complex functionality
  * return their output as JSON

## Development install
A full Edger development environment allows for development of both ant and aardvark.  Therefore it includes the following components:
* a clone of this Edger git repo
* the Espressif IoT Development Framework (ESP-IDF) for building ant
* node.js and pnpm for building aardvark, and optionally serving it in development mode for debugging
* some scripts and Desktop icons to wrapper and expose the above functionality

A convenience script is provided for installation on (currently Ubuntu) Linux - linux_install.sh
This script does the following:
1. Checks/clones the Edger repo (by default in $HOME/workspace/esp32/edger, or as specified by --with-edger={absolute-path})
2. Checks/clones the ESP IDF (by default in $HOME/esp/esp-idf, or as specified by --with-esp-idf={absolute-path})
3. Installs node.js using the Node Version Manager script - nvm.sh
4. Uses Node Package Manager (npm) to install pnpm
5. Uses pnpm to build aardvark
6. Installs the ESP-IDF (creates $HOME/.espressif)
7. Copies scripts to $HOME/bin:
  * changewifi - accepts input for wifi SSID and password, modifies the ant sdkconfig, and runs idf.py to build (and flash) ant
  * startaardvark - starts the node development server serving up the aardvark page/code
8. (if $HOME/Desktop exists, indicating a graphical interface is installed) Copies Desktop files to create clickable icons:
  * CHANGE WIFI - runs the change wifi script
  * START AARDVARK - runs the startaardvark script
  * START BROWSER - opens the default browser to the local aardvark URL
9. Appends to ~/.bashrc if it's not there already:
  * append $HOME/bin to PATH
  * alias for idf.py that will load the idf exports before running it
  * env variables settings for NVM

## User group
The user group kickoff meeting was held on 8/31/2022. Video here: https://youtu.be/zqejgwW3aIo
