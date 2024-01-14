cmake_minimum_required(VERSION 3.10)
project(buzzer)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_library(buzzer buzzer.cpp buzzer.h)

target_link_libraries(buzzer PUBLIC pico_stdlib hardware_i2c hardware_adc hardware_pwm hardware_spi hardware_uart)