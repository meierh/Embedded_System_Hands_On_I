#ifndef INC_3_INTER_INTEGRATED_CIRCUIT_MAGNETOMETER_LIS3MDL_H
#define INC_3_INTER_INTEGRATED_CIRCUIT_MAGNETOMETER_LIS3MDL_H
#include "i2c_sensor.h"


class magnetometer_LIS3MDL :i2c_sensor  {
public:
    magnetometer_LIS3MDL();
    // register addresses
    enum regAddr: uint8_t
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
        INT_CFG     = 0x30,
        INT_SRC     = 0x31,
        INT_THS_L   = 0x32,
        INT_THS_H   = 0x33,
    };

    uint8_t last_status; // status of last I2C transmission
    // read the whoami register of the device
    uint8_t readWhoAmI(void);
    void enableSensor(void);
    std::vector<uint8_t> readMagneticField(void);
    uint8_t readTemperature(void);
};


#endif //INC_3_INTER_INTEGRATED_CIRCUIT_MAGNETOMETER_LIS3MDL_H
