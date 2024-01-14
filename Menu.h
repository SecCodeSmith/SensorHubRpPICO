//
// Created by kubab on 08/01/2024.
//

#ifndef PROJECTPMC_MENU_H
#define PROJECTPMC_MENU_H
#include "pico/stdlib.h"
#include "LCD_DISPLAY_I2C/lcd_displ_i2c.h"
#include "BME688/BME688.h"

enum class menu {
    main, settings
};

enum class main_menu {
    tempeture, pressure, humidity, gas_resistance, air_quality
};

enum class settings_menu {
    tempeture, pressure, refresh_time, buzzer
};

struct stage {
    menu current_menu;
    main_menu current_main_menu;
    settings_menu current_settings_menu;
};

enum class TEMPERATURE_UNIT{
    celsius, fahrenheit, kelvin
};

enum class PRESSURE_UNIT {
    hPa, mmHg, atm, psi
};


struct SETTINGS {
    uint32_t refresh_time_ms;
    TEMPERATURE_UNIT temperature_unit;
    PRESSURE_UNIT pressure_unit;
    bool buzzer_on;
};

class Menu {
private:
protected:
    stage current_stage = {menu::main, main_menu::tempeture, settings_menu::tempeture};
public:
    void print_menu(lcd_displ_i2c *lcd, BME688_data_float *data, SETTINGS *setting);
    void next_menu();
    void prev_menu();
    void go_to_main_menu();
    void go_to_settings_menu();
    void next_stage();
    stage get_current_stage();
};


#endif //PROJECTPMC_MENU_H
