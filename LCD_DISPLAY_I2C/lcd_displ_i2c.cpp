//
// Created by kubab on 07/01/2024.
//

#include <string>
#include "lcd_displ_i2c.h"

lcd_displ_i2c::lcd_displ_i2c(uint8_t sda_pin, uint8_t scl_pin, uint8_t i2c_address, i2c_inst_t *i2c_port,
                             uint32_t i2c_frequency,
                             uint8_t max_row, uint8_t max_col) {
    this->i2c_sda = sda_pin;
    this->i2c_scl = scl_pin;
    this->i2c_address = i2c_address;
    this->i2c = i2c_port;
    this->i2c_frequency = i2c_frequency;
    this->max_row = max_row;
    this->max_col = max_col;
}

lcd_displ_i2c::~lcd_displ_i2c() {
    i2c_deinit(this->i2c);
}

void lcd_displ_i2c::init() {
    // if i2c isn't initialized
    if (gpio_get_function(this->i2c_sda) != GPIO_FUNC_I2C &&
            gpio_get_function(this->i2c_scl) != GPIO_FUNC_I2C) {
        i2c_init(this->i2c, this->i2c_frequency);
        gpio_set_function(this->i2c_sda, GPIO_FUNC_I2C);
        gpio_set_function(this->i2c_scl, GPIO_FUNC_I2C);
        gpio_pull_up(this->i2c_sda);
        gpio_pull_up(this->i2c_scl);
        this->is_init = true;
    } else {
        this->is_init = false;
    }

    // initialization of LCD display
    this->send_byte(0x03, LCD_COMMAND);
    this->send_byte(0x03, LCD_COMMAND);
    this->send_byte(0x03, LCD_COMMAND);
    this->send_byte(0x02, LCD_COMMAND);

    this->send_byte(LCD_ENTRY_MODE_SET | LCD_ENTRY_LEFT, LCD_COMMAND);
    this->send_byte(LCD_FUNCTION_SET | LCD_2LINE, LCD_COMMAND);
    this->send_byte(LCD_DISPLAY_CONTROL | LCD_DISPLAY_ON, LCD_COMMAND);

    this->clear();
}

bool lcd_displ_i2c::is_init_ok() const {
    return this->is_init;
}

void lcd_displ_i2c::i2c_write_byte(uint8_t data) {
    i2c_write_blocking(this->i2c, this->i2c_address, &data, 1, false);
}

void lcd_displ_i2c::toggle_enable(uint8_t data) {
    sleep_us(DELAY_US);
    this->i2c_write_byte(data | LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
    this->i2c_write_byte(data & ~LCD_ENABLE_BIT);
    sleep_us(DELAY_US);
}

void lcd_displ_i2c::send_byte(uint8_t data, uint8_t mode) {
    uint8_t high = mode | (data & 0xF0) | LCD_BACKLIGHT;
    uint8_t low = mode | ((data << 4) & 0xF0) | LCD_BACKLIGHT;

    this->i2c_write_byte(high);
    this->toggle_enable(high);
    this->i2c_write_byte(low);
    this->toggle_enable(low);


}

void lcd_displ_i2c::clear() {
    this->send_byte(LCD_CLEAR_DISPLAY, LCD_COMMAND);
}

void lcd_displ_i2c::set_cursor(uint8_t row, uint8_t col) {
    if (this->max_row < row || this->max_col < col) {
        return;
    }

    uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    this->send_byte(LCD_SET_DDRAM_ADDR | (col + row_offsets[row]), LCD_COMMAND);
}

void lcd_displ_i2c::char_write(char data) {
    this->send_byte(data, LCD_CHARACTER);
}

void lcd_displ_i2c::print_string(const std::string& data) {
    for (char c : data) {
        this->char_write(c);
    }
}

void lcd_displ_i2c::print_string_wrapped(const std::string& data) {
    uint8_t row = 0;
    uint8_t col = 0;
    for (char c : data) {
        if (c == '\n') {
            row++;
            col = 0;
            if (row > this->max_row) {
                break;
            }
            this->set_cursor(row, col);
        } else {
            this->char_write(c);
            col++;
            if (col > this->max_col-1) {
                row++;
                col = 0;
                if (row > this->max_row) {
                    break;
                }
                this->set_cursor(row, col);
            }
        }
    }

}


