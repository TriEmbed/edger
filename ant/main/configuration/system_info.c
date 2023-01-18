//
// Created by nicked on 10/7/22.
//

#include <fcntl.h>
#include "esp_http_server.h"
#include "esp_system.h"
#include "esp_log.h"
#include "cJSON.h"
#include <time.h>
#include "esp_flash.h"
#include "rest_server.h"
#include <esp_netif.h>




/* Simple handler for getting system handler */
esp_err_t
system_info_get_handler (httpd_req_t * req)
{
  httpd_resp_set_type (req, "application/json");
  cors_header (req);
  esp_chip_info_t chip_info;
  uint8_t MAC[6];
  char buf[24];
  char *info;
  uint32_t size_flash_chip;
  tcpip_adapter_ip_info_t ipInfo;
  cJSON *root = cJSON_CreateObject ();
  //////////////////////////////////////////////

  esp_chip_info (&chip_info);
  cJSON_AddStringToObject (root, "version", IDF_VER);
  cJSON_AddStringToObject (root, "friendly name", "friendly name");
  cJSON_AddStringToObject (root, "comments", "comments");
  cJSON_AddNumberToObject (root, "cores", chip_info.cores);


  // IP address.
  tcpip_adapter_get_ip_info (TCPIP_ADAPTER_IF_STA, &ipInfo);
  sprintf (buf, IPSTR, IP2STR (&ipInfo.ip));

      cJSON_AddStringToObject (root, "ip", buf);

  cJSON_AddNumberToObject (root, "ESP32 Chip Features", chip_info.features);

  cJSON_AddNumberToObject (root, "ESP32 Chip Revision ", chip_info.revision);

  esp_flash_get_size (NULL, &size_flash_chip);

  cJSON_AddNumberToObject (root, "SPI Flash Chip MByte",
			   size_flash_chip / (1024 * 1024));
  cJSON_AddStringToObject (root, "EMB_FLASH",
			   chip_info.features & CHIP_FEATURE_EMB_FLASH ?
			   "Embedded" : "External");
  esp_base_mac_addr_get (MAC);
  sprintf (buf, "%02X.%02X.%02X.%02X.%02X.%02X", MAC[0], MAC[1], MAC[2],
       MAC[3], MAC[4], MAC[5]);
  cJSON_AddStringToObject (root, "mac", buf);

  switch (chip_info.model)
    {
    case CHIP_ESP32:
      info = "ESP32";
      break;
    case CHIP_ESP32S2:
      info = "ESP32-S2";
      break;
    case CHIP_ESP32S3:
      info = "ESP32-S3";
      break;
    case CHIP_ESP32C3:
      info = "ESP32-C3";
      break;
    case CHIP_ESP32H2:
      info = "ESP32-H2";
      break;
      // case CHIP_ESP32C2:
    default:
      info = "ESP32-C2";
      break;
    }
  cJSON_AddStringToObject (root, " cpu type ", info);
  //////////////////////////////////////////////
  const char *reason = "reset Reason";
  switch (esp_reset_reason ())
    {
    case ESP_RST_UNKNOWN:
      cJSON_AddStringToObject (root, reason,
			       "Reset reason can not be determined");
      break;
    case ESP_RST_POWERON:
      cJSON_AddStringToObject (root, reason, "Reset due to power on event");
      break;
    case ESP_RST_EXT:
      cJSON_AddStringToObject (root, reason,
			       "Reset by external pin (not applicable for ESP32");
      break;
    case ESP_RST_SW:
      cJSON_AddStringToObject (root, reason,
			       " Software reset via esp_restart ");
      break;
    case ESP_RST_PANIC:
      cJSON_AddStringToObject (root, reason,
			       " Software reset due to exception / panic ");
      break;
    case ESP_RST_INT_WDT:
      cJSON_AddStringToObject (root, reason,
			       " Reset (software or hardware) due to interrupt watchdog ");
      break;
    case ESP_RST_TASK_WDT:
      cJSON_AddStringToObject (root, reason, " Reset due to task watchdog ");
      break;
    case ESP_RST_WDT:
      cJSON_AddStringToObject (root, reason,
			       " Reset due to other watchdogs ");
      break;
    case ESP_RST_DEEPSLEEP:
      cJSON_AddStringToObject (root, reason,
			       " Reset after exiting deep sleep mode ");
      break;
    case ESP_RST_BROWNOUT:
      cJSON_AddStringToObject (root, reason,
			       " Brownout reset (software or hardware) ");
      break;
    case ESP_RST_SDIO:
      cJSON_AddStringToObject (root, reason, " Reset over SDIO ");
      break;
    }

  //////////////////////////////////////////////
  time_t now;
  char strftime_buf[64];
  struct tm timeinfo;
  time (&now);
  // Set timezone to China Standard Time
  setenv (" TZ ", " EST ", 1);
  tzset ();
  localtime_r (&now, &timeinfo);
  strftime (strftime_buf, sizeof (strftime_buf), "%c", &timeinfo);
  cJSON_AddStringToObject (root, " Time ", strftime_buf);
  cJSON_AddStringToObject (root, " compile data ", __DATE__);
  cJSON_AddStringToObject (root, " compile time ", __TIME__);
  //////////////////////////////////////////////
  cJSON_AddStringToObject (root, " idf version ", esp_get_idf_version ());
  cJSON_AddNumberToObject
    (root, " minimum free heap size ", esp_get_minimum_free_heap_size ());
  cJSON_AddNumberToObject
    (root, " free heap size ", esp_get_free_heap_size ());
  const char *sys_info = cJSON_Print (root);
  httpd_resp_sendstr (req, sys_info);
  free ((void *) sys_info);
  cJSON_Delete (root);
  return ESP_OK;
}
