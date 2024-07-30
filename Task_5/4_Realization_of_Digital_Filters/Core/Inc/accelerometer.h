//
// Created by Tobias Henig on 23.07.24.
//

#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include<cstdint>

class accelerometer {
public:
    /**
     * virtual destructor for polymorphism
     */
    virtual ~accelerometer() = default;

    /**
     * Reads the measurements of the sensor
     * @param x acceleration on x axis [m / s^2]
     * @param y acceleration on x axis [m / s^2]
     * @param z acceleration on x axis [m / s^2]
     */
    virtual void read_accelerometer(double& x, double& y, double& z) = 0;

    /**
     * @return the value of the WHO_AM_I register
     */
    virtual uint8_t readWHO_AM_I() = 0;

protected:
    /**
     * Resets and configures the sensor device initially
     */
    virtual void reset_and_config() = 0;

    /**
     * Waits for new measurements to be available
     */
    virtual void wait() = 0;
};



#endif //ACCELEROMETER_H
