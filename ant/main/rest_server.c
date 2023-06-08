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
#include "i2c/i2c.h"
#include "gpio/gpio.h"
//#include "nvram.h"
#include "esp_myconsole.h"
#include "ADT7420/ADT7420.h"
#include "BMP3XX/BMP3XX.h"

#ifdef CONFIG_IDF_TARGET_ESP32C3

#include "driver/rmt.h"
#include "led_strip.h"

#endif

static const char *TAG = __FILE__;

#define RMT_TX_CHANNEL RMT_CHANNEL_0

#define EXAMPLE_CHASE_SPEED_MS (10)

char buf[128];

cJSON *createMenus();

static const char *REST_TAG = "esp-rest";
#define REST_CHECK(a, str, goto_tag, ...)                                              \
    do                                                                                 \
    {                                                                                  \
        if (!(a))                                                                      \
        {                                                                              \
            ESP_LOGE(REST_TAG, "%s(%d): " str, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
            goto goto_tag;                                                             \
        }                                                                              \
    } while (0)


#define CHECK_FILE_EXTENSION(filename, ext) (strcasecmp(&filename[strlen(filename) - strlen(ext)], ext) == 0)

/* Set HTTP response content type according to file extension */
static esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filepath) {
    const char *type = "text/plain";
    if (CHECK_FILE_EXTENSION (filepath, ".html")) {
        type = "text/html";
    } else if (CHECK_FILE_EXTENSION (filepath, ".js")) {
        type = "application/javascript";
    } else if (CHECK_FILE_EXTENSION (filepath, ".css")) {
        type = "text/css";
    } else if (CHECK_FILE_EXTENSION (filepath, ".png")) {
        type = "image/png";
    } else if (CHECK_FILE_EXTENSION (filepath, ".ico")) {
        type = "image/x-icon";
    } else if (CHECK_FILE_EXTENSION (filepath, ".svg")) {
        type = "text/xml";
    }
    return httpd_resp_set_type(req, type);
}

esp_err_t cors_header(httpd_req_t *req) {
//  httpd_resp_set_type (req, "application/json");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
//  httpd_resp_set_hdr(req,"Access-Control-Max-Age", "600");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Methods", "PATCH,PUT,POST,GET,OPTIONS");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Headers", "*");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Credentials", "true");

    return ESP_OK;
}

/* Simple handler for getting temperature data */
static esp_err_t cors_handler(httpd_req_t *req) {
    cors_header(req);
    httpd_resp_sendstr(req, NULL);
    return ESP_OK;
}

/* Send HTTP response with the contents of the requested file */
static esp_err_t rest_common_get_handler(httpd_req_t *req) {
    char filepath[FILE_PATH_MAX];

    rest_server_context_t *rest_context = (rest_server_context_t *) req->user_ctx;
    strlcpy(filepath, rest_context->base_path, sizeof(filepath));
    if (req->uri[strlen(req->uri) - 1] == '/') {
        strlcat(filepath, "/index.html", sizeof(filepath));
    } else {
        strlcat(filepath, req->uri, sizeof(filepath));
    }
    int fd = open(filepath, O_RDONLY, 0);
    if (fd == -1) {
        ESP_LOGE (REST_TAG, "Failed to open file : %s", filepath);
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to read existing file");
        return ESP_FAIL;
    }

    set_content_type_from_file(req, filepath);

    char *chunk = rest_context->scratch;
    ssize_t read_bytes;
    do {
        /* Read file in chunks into the scratch buffer */
        read_bytes = read(fd, chunk, SCRATCH_BUFSIZE);
        if (read_bytes == -1) {
            ESP_LOGE (REST_TAG, "Failed to read file : %s", filepath);
        } else if (read_bytes > 0) {
            /* Send the buffer contents as HTTP response chunk */
            if (httpd_resp_send_chunk(req, chunk, read_bytes) != ESP_OK) {
                close(fd);
                ESP_LOGE (REST_TAG, "File sending failed!");
                /* Abort sending file */
                httpd_resp_sendstr_chunk(req, NULL);
                /* Respond with 500 Internal Server Error */
                httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");
                return ESP_FAIL;
            }
        }
    } while (read_bytes > 0);
    /* Close file after sending complete */
    close(fd);
    ESP_LOGI (REST_TAG, "File sending complete");
    /* Respond with an empty chunk to signal HTTP response completion */
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}


static void led_main(uint32_t red, uint32_t green, uint32_t blue) {

#ifdef CONFIG_IDF_TARGET_ESP32C3
    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(CONFIG_EXAMPLE_RMT_TX_GPIO, RMT_TX_CHANNEL);
    // set counter clock to 40MHz
    config.clk_div = 2;

    ESP_ERROR_CHECK (rmt_config(&config));
    ESP_ERROR_CHECK (rmt_driver_install(config.channel, 0, 0));

    // install ws2812 driver
    led_strip_config_t strip_config = LED_STRIP_DEFAULT_CONFIG(CONFIG_EXAMPLE_STRIP_LED_NUMBER,
                                                               (led_strip_dev_t) config.channel);
    led_strip_t *strip = led_strip_new_rmt_ws2812(&strip_config);
    if (!strip) {
        ESP_LOGE (TAG, "install WS2812 driver failed");
    }
    // Clear LED strip (turn off all LEDs)

    for (int i = 0; i < 3; i++) {
        for (int j = i; j < CONFIG_EXAMPLE_STRIP_LED_NUMBER; j += 3) {
            // Build RGB values

            // Write RGB values to strip driver
            ESP_ERROR_CHECK (strip->set_pixel(strip, j, red, green, blue));
        }

        // Flush RGB values to LEDs
        ESP_ERROR_CHECK (strip->refresh(strip, 100));

    }

    rmt_driver_uninstall(config.channel);
#endif 
}

/* Simple handler for light brightness control */
static esp_err_t light_brightness_post_handler(httpd_req_t *req) {

    int total_len = req->content_len;
    int cur_len = 0;
    char *buf = ((rest_server_context_t *) (req->user_ctx))->scratch;
    int received = 0;
    if (total_len >= SCRATCH_BUFSIZE) {
        /* Respond with 500 Internal Server Error */
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "content too long");
        return ESP_FAIL;
    }
    while (cur_len < total_len) {
        received = httpd_req_recv(req, buf + cur_len, total_len);
        if (received <= 0) {
            /* Respond with 500 Internal Server Error */
            httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to post control value");
            return ESP_FAIL;
        }
        cur_len += received;
    }
    buf[total_len] = '\0';

    cJSON *root = cJSON_Parse(buf);
    int red = cJSON_GetObjectItem(root, "red")->valueint;
    int green = cJSON_GetObjectItem(root, "green")->valueint;
    int blue = cJSON_GetObjectItem(root, "blue")->valueint;
    ESP_LOGI (REST_TAG, "Light control: red = %d, green = %d, blue = %d", red, green, blue);
    led_main(red, green, blue);
    cJSON_Delete(root);
    httpd_resp_sendstr(req, "Post control value successfully");
    return ESP_OK;
}

extern char *menus;


/* Simple handler for getting system handler */
esp_err_t menu_get_handler(httpd_req_t *req) {
    uint8_t MAC[6];

    httpd_resp_set_type(req, "application/json");
    cors_header(req);


    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "username", "admin");


    cJSON_AddStringToObject(root, "token", "ac21ebab-bddc-41a3-bef5-4ecf3325c888");

    esp_base_mac_addr_get(MAC);
    sprintf(buf, "%02X.%02X.%02X.%02X.%02X.%02X", MAC[0], MAC[1], MAC[2], MAC[3], MAC[4], MAC[5]);
    cJSON_AddStringToObject(root, "MAC", buf);
    cJSON_AddItemToObject(root, "permissions", cJSON_CreateArray());

    cJSON_AddItemToObject(root, "menus", createMenus());

    const char *sys_info = cJSON_Print(root);
    httpd_resp_sendstr(req, sys_info);
    free((void *) sys_info);
    cJSON_Delete(root);
    return ESP_OK;
}


const struct {
    char *path;

    esp_err_t (*handler)(httpd_req_t *);

    int method;
}

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))


// https://www.esp32.com/viewtopic.php?t=12649
//  The GET method requests a representation of the specified resource. Requests using GET should only retrieve data.
//        The HEAD method asks for a response identical to a GET request, but without the response body.
//        The POST method submits an entity to the specified resource, often causing a change in state or side effects on the server.
//        The PUT method replaces all current representations of the target resource with the request payload.
//        The DELETE method deletes the specified resource.
//        The CONNECT method establishes a tunnel to the server identified by the target resource.
//        The OPTIONS method describes the communication options for the target resource.
//        The TRACE method performs a message loop-back test along the path to the target resource.
//        The PATCH method applies partial modifications to a resource.


static commandPath[] = {
        /* URI handler for getting web server files */

        {"/api/v1/light/brightness", cors_handler,                  HTTP_OPTIONS},
        {"/api/v1/light/brightness", light_brightness_post_handler, HTTP_POST},
        {"/api/v1/temp/raw",         cors_handler,                  HTTP_OPTIONS},
        //{"/api/v1/temp/raw", temperature_data_get_handler, HTTP_GET},
        {"/api/v1/temp/raw",         temperatureBmp3xx,             HTTP_GET},

        {"/api/v1/configuration",    cors_handler,                  HTTP_OPTIONS},
        {"/api/v1/configuration",    configuration_get_handler,     HTTP_GET},

        {"/api/v1/system/info",      cors_handler,                  HTTP_OPTIONS},
        {"/api/v1/system/info",      system_info_get_handler,       HTTP_GET},

        {"/api/v1/system/menu",      cors_handler,                  HTTP_OPTIONS},
        {"/api/v1/system/menu",      menu_get_handler,              HTTP_GET},

        {"/api/v1/i2c",              cors_handler,                  HTTP_OPTIONS},
        {"/api/v1/i2c",              i2c_patch_handler,             HTTP_PATCH},

        //   {"/api/v1/nvram",            cors_handler,                  HTTP_OPTIONS},
        //{"/api/v1/nvram",            nvram_patch_handler,           HTTP_PATCH},
        // { "/status",OTA_update_status_handler,HTTP_POST},
        // { "/wifi",get_ssid_handler,HTTP_GET},
        //  {"/*", rest_common_get_handler, HTTP_GET},
};


esp_err_t start_rest_server(const char *base_path) {
    led_main(10, 0, 0);

    REST_CHECK (base_path, "wrong base path", err);
    rest_server_context_t *rest_context = calloc(1, sizeof(rest_server_context_t));

    REST_CHECK (rest_context, "No memory for rest context", err);
    strlcpy(rest_context->base_path, base_path, sizeof(rest_context->base_path));

    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG ();
    config.max_uri_handlers = 16;
    config.uri_match_fn = httpd_uri_match_wildcard;

    ESP_LOGI (REST_TAG, "Starting HTTP Server");
    REST_CHECK (httpd_start(&server, &config) == ESP_OK, "Start server failed", err_start);

//   server.enableCORS();
    /////////////////////////////////////////////
    for (int n = 0; n < NELEMS (commandPath); n++) {
        httpd_uri_t uri = {.uri = commandPath[n].path, .method = commandPath[n].method, .handler = commandPath[n].handler, .user_ctx = rest_context};
        httpd_register_uri_handler(server, &uri);
    }
    return ESP_OK;
    err_start:
    free(rest_context);
    err:
    return ESP_FAIL;
}
