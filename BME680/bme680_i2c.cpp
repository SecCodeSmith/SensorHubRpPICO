//
// Created by kubab on 11/11/2023.
//

#include <valarray>
#include "bme680_i2c.h"
#include "bme680_register_address.h"

uint8_t bme680_i2c::read8(uint8_t reg) {
    uint8_t data[1] = {reg};
    uint8_t result[1];
    i2c_write_blocking(this->i2c, this->i2c_address, data, 1, true);
    i2c_read_blocking(this->i2c, this->i2c_address, result, 1, false);
    return result[0];
}

uint16_t bme680_i2c::read16(uint8_t reg) {
    uint8_t data[1] = {reg};
    uint8_t result[2];
    i2c_write_blocking(this->i2c, this->i2c_address, data, 1, true);
    i2c_read_blocking(this->i2c, this->i2c_address, result, 2, false);
    return (uint16_t) (result[0] | (result[1] << 8));
}

uint32_t bme680_i2c::read24(uint8_t reg) {
    uint8_t data[1] = {reg};
    uint8_t result[3];
    i2c_write_blocking(this->i2c, this->i2c_address, data, 1, true);
    i2c_read_blocking(this->i2c, this->i2c_address, result, 3, false);
    return (uint16_t) (result[0] | (result[1] << 8) | (result[2] << 16));
}

uint32_t bme680_i2c::read32(uint8_t reg) {
    uint8_t data[1] = {reg};
    uint8_t result[4];
    i2c_write_blocking(this->i2c, this->i2c_address, data, 1, true);
    i2c_read_blocking(this->i2c, this->i2c_address, result, 4, false);
    return (uint16_t) (result[0] | (result[1] << 8) | (result[2] << 16) | (result[3] << 24));
}

void bme680_i2c::write8(uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};
    i2c_write_blocking(this->i2c, this->i2c_address, data, 2, false);
}

void bme680_i2c::write16(uint8_t reg, uint16_t value) {
    uint8_t data[3] = {reg, (uint8_t) (value & 0xFF), (uint8_t) (value >> 8)};
    i2c_write_blocking(this->i2c, this->i2c_address, data, 3, false);
}

void bme680_i2c::write24(uint8_t reg, uint32_t value) {
    uint8_t data[4] = {reg, (uint8_t) (value & 0xFF), (uint8_t) (value >> 8), (uint8_t) (value >> 16)};
    i2c_write_blocking(this->i2c, this->i2c_address, data, 4, false);
}

void bme680_i2c::write32(uint8_t reg, uint32_t value) {
    uint8_t data[5] = {reg, (uint8_t) (value & 0xFF), (uint8_t) (value >> 8), (uint8_t) (value >> 16),
                       (uint8_t) (value >> 24)};
    i2c_write_blocking(this->i2c, this->i2c_address, data, 5, false);
}

bme680_i2c::bme680_i2c(i2c_inst_t *i2c, int i2c_address, int i2c_sda, int i2c_scl, int i2c_frequency) {
    this->i2c = i2c;
    this->i2c_address = i2c_address;
    this->i2c_sda = i2c_sda;
    this->i2c_scl = i2c_scl;
    this->i2c_frequency = i2c_frequency;
}

bme680_i2c::~bme680_i2c() {
}

bool bme680_i2c::init() {
    if (gpio_get_function(this->i2c_sda) != GPIO_FUNC_I2C || gpio_get_function(this->i2c_scl) != GPIO_FUNC_I2C) {
        gpio_set_function(this->i2c_sda, GPIO_FUNC_I2C);
        gpio_set_function(this->i2c_scl, GPIO_FUNC_I2C);
        gpio_pull_up(this->i2c_sda);
        gpio_pull_up(this->i2c_scl);
        i2c_init(this->i2c, this->i2c_frequency);
    }

    int id = read8(0xD0);

    if (id != 0x61) {
        return false;
    }

    this->read_calibration_data();
    this->set_default_settings();
    return true;
}

void bme680_i2c::start_forced_mode() {
    int ctrl_meas = read8(BME680_CTRL_MEAS);
    ctrl_meas = ctrl_meas | 0b01;
    write8(BME680_CTRL_MEAS, ctrl_meas);
}

void bme680_i2c::start_sleep_mode() {
    int ctrl_meas = read8(BME680_CTRL_MEAS);
    ctrl_meas = ctrl_meas & 0b00;
    write8(BME680_CTRL_MEAS, ctrl_meas);
}

void bme680_i2c::read_calibration_data() {
    this->calib_data.par_t1 = read8(0xe9) | read8(0xea) << 8;
    this->calib_data.par_t2 = read8(0x8a) | read8(0x8b) << 8;
    this->calib_data.par_t3 = read8(0x8c);

    this->calib_data.par_p1 = read8(0x8e) | read8(0x8f) << 8;
    this->calib_data.par_p2 = read8(0x90) | read8(0x91) << 8;
    this->calib_data.par_p3 = read8(0x92);
    this->calib_data.par_p4 = read8(0x94) | read8(0x95) << 8;
    this->calib_data.par_p5 = read8(0x96) | read8(0x97) << 8;
    this->calib_data.par_p6 = read8(0x99);
    this->calib_data.par_p7 = read8(0x98);
    this->calib_data.par_p8 = read8(0x9c) | read8(0x9d) << 8;
    this->calib_data.par_p9 = read8(0x9e) | read8(0x9f) << 8;
    this->calib_data.par_p10 = read8(0xa0);

    uint8_t E1 = read8(0xe1);
    uint8_t E2 = read8(0xe2);
    uint8_t E3 = read8(0xe3);

    this->calib_data.par_h1 = E3 << 4 | (E1 & 0x0F);
    this->calib_data.par_h2 = E1 >> 4 | E2 << 4;
    this->calib_data.par_h3 = read8(0xe4);
    this->calib_data.par_h4 = read8(0xe5);
    this->calib_data.par_h5 = read8(0xe6);
    this->calib_data.par_h6 = read8(0xe7);
    this->calib_data.par_h7 = read8(0xe8);

    this->calib_data.par_g1 = read8(0xED);
    this->calib_data.par_g2 = read8(0xeb) | read8(0xec) << 8;
    this->calib_data.par_g3 = read8(0xee);

    this->calib_data.range_sw_err = read8(0x04);
}

float bme680_i2c::calc_temperature() {
    int32_t temp_adc = ((read8(BME680_TEMP_XLSB) & 0xF0) >> 4) |
                       read8(BME680_TEMP_LSB) << 4 |
                       read8(BME680_TEMP_MSB) << 12;

    float var1;
    float var2;
    float calc_temp;

    /* calculate var1 data */
    var1 = ((((float) temp_adc / 16384.0f) - ((float) this->calib_data.par_t1 / 1024.0f)) *
            ((float) this->calib_data.par_t2));

    /* calculate var2 data */
    var2 =
            (((((float) temp_adc / 131072.0f) - ((float) this->calib_data.par_t1 / 8192.0f)) *
              (((float) temp_adc / 131072.0f) - ((float) this->calib_data.par_t1 / 8192.0f))) *
             ((float) this->calib_data.par_t3 * 16.0f));

    /* t_fine value*/
    this->calib_data.t_fine = (var1 + var2);

    /* compensated temperature data*/
    calc_temp = this->calib_data.t_fine / 5120.0f;

    return calc_temp;
}

float bme680_i2c::calc_pressure() {
    int32_t pres_adc = read8(BME680_PRESS_XLSB) >> 4 |
                       read8(BME680_PRESS_LSB) << 4 |
                       read8(BME680_PRESS_MSB) << 12;

    float var1, var2, var3, calc_pres;

    var1 = (((float) this->calib_data.t_fine / 2.0f) - 64000.0f);
    var2 = var1 * var1 * (((float) this->calib_data.par_p6) / (131072.0f));
    var2 = var2 + (var1 * ((float) this->calib_data.par_p5) * 2.0f);
    var2 = (var2 / 4.0f) + (((float) this->calib_data.par_p4) * 65536.0f);
    var1 = (((((float) this->calib_data.par_p3 * var1 * var1) / 16384.0f)
             + ((float) this->calib_data.par_p2 * var1)) / 524288.0f);
    var1 = ((1.0f + (var1 / 32768.0f)) * ((float) this->calib_data.par_p1));
    calc_pres = (1048576.0f - ((float) pres_adc));

    /* Avoid exception caused by division by zero */
    if ((int) var1 != 0) {
        calc_pres = (((calc_pres - (var2 / 4096.0f)) * 6250.0f) / var1);
        var1 = (((float) this->calib_data.par_p9) * calc_pres * calc_pres) / 2147483648.0f;
        var2 = calc_pres * (((float) this->calib_data.par_p8) / 32768.0f);
        var3 = ((calc_pres / 256.0f) * (calc_pres / 256.0f) * (calc_pres / 256.0f)
                * (this->calib_data.par_p10 / 131072.0f));
        calc_pres = (calc_pres + (var1 + var2 + var3 + ((float) this->calib_data.par_p7 * 128.0f)) / 16.0f);
    } else {
        calc_pres = 0;
    }

    return calc_pres;
}

void bme680_i2c::set_default_settings() {
    write8(BME680_CTRL_HUM, 0b010);
    write8(BME680_CTRL_MEAS, 0b10110100);
    write8(BME680_CONF, 0b10000000);
    uint8_t ctr_gas = 0;
    ctr_gas = read8(BME680_CTRL_GAS_1);

    ctr_gas = (ctr_gas & 0b11110000) | 0b00000100;
    write8(BME680_CTRL_GAS_1, ctr_gas);
}

void bme680_i2c::soft_reset() {
    write8(BME680_RESET, 0xB6);
    this->set_default_settings();
    this->read_calibration_data();
}

float bme680_i2c::calc_humidity() {
    float hum_adc = read8(0x26) | read8(0x25) << 8;

    float calc_hum;
    float var1;
    float var2;
    float var3;
    float var4;
    float temp_comp;

    temp_comp = ((this->calib_data.t_fine) / 5120.0f);
    var1 = (float) ((float) hum_adc) -
           (((float) this->calib_data.par_h1 * 16.0f) + (((float) this->calib_data.par_h3 / 2.0f) * temp_comp));
    var2 = var1 *
           ((float) (((float) this->calib_data.par_h2 / 262144.0f) *
                     (1.0f + (((float) this->calib_data.par_h4 / 16384.0f) * temp_comp) +
                      (((float) this->calib_data.par_h5 / 1048576.0f) * temp_comp * temp_comp))));
    var3 = (float) this->calib_data.par_h6 / 16384.0f;
    var4 = (float) this->calib_data.par_h7 / 2097152.0f;
    calc_hum = var2 + ((var3 + (var4 * temp_comp)) * var2 * var2);
    if (calc_hum > 100.0f) {
        calc_hum = 100.0f;
    } else if (calc_hum < 0.0f) {
        calc_hum = 0.0f;
    }

    return hum_adc / 1000;
}

BME680_data bme680_i2c::get_data() {
    this->start_forced_mode();
    while (read8(BME680_EAS_STATUS_0) & 0x80) { ;
    }
    BME680_data data;

    data.temperature = this->calc_temperature();
    data.pressure = this->calc_pressure();
    data.humidity = this->calc_humidity();
    write8(BME680_CTRL_GAS_1, read8(BME680_CTRL_GAS_1) | 0b00010000);
    while (read8(BME680_EAS_STATUS_0) & 0x40) { ;
    }
    data.gas_resistance = this->calc_gas_resistance(data.temperature);
    data.air_quality = read8(0x1D) & 0x0F;
    return data;
}

float bme680_i2c::calc_gas_resistance(float &temp) {
    uint8_t gas_range = read8(0x2b) & 0x0F;;
    uint16_t gas_res_adc = read8(0xb2) >> 6 | read8(0x2a) << 2;
    float calc_gas_res;
    float var1 = 0;
    float var2 = 0;
    float var3 = 0;

    const float lookup_k1_range[16] = {
            0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, -0.8,
            0.0, 0.0, -0.2, -0.5, 0.0, -1.0, 0.0, 0.0};
    const float lookup_k2_range[16] = {
            0.0, 0.0, 0.0, 0.0, 0.1, 0.7, 0.0, -0.8,
            -0.1, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    var1 = (1340.0f + (5.0f * this->calib_data.range_sw_err));
    var2 = (var1) * (1.0f + lookup_k1_range[gas_range] / 100.0f);
    var3 = 1.0f + (lookup_k2_range[gas_range] / 100.0f);

    calc_gas_res = 1.0f / (float) (var3 * (0.000000125f) * (float) (1 << gas_range) * (((((float) gas_res_adc)
                                                                                         - 512.0f) / var2) + 1.0f));

    return calc_gas_res;
}

float bme680_i2c::calc_air_quality(float gas_resistance, float humidity) {
    float var1 = 0;
    float var2 = 0;
    float calc_air_quality = 0;

    var1 = (float) (this->calib_data.par_g1 / 16.0f) + 750.0f;
    var2 = (float) (gas_resistance - this->calib_data.par_g2) / var1;
    calc_air_quality = (float) (this->calib_data.par_g3 * 100.0f) + (float) (this->calib_data.par_g3 * var2);
    return calc_air_quality;
}

void bme680_i2c::set_gas_heater(uint16_t temp, uint16_t duration) {
    float heater_res = this->calc_heater_resistance(temp);
    uint8_t heater_res_x = 0;
    uint8_t heater_dur_x = 0;
    uint8_t reg = 0;

    if (heater_res < 0) {
        heater_res = 0;
    }

    if (heater_res > 0) {
        heater_res_x = (uint8_t) (log(heater_res) / log(2)) - 1;
        reg = heater_res_x;
    }

    if (duration > 0) {
        heater_dur_x = (uint8_t) (log(duration) / log(2));
        reg = reg | (heater_dur_x << 4);
    }

    write8(BME680_CTRL_GAS_1, reg);

}

void bme680_i2c::set_heater_profile(uint16_t temp, uint16_t duration) {
    this->set_gas_heater(temp, duration);
    write8(BME680_CTRL_GAS_0, 0b00110000);
}

void bme680_i2c::set_heater_profile(uint16_t temp, uint16_t duration, uint8_t profile) {
    this->set_gas_heater(temp, duration);
    write8(BME680_CTRL_GAS_0, profile);
}

void bme680_i2c::set_heater_profile(uint16_t temp, uint16_t duration, uint8_t profile, uint8_t nb_profile) {
    this->set_gas_heater(temp, duration);
    write8(BME680_CTRL_GAS_0, profile | (nb_profile << 5));
}

void bme680_i2c::set_heater_profile(uint16_t temp, uint16_t duration, uint8_t profile, uint8_t nb_profile,
                                    uint8_t heatr_ctrl) {
    this->set_gas_heater(temp, duration);
    write8(BME680_CTRL_GAS_0, profile | (nb_profile << 5) | (heatr_ctrl << 7));
}

void bme680_i2c::set_heater_profile(uint16_t temp, uint16_t duration, uint8_t profile, uint8_t nb_profile,
                                    uint8_t heatr_ctrl, uint8_t run_gas) {
    this->set_gas_heater(temp, duration);
    write8(BME680_CTRL_GAS_0, profile | (nb_profile << 5) | (heatr_ctrl << 7) | (run_gas << 4));
}

void bme680_i2c::set_heater_profile(uint16_t temp, uint16_t duration, uint8_t profile, uint8_t nb_profile,
                                    uint8_t heatr_ctrl, uint8_t run_gas, uint8_t heatr_temp) {
    this->set_gas_heater(temp, duration);
    write8(BME680_CTRL_GAS_0, profile | (nb_profile << 5) | (heatr_ctrl << 7) | (run_gas << 4) | (heatr_temp << 1));
}

void bme680_i2c::set_heater_profile(uint16_t temp, uint16_t duration, uint8_t profile, uint8_t nb_profile,
                                    uint8_t heatr_ctrl, uint8_t run_gas, uint8_t heatr_temp, uint8_t heatr_dur) {
    this->set_gas_heater(temp, duration);
    write8(BME680_CTRL_GAS_0,
           profile | (nb_profile << 5) | (heatr_ctrl << 7) | (run_gas << 4) | (heatr_temp << 1) | heatr_dur);
}

void bme680_i2c::set_heater_profile(uint16_t temp, uint16_t duration, uint8_t profile, uint8_t nb_profile,
                                    uint8_t heatr_ctrl, uint8_t run_gas, uint8_t heatr_temp, uint8_t heatr_dur,
                                    uint8_t heatr_dur_prof) {
    this->set_gas_heater(temp, duration);
    write8(BME680_CTRL_GAS_0,
           profile | (nb_profile << 5) | (heatr_ctrl << 7) | (run_gas << 4) | (heatr_temp << 1) | heatr_dur |
           (heatr_dur_prof << 4));
}

void bme680_i2c::set_heater_profile(uint16_t temp, uint16_t duration, uint8_t profile, uint8_t nb_profile,
                                    uint8_t heatr_ctrl, uint8_t run_gas, uint8_t heatr_temp, uint8_t heatr_dur,
                                    uint8_t heatr_dur_prof, uint8_t heatr_sw_err) {
    this->set_gas_heater(temp, duration);
    write8(BME680_CTRL_GAS_0,
           profile | (nb_profile << 5) | (heatr_ctrl << 7) | (run_gas << 4) | (heatr_temp << 1) | heatr_dur |
           (heatr_dur_prof << 4) | (heatr_sw_err << 5));
}

void bme680_i2c::set_heater_profile(uint16_t temp, uint16_t duration, uint8_t profile, uint8_t nb_profile,
                                    uint8_t heatr_ctrl, uint8_t run_gas, uint8_t heatr_temp, uint8_t heatr_dur,
                                    uint8_t heatr_dur_prof, uint8_t heatr_sw_err, uint8_t idac_heatr) {
    this->set_gas_heater(temp, duration);
    write8(BME680_CTRL_GAS_0,
           profile | (nb_profile << 5) | (heatr_ctrl << 7) | (run_gas << 4) | (heatr_temp << 1) | heatr_dur |
           (heatr_dur_prof << 4) | (heatr_sw_err << 5) | (idac_heatr << 7));
}

void bme680_i2c::set_temperature_oversampling(uint8_t value) {
    uint8_t ctrl_meas = read8(BME680_CTRL_MEAS);
    ctrl_meas = ctrl_meas & 0b11100111;
    ctrl_meas = ctrl_meas | (value << 3);
    write8(BME680_CTRL_MEAS, ctrl_meas);
}

void bme680_i2c::set_pressure_oversampling(uint8_t value) {
    uint8_t ctrl_meas = read8(BME680_CTRL_MEAS);
    ctrl_meas = ctrl_meas & 0b00011111;
    ctrl_meas = ctrl_meas | (value << 5);
    write8(BME680_CTRL_MEAS, ctrl_meas);
}

void bme680_i2c::set_humidity_oversampling(uint8_t value) {
    uint8_t ctrl_hum = read8(BME680_CTRL_HUM);
    ctrl_hum = ctrl_hum & 0b11111000;
    ctrl_hum = ctrl_hum | value;
    write8(BME680_CTRL_HUM, ctrl_hum);
}

void bme680_i2c::set_gas_oversampling(uint8_t value) {
    uint8_t ctrl_gas_1 = read8(BME680_CTRL_GAS_1);
    ctrl_gas_1 = ctrl_gas_1 & 0b00001111;
    ctrl_gas_1 = ctrl_gas_1 | (value << 4);
    write8(BME680_CTRL_GAS_1, ctrl_gas_1);
}

void bme680_i2c::set_filter(uint8_t value) {
    uint8_t config = read8(BME680_CONF);
    config = config & 0b00011111;
    config = config | (value << 5);
    write8(BME680_CONF, config);
}

void bme680_i2c::set_heater_temperature(uint8_t value) {
    uint8_t ctrl_gas_1 = read8(BME680_CTRL_GAS_1);
    ctrl_gas_1 = ctrl_gas_1 & 0b11111110;
    ctrl_gas_1 = ctrl_gas_1 | value;
    write8(BME680_CTRL_GAS_1, ctrl_gas_1);
}

void bme680_i2c::set_heater_duration(uint8_t value) {
    uint8_t ctrl_gas_1 = read8(BME680_CTRL_GAS_1);
    ctrl_gas_1 = ctrl_gas_1 & 0b11110001;
    ctrl_gas_1 = ctrl_gas_1 | (value << 1);
    write8(BME680_CTRL_GAS_1, ctrl_gas_1);
}

uint8_t bme680_i2c::calc_gas_res_heat(uint16_t temp) {
    float var1 = 0;
    float var2 = 0;
    float var3 = 0;
    float var4 = 0;
    float var5 = 0;
    float res_heat = 0;
    uint8_t res_heat_range = read8(0x02) & 0x30;
    uint8_t res_heat_val = read8(0x00);

    if (temp > 400) /* Cap temperature */
        temp = 400;
    var1 = (((float) this->calib_data.par_g1 / (16.0f)) + 49.0f);
    var2 = ((((float) this->calib_data.par_g2 / (32768.0f)) * (0.0005f)) + 0.00235f);
    var3 = ((float) this->calib_data.par_g3 / (1024.0f));
    var4 = (var1 * (1.0f + (var2 * (float) temp)));
    var5 = (var4 + (var3 * 25.0f));
    res_heat = (uint8_t) (3.4f * ((var5 * (4 / (4 + (float) res_heat_range)) *
                                   (1 / (1 + ((float) res_heat_val * 0.002f)))) - 25));

    return res_heat;
}



