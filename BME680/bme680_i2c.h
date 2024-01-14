//
// Created by kubab on 11/11/2023.
//

#ifndef PROJECTPMC_BME680_I2C_H
#define PROJECTPMC_BME680_I2C_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "bme680_register_address.h"

struct bme_calib_data {
    uint16_t par_h1;
    uint16_t par_h2;
    uint8_t par_h3;
    uint8_t par_h4;
    uint8_t par_h5;
    uint8_t par_h6;
    uint8_t par_h7;

    uint8_t par_g1;
    uint16_t par_g2;
    uint8_t par_g3;

    uint16_t par_t1;
    uint16_t par_t2;
    uint8_t par_t3;

    uint16_t par_p1;
    uint16_t par_p2;
    uint8_t par_p3;
    uint16_t par_p4;
    uint16_t par_p5;
    uint8_t par_p6;
    uint8_t par_p7;
    uint16_t par_p8;
    uint16_t par_p9;
    uint8_t par_p10;

    float t_fine;
    uint8_t range_sw_err;

};

struct BME680_data {
    float temperature;
    float pressure;
    float humidity;
    float gas_resistance;
    uint8_t air_quality;
};

class bme680_i2c {
protected:
    i2c_inst_t *i2c;
    int i2c_address;
    int i2c_sda;
    int i2c_scl;
    int i2c_frequency;
    bme_calib_data calib_data;

    uint8_t read8(uint8_t reg);

    uint16_t read16(uint8_t reg);

    uint32_t read24(uint8_t reg);

    uint32_t read32(uint8_t reg);

    void write8(uint8_t reg, uint8_t value);

    void write16(uint8_t reg, uint16_t value);

    void write24(uint8_t reg, uint32_t value);

    void write32(uint8_t reg, uint32_t value);

    void read_calibration_data();

    float calc_temperature();

    float calc_pressure();

    float calc_humidity();

    float calc_gas_resistance(float &temp);

    uint8_t calc_gas_res_heat(uint16_t temp);

    float calc_air_quality(float gas_resistance, float humidity);

public:
    bme680_i2c(i2c_inst_t *i2c, int i2c_address, int i2c_sda, int i2c_scl, int i2c_frequency);

    ~bme680_i2c();

    void set_gas_heater(uint16_t temp, uint16_t duration);

    void set_heater_profile(uint16_t temp, uint16_t duration);

    void set_heater_profile(uint16_t temp, uint16_t duration, uint8_t profile);

    void set_heater_profile(uint16_t temp, uint16_t duration, uint8_t profile, uint8_t nb_profile);

    void set_heater_profile(uint16_t temp, uint16_t duration, uint8_t profile, uint8_t nb_profile, uint8_t heatr_ctrl);

    void set_heater_profile(uint16_t temp, uint16_t duration, uint8_t profile, uint8_t nb_profile, uint8_t heatr_ctrl,
                            uint8_t run_gas);

    void set_heater_profile(uint16_t temp, uint16_t duration, uint8_t profile, uint8_t nb_profile, uint8_t heatr_ctrl,
                            uint8_t run_gas, uint8_t heatr_temp);

    void set_heater_profile(uint16_t temp, uint16_t duration, uint8_t profile, uint8_t nb_profile, uint8_t heatr_ctrl,
                            uint8_t run_gas, uint8_t heatr_temp, uint8_t heatr_dur);

    void set_heater_profile(uint16_t temp, uint16_t duration, uint8_t profile, uint8_t nb_profile, uint8_t heatr_ctrl,
                            uint8_t run_gas, uint8_t heatr_temp, uint8_t heatr_dur, uint8_t heatr_dur_prof);

    void set_heater_profile(uint16_t temp, uint16_t duration, uint8_t profile, uint8_t nb_profile, uint8_t heatr_ctrl,
                            uint8_t run_gas, uint8_t heatr_temp, uint8_t heatr_dur, uint8_t heatr_dur_prof,
                            uint8_t heatr_sw_err);

    void set_heater_profile(uint16_t temp, uint16_t duration, uint8_t profile, uint8_t nb_profile, uint8_t heatr_ctrl,
                            uint8_t run_gas, uint8_t heatr_temp, uint8_t heatr_dur, uint8_t heatr_dur_prof,
                            uint8_t heatr_sw_err, uint8_t idac_heatr);

    bool init();

    void soft_reset();

    void start_forced_mode();

    void start_sleep_mode();

    BME680_data get_data();

    void set_default_settings();

    void set_temperature_oversampling(uint8_t value);

    void set_pressure_oversampling(uint8_t value);

    void set_humidity_oversampling(uint8_t value);

    void set_gas_oversampling(uint8_t value);

    void set_filter(uint8_t value);

    void set_heater_temperature(uint8_t value);

    void set_heater_duration(uint8_t value);

    float calc_heater_resistance(uint16_t i);
};


#endif //PROJECTPMC_BME680_I2C_H
