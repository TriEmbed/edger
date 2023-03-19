#!/usr/bin/env python
'''
Periodically get temperature measurements from a Bosch BMP085 or BMP280 
temperature(/barometer) sensor and display them to standard output as degrees
fahrenheit and centigrade. Note that both of these sensors are obsolete. The
BMP388 is a good alternative.

pete@soper.us March 18, 2023 Creative Commons CC0 1.0 Universal license
'''

import sys
import time
import requests

# Seconds to wait between temp reports

WAIT_TIME = 5

# Bosch sensor I2C access variables

# BMP085 default address.
BMP085_I2CADDR           = 0x77

# BMP085 Registers
BMP085_CAL_AC1           = 0xAA
BMP085_CAL_AC2           = 0xAC
BMP085_CAL_AC3           = 0xAE
BMP085_CAL_AC4           = 0xB0
BMP085_CAL_AC5           = 0xB2
BMP085_CAL_AC6           = 0xB4
BMP085_CAL_B1            = 0xB6
BMP085_CAL_B2            = 0xB8
BMP085_CAL_MB            = 0xBA
BMP085_CAL_MC            = 0xBC
BMP085_CAL_MD            = 0xBE
BMP085_CONTROL           = 0xF4
BMP085_TEMPDATA          = 0xF6
BMP085_PRESSUREDATA      = 0xF6

# Commands
BMP085_READTEMPCMD       = 0x2E
BMP085_READPRESSURECMD   = 0x34

# Just go with the first Edger device for now. It would be easy to pass this
# in as a parameter (referenced with $1)

HOST='esp-home.local'

# The endpoint URL for the I2C space within the Edger board
ENDPOINT_URL='http://' + HOST + '/api/v1/i2c'

# For patch request
headers = {
    'Content-Type': 'text/plain',
}

DATA = 'asdasd'

def write_cmd(i2caddr, index, cmd):
    '''Write a command byte to the endpoint URL'''
    try:
        response = requests.patch(
            ENDPOINT_URL + '?set={"address":' + str(i2caddr) + ',"index":' +
            str(index) + ',"length":1,"data":[' + str(cmd) + ']}--header',
            headers=headers, data=DATA,
        )

    except requests.exceptions.ConnectionError:
        print('Connection error:')
        print('This usually means the dev board is not powered up,')
        print('the WIFI parameters are wrong or the WIFI is not')
        print('working, or the dev board build/flash failed to')
        print('prepare the board with the right firmware (i.e. plain')
        print('esp32 or C3 esp32, etc). Additional clues might be')
        print('found by running "idf.py monitor".')
        sys.exit()
    except Exception as e:
        print('Unknown exception: ' + e)
        sys.exit()

    if response.status_code != 200:
        print('write_cmd() got this status: ' + str(response.status_code))
        sys.exit()

def read_data(i2caddr, index, length):
    '''Read a string of bytes from the endpoint URL'''
    try:
        response = requests.patch(
            ENDPOINT_URL + '?get={"address":' + str(i2caddr) + ',"index":' +
                    str(index) + ',"length":' + str(length) + '}',
            headers=headers,
            data=DATA,
        )
    except requests.exceptions.ConnectionError:
        print('Connection error:')
        print('This usually means the dev board is not powered up,')
        print('the WIFI parameters are wrong or the WIFI is not')
        print('working, or the dev board build/flash failed to')
        print('prepare the board with the right firmware (i.e. plain')
        print('esp32 or C3 esp32, etc). Additional clues might be')
        print('found by running "idf.py monitor".')
        sys.exit()
    except Exception as e:
        print('Unknown exception: ' + e)
        sys.exit()

    if response.status_code != 200:
        print('read_data() got this status: ' + str(response.status_code))
        sys.exit()
    response_json = response.json()
    return response_json['i2c'][0]['get']

def get_unsigned_byte(i2caddr, index):
    '''Read an 8 bit unsigned integer from the endpoint URL'''
    tmp = read_data(i2caddr, index, 1)
    return tmp[0]

def get_unsigned_short(i2caddr, index):
    '''Read a 16 bit unsigned integer from the endpoint URL'''
    tmp = read_data(i2caddr, index, 2)
    return tmp[0] << 8 | tmp[1]

def get_signed_byte(i2caddr, index):
    '''Read an 8 bit signed integer from the endpoint URL'''
    tmp = get_unsigned_byte(i2caddr, index)
    if tmp >= 128:
        tmp = (256 - tmp) * -1
    return tmp

def get_signed_short(i2caddr, index):
    '''Read a 16 bit signed integer from the endpoint URL'''
    tmp = get_unsigned_short(i2caddr, index)
    if tmp >= 32768:
        tmp = (65536 - tmp) * -1
    return tmp

# Fetch calibration constants. Follow naming conventions from datasheet.

AC1 = get_signed_short(BMP085_I2CADDR, BMP085_CAL_AC1)
AC2 = get_signed_short(BMP085_I2CADDR, BMP085_CAL_AC2)
AC3 = get_signed_short(BMP085_I2CADDR, BMP085_CAL_AC3)
AC4 = get_unsigned_short(BMP085_I2CADDR, BMP085_CAL_AC4)
AC5 = get_unsigned_short(BMP085_I2CADDR, BMP085_CAL_AC5)
AC6 = get_unsigned_short(BMP085_I2CADDR, BMP085_CAL_AC6)
B1 = get_signed_short(BMP085_I2CADDR, BMP085_CAL_B1)
B2 = get_signed_short(BMP085_I2CADDR, BMP085_CAL_B2)
MB = get_signed_short(BMP085_I2CADDR, BMP085_CAL_MB)
MC = get_signed_short(BMP085_I2CADDR, BMP085_CAL_MC)
MD = get_signed_short(BMP085_I2CADDR, BMP085_CAL_MD)

# Loop endlessly (until control-C entered), putting out temps

while True:
    try:
        write_cmd(BMP085_I2CADDR, BMP085_CONTROL, BMP085_READTEMPCMD)

        # Follow naming conventions from datasheet

        ut = get_unsigned_short(BMP085_I2CADDR, BMP085_TEMPDATA)

        x1 = (((ut - AC6) * AC5)) / 32768
        x2 = int((MC * 2048) / (x1 + MD))
        b5 = ((x1 + x2) + 8) / 16

        degrees_c = b5 / 10.0
        degrees_f = degrees_c * 1.8 + 32.0

        print(f'C: {degrees_c:5.1f} F: {degrees_f:5.1f}')

        time.sleep(WAIT_TIME)
    except KeyboardInterrupt:
        sys.exit()
