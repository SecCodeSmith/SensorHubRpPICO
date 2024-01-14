//
// Created by kubab on 07/01/2024.
//

#ifndef PROJECTPMC_LCD_DISPL_I2C_H
#define PROJECTPMC_LCD_DISPL_I2C_H

#include <cstring>
#include "lcd_const.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

class lcd_displ_i2c {
private:
protected:
    uint8_t i2c_address;
    uint8_t i2c_sda;
    uint8_t i2c_scl;
    uint8_t i2c_frequency;
    uint8_t max_row, max_col;
    i2c_inst_t *i2c;
    bool is_init = false;

    bool is_init_ok() const;

    void i2c_write_byte(uint8_t data);
    void toggle_enable(uint8_t data);
    void send_byte(uint8_t data, uint8_t mode);

public:
    lcd_displ_i2c(uint8_t sda_pin, uint8_t scl_pin, uint8_t i2c_address, i2c_inst_t *i2c_port, uint32_t i2c_frequency,
                  uint8_t max_row, uint8_t max_col);
    ~lcd_displ_i2c();
    void init();

    void clear();
    void set_cursor(uint8_t row, uint8_t col);
    void char_write(char data);
    void print_string(const std::string& data);
    void print_string_wrapped(const std::string& data);
};


#endif //PROJECTPMC_LCD_DISPL_I2C_H
