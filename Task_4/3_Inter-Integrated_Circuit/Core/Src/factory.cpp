//
// Created by Tobias Henig on 23.07.24.
//

#include "factory.h"

#include "LIS3MDL.h"
#include "LPS331AP_LPS25H.h"
#include "LSM303D.h"
#include "LSM6DS33.h"

barometer* factory::getBarometer()
{
    return new LPS331AP_LPS25H;
}

magnetometer* factory::getMagnetometer()
{
    LPS331AP_LPS25H barometer;
    uint8_t barometerWHO_AM_I = barometer.readWHO_AM_I();

    if (barometerWHO_AM_I == 0xBB)
        return new LSM303D;
    if (barometerWHO_AM_I == 0xBD)
        return new LIS3MDL;

    Error_Handler();
    return nullptr;
}

accelerometer* factory::getAccelerometer()
{
    LPS331AP_LPS25H barometer;
    uint8_t barometerWHO_AM_I = barometer.readWHO_AM_I();

    if (barometerWHO_AM_I == 0xBB)
        return new LSM303D;
    if (barometerWHO_AM_I == 0xBD)
        return new LSM6DS33;

    Error_Handler();
    return nullptr;
}
