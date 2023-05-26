/*
 * Bosch BMP388 and BMP390 handler for temperature
 *
 * The device datasheets are here:
 * https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmp388-ds001.pdf
 * https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmp390-ds002.pdf
 * 
 *
 * Bugs/todo
 * 0) Put whoami check in place with appropriate error and logging.
 * 1) Oversampling defaults to "ultra low power, no oversampling". But even
 * with no oversampling the resolution is .005C, so not in a hurry.
 * 2) Pressure compensation isn't right: off by factor of 100. Need to redo
 * comparison with datasheet and other driver algorithms to find mistake. 
 * Could it be that the 2^65 coefficient is screwing up (since this cannot be
 * expessed with a 64 bit integer)? Try leaving this out.
 *
 * pete@soper.us May 3, 2023 Creative Commons CC0 1.0 Universal license
 */

/*
 * Chip details
 */

// I2C addresses

#define BMP3XX_I2CADDR 0x76	// default on all boards seen so far
#define BMP3XX_ALT_I2CADDR 0x77

// Chip IDs
#define BMP3XX_CHIPID_388 0x50
#define BMP3XX_CHIPID_390 0x60

// Relevant registers

#define BMP3XX_REGISTER_CHIPID 0x00
#define BMP3XX_CHIPID_388 0x50
#define BMP3XX_CHIPID_390 0x60
#define BMP3XX_REGISTER_STATUS 0x03
#define BMP3XX_REGISTER_PRESSUREDATA 0x04
#define BMP3XX_REGISTER_CONTROL 0x1B
#define BMP3XX_REGISTER_CAL_DATA 0x31

// For power control reg: enables both temp & pressure and sets forced mode

#define BMP3XX_ENABLE_FORCED 0x13

// Fetch sample and make it available to HTTP response

esp_err_t temperatureBmp3xx(httpd_req_t *req);
