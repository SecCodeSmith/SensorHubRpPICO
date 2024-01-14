cmake_minimum_required(VERSION 3.10)
project(LCD_DISPLAY_I2C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_library(LCD_DISPLAY_I2C
        lcd_displ_i2c.h lcd_displ_i2c.cpp lcd_const.h)

target_link_libraries(LCD_DISPLAY_I2C PUBLIC pico_stdlib hardware_i2c hardware_adc hardware_pwm hardware_spi hardware_uart)