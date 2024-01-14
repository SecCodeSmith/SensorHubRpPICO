//
// Created by kubab on 05/01/2024.
//

#include <hardware/gpio.h>
#include "BME688.h"

BME688::BME688(uint8_t sda_pin, uint8_t scl_pin, uint8_t i2c_address, i2c_inst_t *i2c_port, uint32_t i2c_frequency) {
    this->sda_pin = sda_pin;
    this->scl_pin = scl_pin;
    this->i2c_address = i2c_address;
    this->i2c = i2c_port;
    this->i2c_frequency = i2c_frequency;
}

bool BME688::init() {
    if (gpio_get_function(this->sda_pin) != GPIO_FUNC_I2C || gpio_get_function(this->scl_pin) != GPIO_FUNC_I2C) {
        gpio_set_function(this->sda_pin, GPIO_FUNC_I2C);
        gpio_set_function(this->scl_pin, GPIO_FUNC_I2C);
        gpio_pull_up(this->sda_pin);
        gpio_pull_up(this->scl_pin);
        i2c_init(this->i2c, this->i2c_frequency);
    }

    int id = read8(this->memory_map_i2c.chip_id);

    if (id != this->memory_map_i2c.chip_id_value) {
        return init_ok;
    }

    //i2c_set_slave_mode(this->i2c, true, this->i2c_address);

    this->init_ok = true;

    return this->init_ok;
}

uint8_t BME688::read8(uint8_t reg) {
    uint8_t data;
    i2c_write_blocking(this->i2c, this->i2c_address, &reg, 1, true);
    i2c_read_blocking(this->i2c, this->i2c_address, &data, 1, false);
    return data;
}

void BME688::write8(uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};
    i2c_write_blocking(this->i2c, this->i2c_address, data, 2, false);
}

void BME688::soft_reset() {
    write8(this->memory_map_i2c.reset, 0xb6);
}

void BME688::force_mode() {
    this->set_humidity_oversampling(0b001);
    this->set_temperature_oversampling(0b010);
    this->set_pressure_oversampling(0b101);

    this->set_gas_wait(0, 0x59);
    this->set_nb_conv(0x0);
    this->set_run_gas();

    this->set_mode(BME688_mode::forced);
}

void BME688::set_humidity_oversampling(uint8_t value) {
    uint8_t reg = read8(this->memory_map_i2c.ctr_hum);

    value = value & 0b00000111;
    reg = reg | value;
    reg = reg & 0b11111000;

    write8(this->memory_map_i2c.ctr_hum, reg);
}

void BME688::set_temperature_oversampling(uint8_t value) {
    uint8_t reg = read8(this->memory_map_i2c.ctr_meas);

    value = value << 5;
    reg = reg & 0b00011111;
    reg = reg | value;

    write8(this->memory_map_i2c.ctr_meas, reg);
}

void BME688::set_pressure_oversampling(uint8_t value) {
    uint8_t reg = read8(this->memory_map_i2c.ctr_meas);

    value = value << 2;
    value = value & 0b00011100;
    reg = reg & 0b11100011;
    reg = reg | value;

    write8(this->memory_map_i2c.ctr_meas, reg);
}

void BME688::set_gas_wait(uint8_t x, uint8_t value) {
    write8(this->memory_map_i2c.gas_wait[x], value);
}

void BME688::set_res_heat(uint8_t x, uint8_t value) {
    write8(this->memory_map_i2c.res_heat[x], value);
}

void BME688::set_nb_conv(uint8_t value) {
    uint8_t reg = read8(this->memory_map_i2c.ctr_gas_1);

    value = value & 0b00001111;
    reg = reg & 0b11110000;
    reg = reg | value;

    write8(this->memory_map_i2c.ctr_gas_1, reg);
}

void BME688::set_run_gas() {
    uint8_t reg = read8(this->memory_map_i2c.ctr_gas_1);

    reg = reg | 0b00100000;

    write8(this->memory_map_i2c.ctr_gas_1, reg);
}

void BME688::set_mode(BME688_mode value) {
    if (value == BME688_mode::parallel) {
        this->parallel_mode_enable = true;
    } else {
        this->parallel_mode_enable = false;
    }
    uint8_t reg = read8(this->memory_map_i2c.ctr_meas);
    uint8_t m = (uint8_t) value;
    m = m & 0b00000011;
    reg = reg & 0b11111100;
    reg = reg | m;

    write8(this->memory_map_i2c.ctr_meas, reg);
}

void BME688::parallel_mode() {
    this->set_humidity_oversampling(0b001);
    this->set_temperature_oversampling(0b010);
    this->set_pressure_oversampling(0b101);

    this->write8(this->memory_map_i2c.gas_wait_shared, 0x59);

    this->set_gas_wait_shared(0x59);
    this->set_nb_conv(0x03);
    this->set_run_gas();

    this->set_mode(BME688_mode::parallel);
}

void BME688::sleep_mode() {
    this->set_mode(BME688_mode::sleep);
}

bool BME688::is_init_ok() {
    return this->init_ok;
}

BME688::temperature_calibration BME688::read_temperature_calibration() {
    temperature_calibration calibration;

    calibration.par_t1 = read8(0xe9) | (read8(0xea) << 8);
    calibration.par_t2 = read8(0x8a) | (read8(0x8b) << 8);
    calibration.par_t3 = read8(0x8c);

    return calibration;
}

int32_t BME688::calc_temperature_int(int index) {
    temperature_calibration calibration = this->read_temperature_calibration();

    uint32_t temp_adc = read8(this->memory_map_i2c.temperature_lsb[index]) |
                       (read8(this->memory_map_i2c.temperature_msb[index]) << 8) |
                       (read8(this->memory_map_i2c.temperature_xlsb[index]) << 16);

    int32_t var1 = (((temp_adc >> 3) - ((int32_t) calibration.par_t1 << 1)));
    int32_t var2 = (var1 * ((int32_t) calibration.par_t2)) >> 11;
    int32_t var3 = ((var1 >> 1) * (var1 >> 1)) >> 12 * ((int32_t) calibration.par_t3 << 4) >> 14;

    int32_t t_fine = var2 + var3;
    return (t_fine * 5 + 128) >> 8;
}

float BME688::calc_temperature_float(int index, float *t_fine) {
    temperature_calibration calibration = this->read_temperature_calibration();

    uint32_t temp_adc = read8(this->memory_map_i2c.temperature_lsb[index]) << 8|
                       (read8(this->memory_map_i2c.temperature_msb[index]) << 16) |
                       (read8(this->memory_map_i2c.temperature_xlsb[index]));

    float var1 =
            (((float) temp_adc / 16384.0f) - ((float) calibration.par_t1 / 1024.0f)) * ((float) calibration.par_t2);
    float var2 = ((((float) temp_adc / 131072.0f) - ((float) calibration.par_t1 / 8192.0f))
                  * (((float) temp_adc / 131072.0f) - ((float) calibration.par_t1 / 8192.0f)))
                 * ((float) calibration.par_t3 * 16.0f);
    *t_fine = var1 + var2;

    return *t_fine / 5120.0f / 100;
}

BME688::pressure_calibration BME688::read_pressure_calibration() {
    pressure_calibration calibration;

    calibration.par_p1 = read8(0x8e) | (read8(0x8f) << 8);
    calibration.par_p2 = read8(0x90) | (read8(0x91) << 8);
    calibration.par_p3 = read8(0x92);
    calibration.par_p4 = read8(0x94) | (read8(0x95) << 8);
    calibration.par_p5 = read8(0x96) | (read8(0x97) << 8);
    calibration.par_p6 = read8(0x99);
    calibration.par_p7 = read8(0x98);
    calibration.par_p8 = read8(0x9c) | (read8(0x9d) << 8);
    calibration.par_p9 = read8(0x9e) | (read8(0x9f) << 8);
    calibration.par_p10 = read8(0xa0);

    return calibration;
}

int16_t BME688::calc_pressure_int(uint8_t index, int32_t t_fine) {
    pressure_calibration calibration = this->read_pressure_calibration();

    int32_t pres_adc = read8(this->memory_map_i2c.pressure_xlsb[index]) >> 4 |
                       (read8(this->memory_map_i2c.pressure_lsb[index]) << 4) |
                       (read8(this->memory_map_i2c.pressure_msb[index]) << 12);

    int32_t var1 = (((int32_t) t_fine) >> 1) - 64000;
    int32_t var2 = ((((var1 >> 2) * (var1 >> 2)) >> 11) * ((int32_t) calibration.par_p6)) >> 2;
    var2 = var2 + ((var1 * ((int32_t) calibration.par_p5)) << 1);
    var2 = (var2 >> 2) + (((int32_t) calibration.par_p4) << 16);
    var1 = (((((var1 >> 2) * (var1 >> 2)) >> 13) *
             ((int32_t) calibration.par_p3 << 5)) >> 3) + ((((int32_t) calibration.par_p2) * var1) >> 1);
    var1 = var1 >> 18;
    var1 = ((32768 + var1) * ((int32_t) calibration.par_p1)) >> 15;
    int32_t pressure_comp = 1048576 - pres_adc;
    pressure_comp = (int32_t) ((pressure_comp - (var2 >> 12)) * ((uint32_t) 3125));
    if (pressure_comp >= (1 << 30))
        pressure_comp = pressure_comp / (uint32_t) var1 << 1;
    else
        pressure_comp = (pressure_comp << 1) / (uint32_t) var1;


    var1 = (((int32_t) calibration.par_p9) *
            ((int32_t) (((pressure_comp >> 3) *
                         (pressure_comp >> 3)) >> 13))) >> 12;
    var2 = (((int32_t) (pressure_comp >> 2)) *
            ((int32_t) calibration.par_p8)) >> 13;
    int32_t var3 = (((int32_t) (pressure_comp >> 8)) *
                    ((int32_t) (pressure_comp >> 8)) *
                    ((int32_t) (pressure_comp >> 8)) *
                    ((int32_t) calibration.par_p10)) >> 17;

    return (int16_t) (pressure_comp + ((var1 + var2 + var3 +
                                        ((int32_t) calibration.par_p7 << 7)) >> 4));
}

float BME688::calc_pressure_float(uint8_t index, float t_fine) {
    pressure_calibration calibration = this->read_pressure_calibration();

    int32_t pres_adc = read8(this->memory_map_i2c.pressure_xlsb[index]) >> 4 |
                       (read8(this->memory_map_i2c.pressure_lsb[index]) << 4) |
                       (read8(this->memory_map_i2c.pressure_msb[index]) << 12);

    float var1 = ((float) t_fine / 2.0f) - 64000.0f;
    float var2 = var1 * var1 * ((float) calibration.par_p6) / 32768.0f;
    var2 = var2 + var1 * ((float) calibration.par_p5) * 2.0f;
    var2 = (var2 / 4.0f) + (((float) calibration.par_p4) * 65536.0f);
    var1 = (((float) calibration.par_p3) * var1 * var1 / 524288.0f
            + ((float) calibration.par_p2) * var1) / 524288.0f;
    var1 = (1.0f + var1 / 32768.0f) * ((float) calibration.par_p1);
    float pressure_comp = 1048576.0f - (float) pres_adc;
    pressure_comp = (pressure_comp - (var2 / 4096.0f)) * 6250.0f / var1;
    var1 = ((float) calibration.par_p9) * pressure_comp * pressure_comp / 2147483648.0f;
    var2 = pressure_comp * ((float) calibration.par_p8) / 32768.0f;
    float var3 = (pressure_comp / 256.0f) * (pressure_comp / 256.0f) *
                 (pressure_comp / 256.0f) * ((float) calibration.par_p10) / 131072.0f;
    return pressure_comp + (var1 + var2 + var3 + ((float) calibration.par_p7 * 128.0f)) / 16.0f;
}

BME688::humidity_calibration BME688::read_humidity_calibration() {
    humidity_calibration calibration{};

    calibration.par_h1 = (read8(0xe1) & 0b00001111) | (read8(0xe2) << 4);
    calibration.par_h2 = read8(0xe3) >> 4 | (read8(0xe4) << 4);
    calibration.par_h3 = read8(0xe4);
    calibration.par_h4 = read8(0xe5);
    calibration.par_h5 = read8(0xe6);
    calibration.par_h6 = read8(0xe7);
    calibration.par_h7 = read8(0xe8);

    return calibration;
}

int32_t BME688::calc_humidity_int(int index, int32_t temp_comp) {
    humidity_calibration calibration = this->read_humidity_calibration();
    int32_t hum_adc = read8(this->memory_map_i2c.humidity_msb[index]) << 8 |
                      read8(this->memory_map_i2c.humidity_lsb[index]);
    int32_t var1;
    int32_t var2;
    int32_t var3;
    int32_t var4;
    int32_t var5;
    int32_t var6;
    int32_t calc_hum;

    var1 = (int32_t)(hum_adc - ((int32_t)((int32_t)calibration.par_h1 * 16))) -
           (((temp_comp * (int32_t)calibration.par_h3) / ((int32_t)100)) >> 1);
    var2 =
            ((int32_t)calibration.par_h2 *
             (((temp_comp * (int32_t)calibration.par_h4) / ((int32_t)100)) +
              (((temp_comp * ((temp_comp * (int32_t)calibration.par_h5) / ((int32_t)100))) >> 6) / ((int32_t)100)) +
              (int32_t)(1 << 14))) >> 10;
    var3 = var1 * var2;
    var4 = (int32_t)calibration.par_h6 << 7;
    var4 = ((var4) + ((temp_comp * (int32_t)calibration.par_h7) / ((int32_t)100))) >> 4;
    var5 = ((var3 >> 14) * (var3 >> 14)) >> 10;
    var6 = (var4 * var5) >> 1;
    calc_hum = (((var3 + var6) >> 10) * ((int32_t)1000)) >> 12;
    return calc_hum;
}

float BME688::calc_humidity_float(int index, float temp_comp) {
    humidity_calibration calibration = this->read_humidity_calibration();
    uint32_t hum_adc = read8(this->memory_map_i2c.humidity_msb[index]) << 8 |
                      read8(this->memory_map_i2c.humidity_lsb[index]);
    float var1;
    float var2;
    float var3;
    float var4;
    float calc_hum;

    var1 = (float)((float)hum_adc) -
           (((float)calibration.par_h1 * 16.0f) + (((float)calibration.par_h3 / 2.0f) * temp_comp));
    var2 = var1 *
           ((float)(((float)calibration.par_h2 / 262144.0f) *
                    (1.0f + (((float)calibration.par_h4 / 16384.0f) * temp_comp) +
                     (((float)calibration.par_h5 / 1048576.0f) * temp_comp * temp_comp))));
    var3 = (float)calibration.par_h6 / 16384.0f;
    var4 = (float)calibration.par_h7 / 2097152.0f;
    calc_hum = var2 + ((var3 + (var4 * temp_comp)) * var2 * var2);

    return calc_hum;
}

int16_t BME688::calc_gas_resistance_int(int index) {
    uint32_t gas_res_adc = (uint32_t)read8(this->memory_map_i2c.gas_resistance_msb[index]) << 2 |
                (uint32_t)read8(this->memory_map_i2c.gas_resistance_lsb[index]) >> 6;
    uint32_t var1 = INT32_C(262144) >> (read8(this->memory_map_i2c.gas_resistance_lsb[index]) >> 6);
    uint32_t var2 = (int32_t) gas_res_adc - INT32_C(512);

    var2 *= INT32_C(3);
    var2 = INT32_C(4096) + var2;

    int32_t calc_gas_res = UINT32_C(1000) * var1 / var2;
    return (int16_t) calc_gas_res * 100;
}

float BME688::calc_gas_resistance_float(int index) {
    uint32_t gas_res_adc = read8(this->memory_map_i2c.gas_resistance_msb[index]) << 2 |
                           read8(this->memory_map_i2c.gas_resistance_lsb[index]) >> 6;
    uint32_t var1 = INT32_C(262144) >> (read8(this->memory_map_i2c.gas_resistance_lsb[index]) >> 6);
    uint32_t var2 = (int32_t) gas_res_adc - INT32_C(512);

    var2 *= INT32_C(3);
    var2 = INT32_C(4096) + var2;

    float calc_gas_res = (UINT32_C(1000) * var1) / var2;
    return calc_gas_res;
}

BME688::~BME688() {

}

BME688_data_float BME688::get_data_float() {

    BME688_data_float data = {0, 0, 0, 0, 0};

    if (this->parallel_mode_enable) {
        uint8_t index = (this->sub_meas_status() % 3);
        uint8_t gas_index = this->gas_meas_status() % 3;
        if (index > 0) --index;
        if (gas_index > 0) --gas_index;
        if (!this->new_data() && !this->is_measuring() && this->parallel_mode_enable)
            this->force_mode();

        while (!this->new_data()) {
            sleep_ms(1);
        }
        float t_fine;
        data.temperature = this->calc_temperature_float(index, &t_fine);
        data.pressure = this->calc_pressure_float(index, t_fine);
        data.humidity = this->calc_humidity_float(index, this->calc_temperature_float(index, nullptr));
        data.gas_resistance = this->calc_gas_resistance_float(gas_index);
    } else {
        while (!this->new_data()) {
            sleep_ms(1);
        }
        data.temperature = this->calc_temperature_float(0, nullptr);
        data.pressure = this->calc_pressure_float(0, this->calc_temperature_int(0) * 512);
        data.humidity = this->calc_humidity_float(0, this->calc_temperature_float(0, nullptr));
        data.gas_resistance = this->calc_gas_resistance_float(0);
    }

    return data;
}

uint8_t BME688::sub_meas_status() {
    uint8_t status = 0;
    uint8_t val = 0;
    for (int i = 0; i < 3; i++) {
        uint8_t n_val = read8(this->memory_map_i2c.sub_meas_status[i]);
        if (n_val > val) {
            val = n_val;
            status = i;
        }
    }
    return status;
}

uint8_t BME688::gas_meas_status() {
    uint8_t status = 0;
    uint8_t val = 0;
    for (int i = 0; i < 3; i++) {
        uint8_t n_val = read8(this->memory_map_i2c.meas_status[i]) & 0b00001111;
        if (n_val > val) {
            val = n_val;
            status = i;
        }
    }
    return status;
}

void BME688::set_gas_wait_shared(uint8_t value) {
    write8(this->memory_map_i2c.gas_wait_shared, value);
}

BME688_data_int BME688::get_data_int() {
    BME688_data_int data;

    if (this->parallel_mode_enable) {
        uint8_t index = (this->sub_meas_status() % 3);
        uint8_t gas_index = this->gas_meas_status() % 3;
        if (index > 0) --index;
        if (gas_index > 0) data.gas_resistance = this->calc_gas_resistance_int(gas_index - 1);
        if (read8(this->memory_map_i2c.meas_status[index]) & 0b1000000) return data;
        data.temperature = this->calc_temperature_int(index);
        data.pressure = this->calc_pressure_int(index, this->calc_temperature_int(index) * 512);
        data.humidity = this->calc_humidity_int(index, this->calc_temperature_int(index));

    } else {
        if (read8(this->memory_map_i2c.meas_status[0]) & 0b1000000) return data;
        data.temperature = this->calc_temperature_int(0);
        data.pressure = this->calc_pressure_int(0, this->calc_temperature_int(0) * 512);
        data.humidity = this->calc_humidity_int(0, this->calc_temperature_int(0));
        data.gas_resistance = this->calc_gas_resistance_int(0);
    }

    return data;
}

bool BME688::is_measuring() {
    if (this->parallel_mode_enable) {
        uint8_t index = (this->sub_meas_status() % 3);
        if (index > 0) --index;
        if (read8(this->memory_map_i2c.meas_status[index]) & 0b01000000) return true;
    } else {
        if (read8(this->memory_map_i2c.meas_status[0]) & 0b01000000) return true;
    }
    return true;
}

bool BME688::new_data() {
    if (this->parallel_mode_enable) {
        uint8_t index = (this->sub_meas_status() % 3);
        if (index > 0) --index;
        if (read8(this->memory_map_i2c.meas_status[index]) & 0b10000000) return true;
    } else {
        if (read8(this->memory_map_i2c.meas_status[0]) & 0b1000000) return true;
    }
    return false;
}

BME688_data_float BME688::get_data_float_wait_for_result() {
    BME688_data_float data = {0, 0, 0, 0, 0};
    if (!this->is_init_ok()) return data;

    while (!this->new_data()) {
        sleep_ms(1);
    }

    data = this->get_data_float();
    return data;
}
