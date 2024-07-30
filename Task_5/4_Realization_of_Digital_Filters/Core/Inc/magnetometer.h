//
// Created by Tobias Henig on 23.07.24.
//

#ifndef MAGNETOMETER_H
#define MAGNETOMETER_H

#include<cstdint>

class magnetometer {
public:
    /**
     * virtual destructor for polymorphism
     */
    virtual ~magnetometer() = default;

    /**
     * Reads the measurements of the sensor
     * @param x magnetic flux density on x axis [T]
     * @param y magnetic flux density on y axis [T]
     * @param z magnetic flux density on z axis [T]
     */
    virtual void read_magnetometer(double& x, double& y, double& z) = 0;

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



#endif //MAGNETOMETER_H
