//
// Created by Tobias Henig on 23.07.24.
//

#include "LPS331AP_LPS25H.h"

#define SA0_LOW_ADDRESS  0b1011100
#define SA0_HIGH_ADDRESS 0b1011101 // default

bool LPS331AP_LPS25H::alreadyReset = false;

LPS331AP_LPS25H::LPS331AP_LPS25H() : i2c_sensor(SA0_HIGH_ADDRESS)
{
    LPS331AP_LPS25H::reset_and_config();
}

void LPS331AP_LPS25H::read_barometer(double& temp, double& press)
{
    wait();
    temp = read_temp();
    press = read_press();
}

uint8_t LPS331AP_LPS25H::readWHO_AM_I()
{
    return readByte(WHO_AM_I);
}

void LPS331AP_LPS25H::reset_and_config()
{
    if (alreadyReset)
        return;

    // Reset the device by setting BOOT and SW_RESET in CTRL_REG2
    writeByte(0b10000101, CTRL_REG2);

    // Wait for the device to be reset
    while ((readByte(CTRL_REG2) & 0b1000000) != 0) {}
    // Clear SWRESET bit
    writeByte(0b00000000, CTRL_REG2);

    // Set the CTRL_REG1 register:
    // 0xB4 = 0b10110100
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
    writeByte(0xB4, CTRL_REG1);

    alreadyReset = true;
}

void LPS331AP_LPS25H::wait()
{
    // Once the last two bits in STATUS_REG are set, a new value can be read
    while ((readByte(STATUS_REG) & 0b11) != 0b11);
}

double LPS331AP_LPS25H::read_temp()
{
    std::vector<uint8_t> sensorData;
    // assert MSB to enable register address auto-increment
    // we read register TEMP_OUT_L, TEMP_OUT_H
    uint8_t readAddress = TEMP_OUT_L | (1 << 7);
    readVector(sensorData, readAddress, 2);

    // concatenate values
    auto raw_temp = static_cast<int16_t>(sensorData[1] << 8 | sensorData[0]);

    // convert to K
    return static_cast<double>(raw_temp) / 480 + 42.5 + 273.15;
}

double LPS331AP_LPS25H::read_press()
{
    std::vector<uint8_t> sensorData;
    // assert MSB to enable register address auto-increment
    // we read PRESS_OUT_XL, PRESS_OUT_L, PRESS_OUT_H
    uint8_t readAddress = PRESS_OUT_XL | (1 << 7);
    readVector(sensorData, readAddress, 3);

    // concatenate values s.t. negative values stay negative
    auto raw_press_upper = static_cast<int16_t>(sensorData[2] << 8 | sensorData[1]);
    int32_t raw_press = raw_press_upper << 8 | sensorData[0];

    // convert to Pa
    return static_cast<double>(raw_press) / 4096 * 100;
}

