//
// Created by nicked on 10/7/22.
//
#include <fcntl.h>
#include "esp_http_server.h"
#include "esp_system.h"
#include "esp_log.h"
#include "cJSON.h"
#include <time.h>

#include <esp_sntp.h>
#include <esp_log.h>
#include "utilities.h"
#include "rest_server.h"
#include "configuration.h"

static const char *TAG = "configuration";


static cJSON *
timeFun(char *parm, UNUSED httpd_req_t *req) {
    cJSON *root = cJSON_CreateObject();
    time_t now;
    char strftime_buf[64];
    struct tm timeinfo;

    time(&now);
// Set timezone to China Standard Time
    setenv("TZ", "EST", 1);
    tzset();

    localtime_r(&now, &timeinfo);
    if (timeinfo.tm_year < (2016 - 1900)) {
        ESP_LOGI(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
        sntp_setoperatingmode(SNTP_OPMODE_POLL);
        sntp_setservername(0, "pool.ntp.org");
        sntp_init();
    }
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current EST date/time is: %s", strftime_buf);
    cJSON_AddStringToObject(root, "time", strftime_buf);

    return root;
}


static cJSON *
testFun(char *parm, UNUSED httpd_req_t *req) {
    cJSON *root = cJSON_CreateObject();
    time_t now;
    char strftime_buf[64];
    struct tm timeinfo;

    time(&now);
// Set timezone to China Standard Time
    setenv("TZ", "EST", 1);
    tzset();

    localtime_r(&now, &timeinfo);
    if (timeinfo.tm_year < (2016 - 1900)) {
        ESP_LOGI(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
        sntp_setoperatingmode(SNTP_OPMODE_POLL);
        sntp_setservername(0, "pool.ntp.org");
        sntp_init();
    }
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current EST date/time is: %s", strftime_buf);
    cJSON_AddStringToObject(root, "time", strftime_buf);

    return root;
}

esp_err_t configuration_get_handler(httpd_req_t *req) {

    static const uriParam configurationParams[] = {
            {.name = "time", .fun = timeFun},
            {.name = "test", .fun = testFun},
            {.name = NULL, .fun = NULL},
    };
    cors_header(req);
    cJSON *root = paramParser(req, configurationParams);

    httpd_resp_set_type(req, "application/json");
    httpd_resp_sendstr(req, cJSON_Print(root));
    cJSON_Delete(root);
    return ESP_OK;
}
