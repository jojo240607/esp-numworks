cmake_minimum_required(VERSION 3.25)
project(numworks2 C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

add_compile_definitions(ASSERTIONS=1
        ION_DISPLAY_WIDTH=320
        ION_DISPLAY_HEIGHT=240
        ION_DISPLAY_BORDER=0
        ION_KEYBOARD_ROWS=9
        ION_KEYBOARD_COLUMNS=6
        PLATFORM_DEVICE=1
        ULAB_CONFIG_FILE="\"numworks_ulab_config.h\"")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_subdirectory(omg)
add_subdirectory(kandinsky)
add_subdirectory(ion)
add_subdirectory(poincare)
add_subdirectory(escher)
add_subdirectory(python)
add_subdirectory(apps)

add_executable(numworks2 main.cpp
        hardware/device_def.c
        hardware/keyinput/gpio.c
        hardware/keyinput/serialconv.c
        hardware/keyinput/machine.c
        hardware/keyinput/key_methine.c
        hardware/sdl/keyboard/key_pad.c
        hardware/sdl/ui/view.c
        hardware/sdl/ui/button.c
        hardware/sdl/display/display.c
        hardware/sdl/display/memory_canvas.c
        hardware/sdl/screen/screen.c
        hardware/self_strcpy.c)

target_include_directories(numworks2
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/../ion/include
)

target_link_libraries(numworks2
        omg
        kandinsky
        ion
        poincare
        escher
        python
        apps
        #gmp
        #mpfi
        #mpfr
        #gcas2
        ${SDL2_LIBRARIES}
)
