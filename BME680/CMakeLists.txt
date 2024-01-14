cmake_minimum_required(VERSION 3.10)
project(BME680)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_library(BME680 BME680_i2c.cpp BME680_i2c.h BME680_register_address.h)

target_link_libraries(BME680 PUBLIC pico_stdlib hardware_i2c hardware_adc hardware_pwm hardware_spi hardware_uart)