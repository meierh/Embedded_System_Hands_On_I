//
// Created by Tobias Henig on 23.07.24.
//

#include "LSM303D.h"

#define SA0_LOW_ADDRESS  0b0011100
#define SA0_HIGH_ADDRESS 0b0011101 // default

bool LSM303D::alreadyReset = false;

LSM303D::LSM303D() : i2c_sensor(SA0_HIGH_ADDRESS)
{
    LSM303D::reset_and_config();
}

void LSM303D::read_accelerometer(double& x, double& y, double& z)
{
    wait();

    std::vector<uint8_t> sensorData;

    // set MSB to enable register address auto-increment
    uint8_t readAddress = OUT_X_L_A | 1 << 7;
    readVector(sensorData, readAddress, 6);

    // concatenate values
    auto raw_x = static_cast<int16_t>(sensorData[1] << 8 | sensorData[0]);
    auto raw_y = static_cast<int16_t>(sensorData[3] << 8 | sensorData[2]);
    auto raw_z = static_cast<int16_t>(sensorData[5] << 8 | sensorData[4]);

    // convert to [m / s^2]
    // first convert to mg (see LA_So on p. 10 in datasheet) with factor 0.061, then to m/s^2 with factor 9.81e-3
    x = static_cast<double>(raw_x) * 0.061 * 9.81e-3;
    y = static_cast<double>(raw_y) * 0.061 * 9.81e-3;
    z = static_cast<double>(raw_z) * 0.061 * 9.81e-3;
}

void LSM303D::read_magnetometer(double& x, double& y, double& z)
{
    wait();

    std::vector<uint8_t> sensorData;

    // set MSB to enable register address auto-increment
    uint8_t readAddress = OUT_X_L_M | 1 << 7;
    readVector(sensorData, readAddress, 6);

    // concatenate values
    auto raw_x = static_cast<int16_t>(sensorData[1] << 8 | sensorData[0]);
    auto raw_y = static_cast<int16_t>(sensorData[3] << 8 | sensorData[2]);
    auto raw_z = static_cast<int16_t>(sensorData[5] << 8 | sensorData[4]);

    // convert to T
    // first convert to mgauss (see M_S0 on p. 10 in datasheet) with factor 0.160, then to T with factor 1e-7
    x = static_cast<double>(raw_x) * 0.160 * 1e-7;
    y = static_cast<double>(raw_y) * 0.160 * 1e-7;
    z = static_cast<double>(raw_z) * 0.160 * 1e-7;
}

uint8_t LSM303D::readWHO_AM_I()
{
    return readByte(WHO_AM_I);
}

void LSM303D::reset_and_config()
{
    if (alreadyReset)
        return;

    // Set acceleration full-scale resolution to +- 2 g
    writeByte(0, CTRL2);

    // Enable accelerometer and BDU
    writeByte(0b01011111, CTRL1);

    // Enable magnetometer and switch to high resolution
    writeByte(0b01100100, CTRL5);

    // Set magnetic full-scale resolution to +-4 gauss
    writeByte(0b00100000, CTRL6);

    // Set magnetic sensor to normal mode
    writeByte(0, CTRL7);

    alreadyReset = true;
}

void LSM303D::wait()
{
    // Once the last four bits in STATUS_A are set, a new acceleration value can be read
    while ((readByte(STATUS_A) & 0b1111) != 0b1111);

    // Once the last three bits in STATUS_M are set, a new magnetic value can be read
    while ((readByte(STATUS_M) & 0b111) != 0b111);
}
