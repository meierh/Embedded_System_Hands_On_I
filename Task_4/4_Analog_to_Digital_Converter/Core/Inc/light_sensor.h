//
// Created by nico on 25.07.24.
//

#ifndef INC_4_ANALOG_TO_DIGITAL_CONVERTER_LIGHT_SENSOR_H
#define INC_4_ANALOG_TO_DIGITAL_CONVERTER_LIGHT_SENSOR_H

/**
 * CLIPPING == true  - every measurement below minimum calibration value yields 0 %, every measurement above calibration yields 100 %
 * CLIPPING == false - negative percentage values and percentage above 100 % are possible
 */
#define CLIPPING true

#include "light_sensor_calibration.h"
#include "main.h"


class light_sensor {


public:
    light_sensor_calibration calibration;

    light_sensor();

    /**
     * Measure the current illuminance value and store it as the minimum within the calibration object
     */
    void calibrate_min();

    /**
     * Measure the current illuminance value and store it as the minimum within the calibration object
     */
    void calibrate_max();

    /**
     * Return the current illuminance value
     * @return the current illuminance in percent (relative to the converted values)
     */
    double read_illuminance(void);

private:
    /**
     * Read a new sensor value from the ADC
     * @return raw ADC value in [0, 4095]
     */
    uint16_t read_raw_illuminance(void);


};


#endif //INC_4_ANALOG_TO_DIGITAL_CONVERTER_LIGHT_SENSOR_H
