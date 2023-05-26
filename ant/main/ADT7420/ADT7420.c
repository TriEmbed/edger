
/* HTTP Restful API Server

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
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
//#include "i2cDriver.h"
#include "configuration/configuration.h"
#include "utilities.h"
#include "i2c/i2cDriver.h"
#include  "ADT7420.h"
#include <esp_random.h>




/* Simple handler for getting temperature data */
esp_err_t
temperature_data_get_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "application/json");
    cors_header(req);
 
 

    cJSON *root = cJSON_CreateObject();

	cJSON * data=	cJSON_CreateArray();
cJSON *  got ;
#ifdef INSERT_I2C_STUFF_HERE
	cJSON_AddItemToArray(data,  cJSON_CreateNumber(0x80));


	cJSON * resp= i2cSet (72, 3, data);
    cJSON_Delete(data);
	cJSON_Delete(resp);


   	 got =i2cGet (72, 0, 2);
#else

   got  =cJSON_CreateNumber(esp_random() % 100);
#endif
    cJSON_AddItemToObject(root, "raw", got);
	
    const char *sys_info = cJSON_Print(root);
    httpd_resp_sendstr(req, sys_info);
    free((void *) sys_info);
    cJSON_Delete(root);
    return ESP_OK;
}






