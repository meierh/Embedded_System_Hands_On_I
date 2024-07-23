//
// Created by Tobias Henig on 23.07.24.
//

#ifndef LSM303D_H
#define LSM303D_H
#include "accelerometer.h"
#include "i2c_sensor.h"
#include "magnetometer.h"


class LSM303D : public magnetometer, public accelerometer, public i2c_sensor {
public:
    LSM303D();
    ~LSM303D() override = default;

    void read_accelerometer(double& x, double& y, double& z) override;
    void read_magnetometer(double& x, double& y, double& z) override;

    uint8_t readWHO_AM_I() override;

    enum regAddr : uint8_t
    {
        WHO_AM_I = 0x0F,

        CTRL0 = 0x1F,
        CTRL1 = 0x20,
        CTRL2 = 0x21,
        CTRL3 = 0x22,
        CTRL4 = 0x23,
        CTRL5 = 0x24,
        CTRL6 = 0x25,
        CTRL7 = 0x26,

        STATUS_A = 0x27,
        STATUS_M = 0x07,

        OUT_X_L_A = 0x28,

        OUT_X_L_M = 0x08
    };

protected:
    void reset_and_config() override;
    void wait() override;

    static bool alreadyReset;

};



#endif //LSM303D_H
