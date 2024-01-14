cmake_minimum_required(VERSION 3.26)

set(PICO_BOARD pico_w)

include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)

project(Project C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_RELEASE "-pipe -Os -fno-builtin")
set(CMAKE_C_FLAGS_RELEASE "-pipe -Os -fno-builtin")

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif ()

pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
)

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif ()

add_subdirectory(BME680)
add_subdirectory(BME688)
add_subdirectory(button)
add_subdirectory(rgb_diode)
add_subdirectory(LCD_DISPLAY_I2C)
add_subdirectory(buzzer)

add_executable(Project main.cpp
        Menu.cpp
        Menu.h
)

target_link_libraries(Project PUBLIC buzzer BME688 button rgb_diode LCD_DISPLAY_I2C
        pico_stdlib hardware_i2c hardware_adc hardware_pwm hardware_spi hardware_uart)

pico_add_extra_outputs(Project)