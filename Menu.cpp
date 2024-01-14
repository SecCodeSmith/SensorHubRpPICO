//
// Created by kubab on 08/01/2024.
//

#include <string>
#include <cmath>
#include "Menu.h"


void Menu::next_menu() {
    switch (this->current_stage.current_menu) {
        case menu::main:
            switch (this->current_stage.current_main_menu) {
                case main_menu::tempeture:
                    this->current_stage.current_main_menu = main_menu::pressure;
                    break;
                case main_menu::pressure:
                    this->current_stage.current_main_menu = main_menu::humidity;
                    break;
                case main_menu::humidity:
                    this->current_stage.current_main_menu = main_menu::gas_resistance;
                    break;
                case main_menu::gas_resistance:
                    this->current_stage.current_main_menu = main_menu::tempeture;
                    break;
                case main_menu::air_quality:
                    this->current_stage.current_main_menu = main_menu::tempeture;
                    break;
            }
            break;
        case menu::settings:
            switch (this->current_stage.current_settings_menu){
                case settings_menu::tempeture:
                    this->current_stage.current_settings_menu = settings_menu::buzzer;
                    break;
                case settings_menu::pressure:
                    this->current_stage.current_settings_menu = settings_menu::tempeture;
                    break;
                case settings_menu::refresh_time:
                    this->current_stage.current_settings_menu = settings_menu::pressure;
                    break;
                case settings_menu::buzzer:
                    this->current_stage.current_settings_menu = settings_menu::refresh_time;
                    break;
            }
            break;
    }
}

void Menu::prev_menu() {
    switch (this->current_stage.current_menu) {
        case menu::main:
            switch (this->current_stage.current_main_menu) {
                case main_menu::tempeture:
                    this->current_stage.current_main_menu = main_menu::gas_resistance;
                    break;
                case main_menu::pressure:
                    this->current_stage.current_main_menu = main_menu::tempeture;
                    break;
                case main_menu::humidity:
                    this->current_stage.current_main_menu = main_menu::pressure;
                    break;
                case main_menu::gas_resistance:
                    this->current_stage.current_main_menu = main_menu::humidity;
                    break;
                case main_menu::air_quality:
                    this->current_stage.current_main_menu = main_menu::gas_resistance;
                    break;
            }
            break;
        case menu::settings:
            switch (this->current_stage.current_settings_menu){
                case settings_menu::tempeture:
                    this->current_stage.current_settings_menu = settings_menu::pressure;
                    break;
                case settings_menu::pressure:
                    this->current_stage.current_settings_menu = settings_menu::refresh_time;
                    break;
                case settings_menu::refresh_time:
                    this->current_stage.current_settings_menu = settings_menu::buzzer;
                    break;
                case settings_menu::buzzer:
                    this->current_stage.current_settings_menu = settings_menu::tempeture;
                    break;
            }
            break;
    }
}

void Menu::go_to_main_menu() {
    this->current_stage.current_menu = menu::main;
}

void Menu::go_to_settings_menu() {
    this->current_stage.current_menu = menu::settings;
}
std::string str;
void Menu::print_menu(lcd_displ_i2c *lcd, BME688_data_float *data, SETTINGS *setting) {
    lcd->clear();
    switch (this->current_stage.current_menu) {
        case menu::main:
            switch (this->current_stage.current_main_menu) {
                case main_menu::tempeture:
                    switch (setting->temperature_unit) {
                        default:
                        case TEMPERATURE_UNIT::celsius:
                            str = std::to_string(std::round(data->temperature * 100) / 100) + "C";
                            break;
                        case TEMPERATURE_UNIT::fahrenheit:
                            str = std::to_string(std::round((data->temperature * 9 / 5 + 32) * 100) / 100) + "F";
                            break;
                        case TEMPERATURE_UNIT::kelvin:
                            str = std::to_string(std::round((data->temperature + 273.15) * 100) / 100) + "K";
                            break;

                    }
                    lcd->print_string_wrapped("Temperature: " + str );
                    break;
                case main_menu::pressure:
                    switch (setting->pressure_unit){
                        case PRESSURE_UNIT::hPa:
                            str = std::to_string(std::round(data->pressure * 100) / 100) + "hPa";
                            break;
                        case PRESSURE_UNIT::mmHg:
                            str = std::to_string(std::round((data->pressure * 0.750062) * 100) / 100) + "mmHg";
                            break;
                        case PRESSURE_UNIT::atm:
                            str = std::to_string(std::round((data->pressure * 0.000986923) * 100) / 100) + "atm";
                            break;
                        case PRESSURE_UNIT::psi:
                            str = std::to_string(std::round((data->pressure * 0.0145038) * 100) / 100) + "psi";
                            break;
                    }
                    lcd->print_string_wrapped("Pressure: " + str);
                    break;
                case main_menu::humidity:
                    lcd->print_string_wrapped("Humidity: " + std::to_string(std::round(data->humidity * 100) / 100) + '%');
                    break;
                case main_menu::gas_resistance:
                    lcd->print_string_wrapped("Gas resistance: " + std::to_string(std::round(data->gas_resistance * 100) / 100) + "Ohm");
                    break;
                case main_menu::air_quality:
                    lcd->print_string_wrapped("Air quality: " + std::to_string(std::round(data->air_quality * 100) / 100) + '%');
                    break;
            }
            break;
        case menu::settings:
            switch (this->current_stage.current_settings_menu){
                case settings_menu::tempeture:
                    str = "";
                    switch (setting->temperature_unit) {
                        case TEMPERATURE_UNIT::celsius:
                            str = "C";
                            break;
                        case TEMPERATURE_UNIT::fahrenheit:
                            str = "F";
                            break;
                        case TEMPERATURE_UNIT::kelvin:
                            str = "K";
                            break;
                    }
                    lcd->print_string_wrapped("Temperature unit: " + str);
                    break;

                case settings_menu::pressure:
                    switch (setting->pressure_unit){
                        case PRESSURE_UNIT::hPa:
                            str = "hPa";
                            break;
                        case PRESSURE_UNIT::mmHg:
                            str = "mmHg";
                            break;
                        case PRESSURE_UNIT::atm:
                            str = "atm";
                            break;
                        case PRESSURE_UNIT::psi:
                            str = "psi";
                            break;
                    }
                    lcd->print_string_wrapped("Pressure unit: " + str);
                    break;

                case settings_menu::refresh_time:
                    lcd->print_string_wrapped("Refresh time: " + std::to_string(setting->refresh_time_ms / 1000) + "s");
                    break;

                case settings_menu::buzzer:
                    str = (setting->buzzer_on ? "ON" : "OFF");
                    lcd->print_string_wrapped("Buzzer: " + str);
                    break;
            }
            break;
    }
}

stage Menu::get_current_stage() {
    return this->current_stage;
}

void Menu::next_stage() {
    switch (this->current_stage.current_menu) {
        case menu::main:
            this->current_stage.current_menu = menu::settings;
            break;
        case menu::settings:
            this->current_stage.current_menu = menu::main;
            break;
    }
}
