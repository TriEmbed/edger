/*
 * Bosch BMP388 and BMP390 handler for temperature
 *
 * See BMP3XX.h for interface details.
 *  
 * pete@soper.us May 3, 2023 Creative Commons CC0 1.0 Universal license
 */

#include <string.h>
#include <fcntl.h>
#include <esp_http_server.h>
#include <esp_system.h>
#include <esp_log.h>
#include <esp_vfs.h>
#include <cJSON.h>
#include <time.h>
#include <http_parser.h>
#include "configuration/system_info.h"
#include "rest_server.h"
#include "configuration/configuration.h"
#include "utilities.h"
#include "i2c/i2cDriver.h"

#include "BMP3XX.h"

// Saved compensation coefficients

static double t1, t2, t3;

// True until calibration values fetched

static char bmp388_need_calibration = 1;

// Calibration coefficients

static double t1, t2, t3;

/* Handler for getting temperature data from a BMP388/390 */

esp_err_t temperatureBmp3xx(httpd_req_t *req) {
    httpd_resp_set_type(req, "application/json");
    cors_header(req);

    cJSON *root = cJSON_CreateObject();

    // Have the chip calibration registers been fetched?

    if (bmp388_need_calibration) {
        unsigned char cal_values[21] = {0};
        cJSON *cal_json = i2cGet(BMP3XX_I2CADDR,
					BMP3XX_REGISTER_CAL_DATA,21,-1,0,0);
        cJSON *item;
        int i = 0;
        cJSON_ArrayForEach(item, cal_json) {
            if (i >= sizeof(cal_values)) {
                printf("cJSON_ArrayForEach overflow %s %d: hanging\n", 
			__FILE__, __LINE__);
                while(1) {
                }
            }
            cal_values[i] = item->valueint;
            i += 1;
        }
        cJSON_Delete(cal_json);

        // Get cal constants
        // Big endian chars => unsigned shorts
        // for (int i=0;i<5;i++) {
        //     printf("cal_values[%d] %x\n", i, cal_values[i]);
        // }
        int comp0 = ((int) cal_values[1] << 8) | (int) cal_values[0];
        int comp1 = ((int) cal_values[3] << 8) | (int) cal_values[2];
        int comp2 = cal_values[4];
        // sign extend
        if (comp2 & 0x80) {
            comp2 |= 0xffffff00;
        }
        // printf("comp0: %x comp1: %x comp2: %d\n", comp0, comp1, comp2);

        // Create coefficients
        t1 = (double) comp0 * 256.0;
        t2 = (double) comp1 / (double) (2LL << 29LL);
        t3 = (double) comp2 / (double) (2LL << 47LL);
        // printf("t1: %le t2: %le t3: %le\n", t1, t2, t3);

        bmp388_need_calibration = 0;
    }


    // Enable sensors and set mode to forced
    cJSON *data = cJSON_CreateArray();

    cJSON_AddItemToArray(data, cJSON_CreateNumber(BMP3XX_ENABLE_FORCED));
    cJSON *resp= i2cSet (BMP3XX_I2CADDR, BMP3XX_REGISTER_CONTROL, data);

    cJSON_Delete(data);
    cJSON_Delete(resp);

    // Sample status until a new sample is available 

    int iterations = 0; // temporary
    char status = 0;
    do {
        iterations += 1;
        if (iterations > 50) {
	    break;
        }
	cJSON *temp1 = i2cGet (BMP3XX_I2CADDR, BMP3XX_REGISTER_STATUS, 1,-1,0,0);
        // magic here to get the byte value out of temp1 and into status
	// magic(temp1, status)
        cJSON *item;
        int i = 0;
        cJSON_ArrayForEach(item, temp1) {
            if (i >= 1) {
                printf("cJSON_ArrayForEach overflow %s %d: hanging\n", 
                          __FILE__, __LINE__);
                while(1) {
                }
            }
            status = item->valueint;
            i += 1;
        }
        cJSON_Delete(temp1);
    } while ((status & 0x60) != 0x60);

    //printf("iterations: %d\n", iterations);

    // Fetch the sensor data
    char sample[6] = {0};
    cJSON *temp2 = i2cGet(BMP3XX_I2CADDR,BMP3XX_REGISTER_PRESSUREDATA,6,-1,0,0);
    cJSON *item;
    int i = 0;
    cJSON_ArrayForEach(item, temp2) {
	if (i >= sizeof(sample)) {
	    printf("cJSON_ArrayForEach overflow %s %d: hanging\n", 
		      __FILE__, __LINE__);
	    while(1) {
	    }
	}
	sample[i] = item->valueint;
	i += 1;
    }
    cJSON_Delete(temp2);
    // Raw samples are 16-21 bit integers (16 with no oversampling)
    //int adc_p = sample[2] << 16 | sample[1] << 8 | sample[0];
    int adc_t = sample[5] << 16 | sample[4] << 8 | sample[3];

    // Apply compensation and convert to fahrenheit
    double pd1, pd2, temperature;
    pd1 = (double) adc_t - t1;
    pd2 = pd1 * t2;
    temperature = (pd2 + (pd1 * pd1) * t3) * 9.0 / 5.0;

    int int_temperature = (int) temperature;
    if ((temperature - int_temperature) > 0.50) {
      int_temperature += 1;
    }

    // printf("pd1: %e pd2: %e temperature: %e int_temperature: %d\n",
    //			pd1, pd2, temperature, int_temperature);

    // printf("temp: %d\n", int_temperature);

    // add value to root object
    cJSON *sensor_value = cJSON_CreateNumber(int_temperature);
    cJSON_AddItemToObject(root, "raw", sensor_value);
	
    const char *sys_info = cJSON_Print(root);
    httpd_resp_sendstr(req, sys_info);
    free((void *) sys_info);
    cJSON_Delete(root);
    return ESP_OK;
}






