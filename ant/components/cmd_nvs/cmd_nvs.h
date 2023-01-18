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
  void register_nvs (void);
  esp_err_t set_value_in_nvs (const char *key, const char *str_type,
			      const char *str_value);
  int32_t inc_count_nvs (int32_t);
  cJSON *preset_nvs ();
  extern const char *reset_key;
  extern const char *ssid;
  extern const char *password;
  extern const char *mdns;
  extern const char *i2c_clock;
  extern const char *i2c_data;
#ifdef __cplusplus
}
#endif
