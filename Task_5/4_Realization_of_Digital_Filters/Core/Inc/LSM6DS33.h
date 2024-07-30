//
// Created by Tobias Henig on 23.07.24.
//

#ifndef LSM6DS33_H
#define LSM6DS33_H
#include "accelerometer.h"
#include "i2c_sensor.h"


class LSM6DS33 : public accelerometer, public i2c_sensor {
public:
    LSM6DS33();
    ~LSM6DS33() override = default;

    void read_accelerometer(double& x, double& y, double& z) override;

    /**
     * read the whoAmI register and return the value
     * @return the value of the whoAmI register
     */
    uint8_t readWHO_AM_I() override;

    enum regAddr : uint8_t
    {
        WHO_AM_I                 = 0x0F,
        CTRL1_XL                 = 0x10,
        CTRL2_G                  = 0x11,
        CTRL3_C                  = 0x12,
        CTRL4_C                  = 0x13,
        CTRL5_C                  = 0x14,
        CTRL6_C                  = 0x15,
        CTRL7_G                  = 0x16,
        CTRL8_XL                 = 0x17,
        CTRL9_XL                 = 0x18,
        CTRL10_C                 = 0x19,

        OUTX_L_G                 = 0x22,
        OUTX_H_G                 = 0x23,
        OUTY_L_G                 = 0x24,
        OUTY_H_G                 = 0x25,
        OUTZ_L_G                 = 0x26,
        OUTZ_H_G                 = 0x27,
        OUTX_L_XL                = 0x28, // DS33
        OUTX_H_XL                = 0x29, // DS33
        OUTY_L_XL                = 0x2A, // DS33
        OUTY_H_XL                = 0x2B, // DS33
        OUTZ_L_XL                = 0x2C, // DS33
        OUTZ_H_XL                = 0x2D, // DS33

        STATUS_REG               = 0x1E,
    };

protected:
    /**
     * enable and configure sensor
     */
    void reset_and_config() override;
    void wait() override;

    /**
     * Read the gyro sensor and return a vector with the values
     * The vector contains 3 16bit values:
     * [0] = x
     * [1] = y
     * [2] = z
     * Note: As reading our the gyroscope values is not required in the task,
     *   we do not do any further conversions here and this method won't be
     *   used in the rest of the program.
     *
     * @return the raw gyro values as vector of 16 bit values
     */
    std::vector<uint16_t> readGyroRaw();

    static bool alreadyReset;
};



#endif //LSM6DS33_H
