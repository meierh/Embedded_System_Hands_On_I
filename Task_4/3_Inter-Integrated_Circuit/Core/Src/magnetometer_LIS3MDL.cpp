#define LIS3MDL_SA1_LOW_ADDRESS   0b0011100
#define LIS3MDL_SA1_HIGH_ADDRESS  0b0011110

#include "magnetometer_LIS3MDL.h"

magnetometer_LIS3MDL::magnetometer_LIS3MDL() : i2c_sensor() {
    address = LIS3MDL_SA1_HIGH_ADDRESS;
}

uint8_t magnetometer_LIS3MDL::readWhoAmI(void) {
    std::vector<uint8_t> msg;
    msg.push_back(regAddr::WHO_AM_I);

    std::vector<uint8_t> revMsg;
    readVector(revMsg, msg, 1);

    return revMsg.at(0);
}

/**
 * enable and setup the sensor
 */
void magnetometer_LIS3MDL::enableSensor() {
    std::vector<uint8_t> msg;
    std::vector<uint8_t> value;

    // 0x70 = 0b01110000
    // OM = 11 (ultra-high-performance mode for X and Y); DO = 100 (10 Hz ODR)
    msg.push_back(regAddr::CTRL_REG1);
    value.push_back(0x70);
    writeVector(value, msg);

    msg.pop_back();
    value.pop_back();

    // 0x00 = 0b00000000
    // FS = 00 (+/- 4 gauss full scale)
    msg.push_back(regAddr::CTRL_REG2);
    value.push_back(0x00);
    writeVector(value, msg);

    msg.pop_back();
    value.pop_back();

    // 0x00 = 0b00000000
    // MD = 00 (continuous-conversion mode)
    msg.push_back(regAddr::CTRL_REG3);
    value.push_back(0x00);
    writeVector(value, msg);

    msg.pop_back();
    value.pop_back();

    // 0x0C = 0b00001100
    // OMZ = 11 (ultra-high-performance mode for Z)
    msg.push_back(regAddr::CTRL_REG4);
    value.push_back(0x0C);
    writeVector(value, msg);

    msg.pop_back();
    value.pop_back();

    // 0x40 = 0b01000000
    // BDU = 1 (block data update)
    msg.push_back(regAddr::CTRL_REG5);
    value.push_back(0x40);
    writeVector(value, msg);
}

void magnetometer_LIS3MDL::readMagneticField(void) {
    //@todo
}

