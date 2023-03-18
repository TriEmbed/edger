#!/usr/bin/env python
'''
Periodically get temperature measurements from a BMP085 or BMP280 
temperature(/barometer) sensor and display them to standard output as degrees
fahrenheit and centigrade.

pete@soper.us March 18, 2023 Creative Commons CC0 1.0 Universal license
'''

import sys
import time
import requests

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
    response = requests.patch(
	ENDPOINT_URL + '?set={"address":' + str(i2caddr) + ',"index":' +
		str(index) + ',"length":1,"data":[' + str(cmd) + ']}--header',
	headers=headers,
	data=DATA,
    )

    if response.status_code != 200:
        print('write_cmd() got this status: ' + str(response.status_code))
        sys.exit()

def read_data(i2caddr, index, length):
    '''Read a string of bytes from the endpoint URL'''
    response = requests.patch(
        ENDPOINT_URL + '?get={"address":' + str(i2caddr) + ',"index":' +
                str(index) + ',"length":' + str(length) + '}',
        headers=headers,
        data=DATA,
    )
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

# Fetch calibration constants

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
    write_cmd(BMP085_I2CADDR, BMP085_CONTROL, BMP085_READTEMPCMD)

    ut = get_unsigned_short(BMP085_I2CADDR, BMP085_TEMPDATA)

    x1 = (((ut - AC6) * AC5)) / 32768
    x2 = int((MC * 2048) / (x1 + MD))
    b5 = ((x1 + x2) + 8) / 16

    print('C: ' + str(float(b5 / 10.0)) + ' F: ' + str(float(b5 / 10.0) * 1.8 + 32.0))
    time.sleep(5)
