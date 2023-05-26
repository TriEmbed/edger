cmake_minimum_required(VERSION 3.5)

# This example uses an extra component for common functions such as Wi-Fi and Ethernet connection.
set(EXTRA_COMPONENT_DIRS $ENV{IDF_PATH}/examples/common_components/protocol_examples_common
        main/components/myconsole components/gps  $ENV{IDF_PATH}/examples/common_components/led_strip)

include_directories($ENV{IDF_PATH}/components/sdmmc/include
        $ENV{IDF_PATH}/components/fatfs/vfs
        $ENV{IDF_PATH}/components/nvs_flash/include)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(edger)

