//
// Created by Tobias Henig on 23.07.24.
//

#ifndef LPS331AP_H
#define LPS331AP_H
#include "barometer.h"
#include "i2c_sensor.h"


class LPS331AP_LPS25H : public barometer, public i2c_sensor {

public:
    LPS331AP_LPS25H();
    ~LPS331AP_LPS25H() override = default;

    void read_barometer(double& temp, double& press) override;
    /**
     * read the whoamI register of the sensor and return the id
     * @return the value of the whoami register
     */
    uint8_t readWHO_AM_I() override;

    // relevant register addresses
    // can be found in the spec sheet at p. 21
    enum regAddr: uint8_t
    {
        WHO_AM_I                = 0x0F,

        CTRL_REG1               = 0x20,
        CTRL_REG2               = 0x21,

        STATUS_REG              = 0x27,

        PRESS_OUT_XL            = 0x28,
        PRESS_OUT_L             = 0x29,
        PRESS_OUT_H             = 0x2A,

        TEMP_OUT_L              = 0x2B,
        TEMP_OUT_H              = 0x2C,
    };

protected:
    /**
     * Resets, configures and enables the sensor, if it has not been already.
     */
    void reset_and_config() override;

    /**
     * Busy-waits until a new measurement is available.
     */
    void wait() override;

    /**
     * read the temperature value
     * we read the register TEMP_OUT_L and TEMP_OUT_H
     * The TEMP_OUT_L register contains the low part of the temperature output
     * value.Temperature data are expressed as TEMP_OUT_H & TEMP_OUT_L as 2’s
     * complement numbers.
     * @return the temperature value in K.
     */
    double read_temp() override;

    /**
     * read the pressure value from the registers and convert it to Pa.
     * PRESS_OUT_XL, PRESS_OUT_L, PRESS_OUT_H
     *
     * The PRESS_OUT_XL register contains the lowest part of the pressure output
     * value,that is the difference between the measured pressure and the reference
     * pressure (REF_P registers).See AUTOZERO bit in CTRL_REG2.The full reference
     * pressure value is composed by PRESS_OUT_H/_L/_XL and is represented as 2’s
     * complement.
     * @return the preasure value in Pa.
     */
    double read_press() override;

    static bool alreadyReset;
};



#endif //LPS331AP_H
