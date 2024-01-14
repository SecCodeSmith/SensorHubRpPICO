#ifndef PROJECTPMC_BME688_H
#define PROJECTPMC_BME688_H

#include <hardware/i2c.h>
#include "pico/stdio.h"


struct BME688_data_float {
    float temperature;
    float pressure;
    float humidity;
    float gas_resistance;
    uint8_t air_quality;
};

struct BME688_data_int {
    int16_t temperature;
    int8_t pressure;
    int8_t humidity;
    int8_t gas_resistance;
    int8_t air_quality;
};

class BME688 {
private:
    struct BME688_memory_I2C_map {
        const uint8_t status = 0x73;
        const uint8_t variant_id = 0xf0;
        const uint8_t reset = 0xe0;
        const uint8_t chip_id = 0xd0;
        const uint8_t chip_id_value = 0x61;
        const uint8_t config = 0x75;

        const uint8_t ctr_meas = 0x74;
        const uint8_t ctr_hum = 0x72;
        const uint8_t ctr_gas_1 = 0x71;
        const uint8_t ctr_gas_2 = 0x70;

        const uint8_t gas_wait_shared = 0x6e;

        const uint8_t gas_wait[10] = {0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d};
        const uint8_t res_heat[10] = {0x63, 0x62, 0x61, 0x60, 0x5f, 0x5e, 0x5d, 0x5c, 0x5b, 0x5a};
        const uint8_t idac_heat[10] = {0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59};

        const uint8_t pressure_msb[3] = {0x1f, 0x30, 0x41};
        const uint8_t pressure_lsb[3] = {0x20, 0x31, 0x42};
        const uint8_t pressure_xlsb[3] = {0x21, 0x32, 0x43};

        const uint8_t temperature_msb[3] = {0x22, 0x33, 0x44};
        const uint8_t temperature_lsb[3] = {0x23, 0x34, 0x45};
        const uint8_t temperature_xlsb[3] = {0x24, 0x35, 0x46};

        const uint8_t humidity_msb[3] = {0x25, 0x36, 0x47};
        const uint8_t humidity_lsb[3] = {0x26, 0x37, 0x48};

        const uint8_t gas_resistance_msb[3] = {0x2c, 0x3d, 0x4e};
        const uint8_t gas_resistance_lsb[3] = {0x2d, 0x3e, 0x4f};

        const uint8_t meas_status[3] = {0x1d, 0x2e, 0x3f};

        const uint8_t sub_meas_status[3] = {0x1e, 0x2f, 0x40};
    };

    struct temperature_calibration {
        int16_t par_t1;
        int16_t par_t2;
        int8_t par_t3;
    };

    struct pressure_calibration {
        uint16_t par_p1;
        int16_t par_p2;
        int8_t par_p3;
        int16_t par_p4;
        int16_t par_p5;
        int8_t par_p6;
        int8_t par_p7;
        int16_t par_p8;
        int16_t par_p9;
        uint8_t par_p10;
    };

    struct humidity_calibration {
        uint16_t par_h1;
        uint16_t par_h2;
        uint8_t par_h3;
        uint8_t par_h4;
        uint8_t par_h5;
        uint8_t par_h6;
        uint8_t par_h7;
    };


protected:

    const BME688_memory_I2C_map memory_map_i2c;
    uint8_t sda_pin, scl_pin, i2c_port_number;
    uint16_t i2c_address;
    i2c_inst_t *i2c;
    uint32_t i2c_frequency;
    bool init_ok = false;
    bool parallel_mode_enable = false;

    uint8_t read8(uint8_t reg);

    void write8(uint8_t reg, uint8_t value);

    void set_humidity_oversampling(uint8_t value);

    void set_temperature_oversampling(uint8_t value);

    void set_pressure_oversampling(uint8_t value);

    void set_gas_wait(uint8_t x, uint8_t value);

    void set_gas_wait_shared(uint8_t value);

    void set_res_heat(uint8_t x, uint8_t value);

    void set_nb_conv(uint8_t value);

    void set_run_gas();

    temperature_calibration read_temperature_calibration();

    pressure_calibration read_pressure_calibration();

    humidity_calibration read_humidity_calibration();

    int32_t calc_temperature_int(int index);

    float calc_temperature_float(int index, float *t_fine);

    int16_t calc_pressure_int(uint8_t index, int32_t t_fine);

    float calc_pressure_float(uint8_t index, float t_fine);

    int32_t calc_humidity_int(int index, int32_t temp_comp);

    float calc_humidity_float(int index, float temp_comp);

    int16_t calc_gas_resistance_int(int index);

    float calc_gas_resistance_float(int index);

    uint8_t sub_meas_status();

    uint8_t gas_meas_status();

public:
    enum class BME688_mode {
        sleep = 0b00,
        forced = 0b01,
        parallel = 0b10
    };

    BME688(uint8_t sda_pin, uint8_t scl_pin,
           uint8_t i2c_address, i2c_inst_t *i2c_port,
           uint32_t i2c_frequency);

    ~BME688();

    bool init();

    bool is_init_ok();

    bool is_measuring();
    bool new_data();

    void soft_reset();

    void force_mode();

    void parallel_mode();

    void sleep_mode();

    BME688_data_float get_data_float();
    BME688_data_float get_data_float_wait_for_result();

    BME688_data_int get_data_int();

    void set_mode(BME688_mode value);
};

#endif //PROJECTPMC_BME688_H