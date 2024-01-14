#include <string>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/i2c.h"

#include "BME688/BME688.h"
#include "button/button.h"
#include "rgb_diode/rgb_diode.h"
#include "LCD_DISPLAY_I2C/lcd_displ_i2c.h"
#include "buzzer/buzzer.h"
#include "Menu.h"

lcd_displ_i2c lcd(0, 1, 0x27, i2c0, 1000000, 2, 16);
BME688 bme688(0,1,0x77, i2c0, 1000000);
rgb_diode diode(2, 3, 4);
buzzer buzzer(9);

button down(11);
button right(12);
button center(13);
button up(14);
button left(15);

static uint32_t lastExecTimeRefresh = to_ms_since_boot(get_absolute_time());
static uint32_t lastExecTimeButtonPress = to_ms_since_boot(get_absolute_time());
static uint32_t lastNextMenuTime = to_ms_since_boot(get_absolute_time());

BME688_data_float data;
SETTINGS settings;

Menu menu;

void main_loop() {
    uint32_t currentTime = to_ms_since_boot(get_absolute_time());
    BME688_data_float data;
    bool new_data = false;

    if (bme688.new_data()) {
        data = bme688.get_data_float();
        new_data = true;
    }

    if ((currentTime - lastExecTimeRefresh >= settings.refresh_time_ms) && new_data) {
        menu.print_menu(&lcd, &data, &settings);
        lastExecTimeRefresh = currentTime;
    }

    if (currentTime - lastExecTimeButtonPress >= 400) {
        if (down.is_pressed()) {
            menu.next_menu();
            menu.print_menu(&lcd, &data, &settings);
        } else if (right.is_pressed()) {
            if (menu.get_current_stage().current_menu == menu::settings) {
                if (settings.buzzer_on) {
                    buzzer.beep(1);
                }
                switch (menu.get_current_stage().current_settings_menu) {
                    case settings_menu::tempeture:
                        settings.temperature_unit = static_cast<TEMPERATURE_UNIT>(
                                (static_cast<int>(settings.temperature_unit) + 1) % 3);
                        break;
                    case settings_menu::pressure:
                        settings.pressure_unit = static_cast<PRESSURE_UNIT>(
                                (static_cast<int>(settings.pressure_unit) + 1) % 4);
                        break;
                    case settings_menu::refresh_time:
                        settings.refresh_time_ms = (settings.refresh_time_ms + 1000) % 10000;
                        break;
                    case settings_menu::buzzer:
                        settings.buzzer_on = !settings.buzzer_on;
                        break;
                }
            }
            menu.print_menu(&lcd, &data, &settings);
        } else if (center.is_pressed()) {
            if (settings.buzzer_on) {
                buzzer.beep(1);
            }
            menu.next_stage();
            menu.print_menu(&lcd, &data, &settings);
        } else if (up.is_pressed()) {
            if (settings.buzzer_on) {
                buzzer.beep(1);
            }
            menu.prev_menu();
            menu.print_menu(&lcd, &data, &settings);
        } else if (left.is_pressed()) {
            if (settings.buzzer_on) {
                buzzer.beep(1);
            }
            if (menu.get_current_stage().current_menu == menu::settings) {
                switch (menu.get_current_stage().current_settings_menu) {
                    case settings_menu::tempeture:
                        settings.temperature_unit = static_cast<TEMPERATURE_UNIT>(
                                (static_cast<int>(settings.temperature_unit) + 2) % 3);
                        break;
                    case settings_menu::pressure:
                        settings.pressure_unit = static_cast<PRESSURE_UNIT>(
                                (static_cast<int>(settings.pressure_unit) + 3) % 4);
                        break;
                    case settings_menu::refresh_time:
                        settings.refresh_time_ms = (settings.refresh_time_ms + 9000) % 10000;
                        break;
                    case settings_menu::buzzer:
                        settings.buzzer_on = !settings.buzzer_on;
                        break;
                }
            }
            menu.print_menu(&lcd, &data, &settings);
        }

        lastExecTimeButtonPress = currentTime;
        lastExecTimeRefresh = currentTime;
    }
}

int main() {
    settings.refresh_time_ms = 1000;
    settings.temperature_unit = TEMPERATURE_UNIT::celsius;
    settings.pressure_unit = PRESSURE_UNIT::hPa;
    settings.buzzer_on = false;

    i2c_init(i2c0, 1000000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0);
    gpio_pull_up(1);

    lcd.init();
    lcd.print_string("Initialize...");
    bme688.init();
    lcd.clear();
    if (!bme688.is_init_ok()) {
        lcd.print_string("Sensor BME688 ERROR");
        return 1;
    } else {
        lcd.print_string("Sensor BME688 OK");
        bme688.parallel_mode();
    }

    diode.init();
    diode.white();
    lcd.clear();
    lcd.print_string("RGB diode OK");
    lcd.clear();
    lcd.print_string("Initialize OK");
    buzzer.beep(50);

    while (1)
        main_loop();

    return 0;
}