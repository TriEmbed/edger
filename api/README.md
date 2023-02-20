# api 
The api directories here are intended to contain web browser GUI use case instructions and CLI programs written in Python, or shell or some other language to demonstrate Edger applications.

### api/gui
This directory is meant to focus on the web browser GUI interfaces.

#### api/gui/doc
This directory is meant to hold formal GUI API documentation with screen shots.

#### api/gui/examples 
This directoryis meant to hold how-to instructions for doing things with the web browser for specific applications and use cases.

### api/cli 
This directoryis meant to focus on on the Edger Ant HTTP URL endpoints.

#### api/cli/doc 
This directoryis meant to contain formal API documentation with Doxygen.

#### api/cli/examples
This directory is mean to contain programs (source, and binaries) of examples of specific applications implemented with endpoint URLs via HTTP.

##### api/cli/examples/edgerBMP
This directory contains an application that periodically prints temperature measurements to a terminal session. It does not need aardvark, just the USB connection of a "white" dev board to the computer to be displaying temperatures from an I2C breakout board and a shared WIFI connection with the dev board.
