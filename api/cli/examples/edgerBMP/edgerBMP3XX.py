#!/usr/bin/env python
'''
Periodically get temperature measurements from a Bosch BMP388 or BMP390 
sensor and display them to standard output as degrees fahrenheit and centigrade. 

pete@soper.us March 19, 2023 Creative Commons CC0 1.0 Universal license
'''

import sys
import time
import struct
import requests

# Seconds to wait between temp reports

WAIT_TIME = 5

# Bosch sensor I2C access variables

# I2C addresses

BMP3XX_I2CADDR = 0x76
BMP3XX_ALT_I2CADDR = 0x77
BMP3XX_REGISTER_CONTROL = 0x1B
BMP3XX_REGISTER_CAL_DATA = 0x31
BMP3XX_REGISTER_PRESSUREDATA = 0x04
BMP3XX_REGISTER_STATUS = 0x03
BMP3XX_REGISTER_CHIPID = 0x00
BMP3XX_CHIPID_388 = 0x50
BMP3XX_CHIPID_390 = 0x60

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
    except Exception as exe:
        print('Unknown exception: ' + exe)
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
    except Exception as exe:
        print('Unknown exception: ' + exe)
        sys.exit()

    if response.status_code != 200:
        print('read_data() got this status: ' + str(response.status_code))
        sys.exit()
    response_json = response.json()
    return response_json['i2c'][0]['get']

def get_unsigned_byte(i2caddr, index):
    '''Read an 8 bit unsigned integer from the endpoint URL'''
    ftmp = read_data(i2caddr, index, 1)
    return ftmp[0]

def get_unsigned_short(i2caddr, index):
    '''Read a 16 bit unsigned integer from the endpoint URL'''
    ftmp = read_data(i2caddr, index, 2)
    return ftmp[0] << 8 | ftmp[1]

def get_signed_byte(i2caddr, index):
    '''Read an 8 bit signed integer from the endpoint URL'''
    ftmp = get_unsigned_byte(i2caddr, index)
    if ftmp >= 128:
        ftmp = (256 - ftmp) * -1
    return ftmp

def get_signed_short(i2caddr, index):
    '''Read a 16 bit signed integer from the endpoint URL'''
    ftmp = get_unsigned_short(i2caddr, index)
    if ftmp >= 32768:
        ftmp = (65536 - ftmp) * -1
    return ftmp

# Fetch calibration data

tmp = read_data(BMP3XX_I2CADDR, BMP3XX_REGISTER_CHIPID, 1)
if (tmp[0] != BMP3XX_CHIPID_388) and (tmp[0] != BMP3XX_CHIPID_390):
  print('Chip ID did not match: ' + str(tmp[0]))
  sys.exit()
tmp = read_data(BMP3XX_I2CADDR, BMP3XX_REGISTER_CAL_DATA, 21)
coeff = bytearray(21)
for i in range(21):
    coeff[i] = tmp[i]
coeff = struct.unpack("<HHbhhbbHHbbhbb", coeff)
T1 = coeff[0] / 2**-8.0
T2 = coeff[1] / 2**30.0
T3 = coeff[2] / 2**48.0

P1 = (coeff[3] - 2**14.0) / 2**20.0
P2 = (coeff[4] - 2**14.0) / 2**29.0
P3 = coeff[5] / 2**32.0
P4 = coeff[6] / 2**37.0
P5 = coeff[7] / 2**-3.0
P6 = coeff[8] / 2**6.0
P7 = coeff[9] / 2**8.0
P8 = coeff[10] / 2**15.0
P9 = coeff[11] / 2**48.0
P10 = coeff[12] / 2**48.0
P11 = coeff[13] / 2**65.0

# Loop endlessly (until control-C entered), putting out temps

while True:
    try:
        # force a sample
        write_cmd(BMP3XX_I2CADDR,BMP3XX_REGISTER_CONTROL, 0x13)
        status = 0

        while (status & 0x60) != 0x60:
            status = read_data(BMP3XX_I2CADDR,BMP3XX_REGISTER_STATUS,1)[0]
            time.sleep(1)

        # fetch the raw temp and pressure

        data = read_data(BMP3XX_I2CADDR,BMP3XX_REGISTER_PRESSUREDATA,6)
        adc_p = data[2] << 16 | data[1] << 8 | data[0]
        adc_t = data[5] << 16 | data[4] << 8 | data[3]

        pd1 = adc_t - T1
        pd2 = pd1 * T2

        # compensate the temperature

        temperature = pd2 + (pd1 * pd1) * T3

        # compensate the pressure

        pd1 = P6 * temperature
        pd2 = P7 * temperature**2.0
        pd3 = P8 * temperature**3.0
        po1 = P5 + pd1 + pd2 + pd3

        pd1 = P2 * temperature
        pd2 = P3 * temperature**2.0
        pd3 = P4 * temperature**3.0
        po2 = adc_p * (P1 + pd1 + pd2 + pd3)

        pd1 = adc_p**2.0
        pd2 = P9 + P10 * temperature
        pd3 = pd1 * pd2
        pd4 = pd3 + P11 * adc_p**3.0

        # This pressure value is unbelievable. Too large by a factor of 100.
        # There is something wrong: leave this until it can be debugged.

        pressure = po1 + po2 + pd4

        degrees_c = temperature
        degrees_f = degrees_c * 1.8 + 32.0
        print(f'C: {degrees_c:5.1f} F: {degrees_f:5.1f}')

        time.sleep(WAIT_TIME)
    except KeyboardInterrupt:
        sys.exit()
