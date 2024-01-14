//
// Created by kubab on 11/11/2023.
//

#ifndef PROJECTPMC_BME680_REGISTER_ADDRESS_H
#define PROJECTPMC_BME680_REGISTER_ADDRESS_H

#include <cstdint>

// Memory map
const uint8_t BME680_CHIP_ID = 0x61;
const uint8_t BME680_RESET = 0xE0;
const uint8_t BME680_CONF = 0x75;
const uint8_t BME680_CTRL_MEAS = 0x74;
const uint8_t BME680_CTRL_HUM = 0x72;
const uint8_t BME680_CTRL_GAS_1 = 0x71;
const uint8_t BME680_CTRL_GAS_0 = 0x70;

const uint8_t BME680_WAIT_9 = 0x6D;
const uint8_t BME680_WAIT_8 = 0x6C;
const uint8_t BME680_WAIT_7 = 0x6B;
const uint8_t BME680_WAIT_6 = 0x6A;
const uint8_t BME680_WAIT_5 = 0x69;
const uint8_t BME680_WAIT_4 = 0x68;
const uint8_t BME680_WAIT_3 = 0x67;
const uint8_t BME680_WAIT_2 = 0x66;
const uint8_t BME680_WAIT_1 = 0x65;
const uint8_t BME680_WAIT_0 = 0x64;

const uint8_t BME680_HEAT_9 = 0x63;
const uint8_t BME680_HEAT_8 = 0x62;
const uint8_t BME680_HEAT_7 = 0x61;
const uint8_t BME680_HEAT_6 = 0x60;
const uint8_t BME680_HEAT_5 = 0x5F;
const uint8_t BME680_HEAT_4 = 0x5E;
const uint8_t BME680_HEAT_3 = 0x5D;
const uint8_t BME680_HEAT_2 = 0x5C;
const uint8_t BME680_HEAT_1 = 0x5B;
const uint8_t BME680_HEAT_0 = 0x5A;

const uint8_t BME680_IDAC_HEAT_9 = 0x59;
const uint8_t BME680_IDAC_HEAT_8 = 0x58;
const uint8_t BME680_IDAC_HEAT_7 = 0x57;
const uint8_t BME680_IDAC_HEAT_6 = 0x56;
const uint8_t BME680_IDAC_HEAT_5 = 0x55;
const uint8_t BME680_IDAC_HEAT_4 = 0x54;
const uint8_t BME680_IDAC_HEAT_3 = 0x53;
const uint8_t BME680_IDAC_HEAT_2 = 0x52;
const uint8_t BME680_IDAC_HEAT_1 = 0x51;
const uint8_t BME680_IDAC_HEAT_0 = 0x50;

const uint8_t BME680_GAS_R_LSB = 0x26;
const uint8_t BME680_GAS_R_MSB = 0x25;

const uint8_t BME680_TEMP_XLSB = 0x24;
const uint8_t BME680_TEMP_LSB = 0x23;
const uint8_t BME680_TEMP_MSB = 0x22;

const uint8_t BME680_PRESS_XLSB = 0x21;
const uint8_t BME680_PRESS_LSB = 0x20;
const uint8_t BME680_PRESS_MSB = 0x1F;

const uint8_t BME680_EAS_STATUS_0 = 0x1D;


#endif //PROJECTPMC_BME680_REGISTER_ADDRESS_H
