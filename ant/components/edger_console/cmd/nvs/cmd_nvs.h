/* Console example — declarations of command registration functions.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include "../../components/json/cJSON/cJSON.h"


// Register NVS functions
void register_nvs(void);

esp_err_t get_nvs_marker(int32_t *value);

esp_err_t set_nvs_marker(int32_t value);

esp_err_t set_value_in_nvs(const char *key, const char *str_type,
                           const char *str_value);

cJSON *preset_nvs();

extern const char *reset_key;
extern const char *ssid_key;
extern const char *password_key;
extern  const char  current_namespace[];
extern const char *mdns;
extern const char *i2c_clock;
extern const char *i2c_data;
void set_wifi(char *ssid, char *password, uint64_t timeout) ;

#ifdef __cplusplus
}
#endif
