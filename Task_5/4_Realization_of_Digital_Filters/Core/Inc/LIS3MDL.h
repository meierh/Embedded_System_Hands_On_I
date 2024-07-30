//
// Created by Tobias Henig on 23.07.24.
//

#ifndef LIS3MDL_H
#define LIS3MDL_H
#include "i2c_sensor.h"
#include "magnetometer.h"


class LIS3MDL : public magnetometer, public i2c_sensor {
public:
    LIS3MDL();
    ~LIS3MDL() override = default;

    void read_magnetometer(double& x, double& y, double& z) override;

    uint8_t readWHO_AM_I() override;

    enum regAddr : uint8_t
    {
        WHO_AM_I    = 0x0F,

        CTRL_REG1   = 0x20,
        CTRL_REG2   = 0x21,
        CTRL_REG3   = 0x22,
        CTRL_REG4   = 0x23,
        CTRL_REG5   = 0x24,

        STATUS_REG  = 0x27,
        OUT_X_L     = 0x28,
        OUT_X_H     = 0x29,
        OUT_Y_L     = 0x2A,
        OUT_Y_H     = 0x2B,
        OUT_Z_L     = 0x2C,
        OUT_Z_H     = 0x2D,
        TEMP_OUT_L  = 0x2E,
        TEMP_OUT_H  = 0x2F,
    };

protected:
    /**
     * enable and setup the sensor
     */
    void reset_and_config() override;
    void wait() override;


    static bool alreadyReset;
};



#endif //LIS3MDL_H
