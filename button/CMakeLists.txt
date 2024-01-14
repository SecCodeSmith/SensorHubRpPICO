cmake_minimum_required(VERSION 3.10)
project(button)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_library(button button.cpp button.h)

target_link_libraries(button PUBLIC pico_stdlib hardware_i2c hardware_adc hardware_pwm hardware_spi hardware_uart)