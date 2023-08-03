idf_component_register(SRCS "cmd_wifi.c"
        "scan_wifi.c"
        INCLUDE_DIRS .
        REQUIRES console esp_wifi nvs_flash)
