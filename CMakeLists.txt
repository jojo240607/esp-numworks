cmake_minimum_required(VERSION 3.16)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_MESSAGE_LOG_LEVEL DEBUG)
add_compile_definitions(ASSERTIONS=1
        ION_DISPLAY_WIDTH=320
        ION_DISPLAY_HEIGHT=240
        ION_DISPLAY_BORDER=0
        ION_KEYBOARD_ROWS=9
        ION_KEYBOARD_COLUMNS=6
        PLATFORM_DEVICE=1
        ULAB_CONFIG_FILE="\"numworks_ulab_config.h\"")
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(i80_controller VERSION 1.0.0 LANGUAGES C CXX)


