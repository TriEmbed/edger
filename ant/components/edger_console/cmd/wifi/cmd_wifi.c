/*```````````````
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
/* Console example — WiFi commands

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
#include <esp_wifi_types.h>
#include "esp_log.h"
#include "esp_console.h"
#include "argtable3/argtable3.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "cmd_wifi.h"

#include "../nvs/cmd_nvs.h"

extern int wifi_scan(int argc, char **argv);

#define JOIN_TIMEOUT_MS (10000)

static void initialise_wifi(void);


static EventGroupHandle_t wifi_event_group;
const int CONNECTED_BIT = BIT0;
/** Arguments used by 'join' function */
static struct {
    struct arg_int *timeout;
    struct arg_str *ssid;
    struct arg_str *password;
    struct arg_end *end;
} join_args;




static int connect(int argc, char **argv) {
    puts("foo 1\n");
    int nerrors = arg_parse(argc, argv, (void **) &join_args);
    puts("foo 2\n");
    if (nerrors != 0) {
        puts("foo 3\n");
        arg_print_errors(stderr, join_args.end, argv[0]);
        return 1;
    }
    puts("foo 4\n");
    ESP_LOGI(__func__, "Connecting to '%s'",
             join_args.ssid->sval[0]);

     if (join_args.timeout->count == 0) {
         puts("foo 5\n");
         join_args.timeout->ival[0] = JOIN_TIMEOUT_MS;
     }

    printf("ssid: %s password: %s\n", join_args.ssid->sval[0], join_args.password->sval[0]);
    set_wifi(join_args.ssid->sval[0],join_args.password->sval[0],join_args.timeout->ival[0] ) ;


    ESP_LOGI(__func__, "Connected");
    return 0;
}

void register_wifi(void) {
      printf("register_wifi called\n");
//    set_ssid_args.ssid = arg_str1(NULL, NULL, "<ssid>", "SSID of AP");
//    set_ssid_args.password = arg_str0(NULL, NULL, "<pass>", "PSK of AP");
//    set_ssid_args.end = arg_end(2);
//    set_ssid_args.timeout = arg_int0(NULL, "timeout", "<t>", "Connection timeout, ms");
//    set_ssid_args.ssid = arg_str1(NULL, NULL, "<ssid>", "SSID of AP");
//    set_ssid_args.password = arg_str0(NULL, NULL, "<pass>", "PSK of AP");
//    set_ssid_args.end = arg_end(2);
//    const esp_console_cmd_t join_cmd = {
//            .command = "set ssid",
//            .help = "WiFi AP set a station",
//            .hint = NULL,
//            .func = &set_ssid,
//            .argtable = &set_ssid_args
//    };
//
//    ESP_ERROR_CHECK(esp_console_cmd_register(&join_cmd));
    join_args.timeout = arg_int0(NULL, "timeout", "<t>", "Connection timeout, ms");
    join_args.ssid = arg_str1(NULL, NULL, "<ssid>", "SSID of AP");
    join_args.password = arg_str0(NULL, NULL, "<pass>", "PSK of AP");
    join_args.end = arg_end(2);

    const esp_console_cmd_t join_cmd = {
            .command = "join",
            .help = "Join WiFi AP as a station",
            .hint = NULL,
            .func = &connect,
            .argtable = &join_args
    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&join_cmd) );
    const esp_console_cmd_t scan_cmd = {
            .command = "scan",
            .help = "look for ssid",
            .hint = NULL,
            .func = &wifi_scan,
            .argtable = NULL
    };

    ESP_ERROR_CHECK(esp_console_cmd_register(&scan_cmd));
}
