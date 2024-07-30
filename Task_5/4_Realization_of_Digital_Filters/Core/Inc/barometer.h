//
// Created by Tobias Henig on 23.07.24.
//

#ifndef BAROMETER_H
#define BAROMETER_H

#include<cstdint>

class barometer {
public:
    /**
     * virtual destructor for polymorphism
     */
    virtual ~barometer() = default;

    /**
     * Reads the measurements of the sensor
     * @param temp Buffer for the temperature measurement [K]
     * @param press Buffer for the preasure measurement [Pa]
     */
    virtual void read_barometer(double& temp, double& press) = 0;

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

    /**
     * @return The temperature measurement currently stored in the device registers, converted to K
     */
    virtual double read_temp() = 0;
    /**
     * @return The preasure measurement currently stored in the device registers, converted to Pa
     */
    virtual double read_press() = 0;
};


#endif //BAROMETER_H
