//
// Created by nico on 13.07.24.
//

#include "barometer_LPS.h"
#include <cstdint>
#include <vector>

#define SA0_LOW_ADDRESS  0b1011100
#define SA0_HIGH_ADDRESS 0b1011101 // default



barometer_LPS::barometer_LPS() : i2c_sensor(SA0_HIGH_ADDRESS) {
}

/**
 * config the sensor and enable it
 */
void barometer_LPS::enableSensor(void) {
    // 0xB0 = 0b10110000
    // [7]  PD: power down control.
    //      Default value: 0
    //      (0: power-down mode; 1: active mode)
    //[6:4] ODR2, ODR1, ODR0: output data rate selection.
    //      Default value: 00
    //      (see Table 18)
    //[3]   DIFF_EN: Interrupt circuit enable.
    //      Default value: 0
    //      (0: interrupt generation disabled; 1: interrupt circuit enabled)
    //[2]   BDU: block data update.
    //      Default value: 0
    //      (0: continuous update; 1: output registers not updated until MSB and LSB reading)
    //[1]   RESET_AZ: Reset AutoZero function. Reset REF_P reg, set pressure to default value in RPDS
    //      register (@0x39/A)
    //      (1: Reset. 0: disable)
    //[0]   SIM: SPI Serial Interface Mode selection.
    //      Default value: 0
    //      (0: 4-wire interface; 1: 3-wire interface)
    //      PD = 1 (active mode);
    //      ODR = 011 (12.5 Hz pressure & temperature output data rate)
    std::vector<uint8_t> msg;
    std::vector<uint8_t> value;
    msg.push_back(regAddr::LPS25H_CTRL_REG1);
    value.push_back(0xB0);
    writeVector(value, LPS25H_CTRL_REG1);
}

/**
 * read the whoamI register of the sensor and return the id
 * @return the value of the whoami register
 */
uint8_t barometer_LPS::readWhoAmI(void) {
    std::vector<uint8_t> msg;
    msg.push_back(regAddr::WHO_AM_I);

    std::vector<uint8_t> revMsg;
    readVector(revMsg, WHO_AM_I, 1);

    return revMsg.at(0);
}

/**
 * read the raw pressure value from the registers:
 * PRESS_OUT_XL, PRESS_OUT_L, PRESS_OUT_H
 *
 * The PRESS_OUT_XL register contains the lowest part of the pressure output
 * value,that is the difference between the measured pressure and the reference
 * pressure (REF_P registers).See AUTOZERO bit in CTRL_REG2.The full reference
 * pressure value is composed by PRESS_OUT_H/_L/_XL and is represented as 2’s
 * complement.
 * @return the raw temperature value
 */
int32_t barometer_LPS::readPressureRaw(void) {
    std::vector<uint8_t> sensorData;
    std::vector<uint8_t> msg;
    // assert MSB to enable register address auto-increment
    // we read PRESS_OUT_XL, PRESS_OUT_L, PRESS_OUT_H
    msg.push_back(regAddr::PRESS_OUT_XL | (1 << 7));
    std::vector<uint8_t> recMsg;
    readVector(recMsg, PRESS_OUT_XL | (1 << 7), 3);

    int32_t result = ((int32_t) (int8_t)recMsg[2]) << 16 | (uint16_t)recMsg[1] << 8 | recMsg[0];  //@todo

    return result;
}

/**
 * read the pressure in millibar
 * @return pressure in millibar
 */
float barometer_LPS::readPressureMillibars(void) {
    return (float) readPressureRaw() / 4096; //@todo
}

/**
 * read the temperature value as raw data
 * we read the register TEMP_OUT_L and TEMP_OUT_H
 * The TEMP_OUT_L register contains the low part of the temperature output
 * value.Temperature data are expressed as TEMP_OUT_H & TEMP_OUT_L as 2’s
 * complement numbers.
 * @return
 */
int16_t barometer_LPS::readTemperatureRaw(void) {

    std::vector<uint8_t> sensorData;
    std::vector<uint8_t> msg;
    // assert MSB to enable register address auto-increment
    // we read register TEMP_OUT_L, TEMP_OUT_H
    msg.push_back((regAddr::TEMP_OUT_L)| (1 << 7));
    std::vector<uint8_t> recMsg;
    readVector(recMsg, TEMP_OUT_L| (1 << 7), 2);

    return (int16_t)(recMsg[1] << 8 | recMsg[0]);
}

/**
 * read the raw temperature value and convert it to celsius
 * @return
 */
float barometer_LPS::readTemperatureInC(void) {
    return  (float) 42.5 + (float)readTemperatureRaw() / 480;
}




