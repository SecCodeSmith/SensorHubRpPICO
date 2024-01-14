cmake_minimum_required(VERSION 3.10)
project(BME688)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_library(BME688 BME688.cpp BME688.h)

target_link_libraries(BME688 PUBLIC pico_stdlib hardware_i2c hardware_adc hardware_pwm hardware_spi hardware_uart)