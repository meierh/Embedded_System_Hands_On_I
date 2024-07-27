//
// Created by nico on 25.07.24.
//

#include "light_sensor.h"

light_sensor::light_sensor() = default;

void light_sensor::calibrate_min()
{
    calibration.set_min_illuminance(read_raw_illuminance());
}

void light_sensor::calibrate_max()
{
    calibration.set_max_illuminance(read_raw_illuminance());
}

double light_sensor::read_illuminance(void) {
    // check if min and max value is set
    if (!calibration.get_calibrated()) {
        //TODO
    }

    uint16_t raw_illu = read_raw_illuminance();

    // interpole the value of the light sensor with the min and max values
    uint16_t min = calibration.get_min_illuminance(), max = calibration.get_max_illuminance();
    double interpol_value = ((double) raw_illu - min) / ((double) max - min);
    // return a percentage value
    interpol_value *= 100;

    return interpol_value;
}

uint16_t light_sensor::read_raw_illuminance(void) {

    // Start ADC if it is not running already
    if ( HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc), HAL_ADC_STATE_REG_BUSY) )
        HAL_ADC_Start(&hadc);

    // Wait for a new conversion
    HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);

    return HAL_ADC_GetValue(&hadc);
}
