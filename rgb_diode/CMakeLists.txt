cmake_minimum_required(VERSION 3.10)
project(rgb_diode)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_library(rgb_diode rgb_diode.cpp)

target_link_libraries(rgb_diode PUBLIC pico_stdlib hardware_i2c hardware_adc hardware_pwm hardware_spi hardware_uart)