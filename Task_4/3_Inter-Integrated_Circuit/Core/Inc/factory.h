//
// Created by Tobias Henig on 23.07.24.
//

#ifndef FACTORY_H
#define FACTORY_H
#include "accelerometer.h"
#include "barometer.h"
#include "magnetometer.h"


class factory {
public:
    factory() = delete;
    ~factory() = delete;

    /**
     * Auto-detect if the v3 or the v5 sensor board is present and return the correct sensor implementation.
     * @return a pointer to a new instance of LPS331AP_LPS25H (the same implementation works on both boards)
     */
    static barometer* getBarometer();

    /**
     * Auto-detect if the v3 or the v5 sensor board is present and return the correct sensor implementation.
     * @return a pointer to a new instance of LSM303D for the v3 board and to LIS3MDL for the v5 board
     */
    static magnetometer* getMagnetometer();

    /**
     * Auto-detect if the v3 or the v5 sensor board is present and return the correct sensor implementation.
     * @return a pointer to a new instance of LSM303D for the v3 board and to LSM6DS33 for the v5 board
     */
    static accelerometer* getAccelerometer();
};



#endif //FACTORY_H
