//
// Created by nico on 25.07.24.
//

#include "light_sensor.h"

#include "uart_helper.h"

light_sensor::light_sensor() = default;

void light_sensor::calibrate_min()
{
    // Read 10 values and calculate average
    double illuminance_readings = 0;
    for (int i = 0; i < 10; i++)
        illuminance_readings += read_raw_illuminance();

    calibration.set_min_illuminance(illuminance_readings / 10);
}

void light_sensor::calibrate_max()
{
    // Read 10 values and calculate average
    double illuminance_readings = 0;
    for (int i = 0; i < 10; i++)
        illuminance_readings += read_raw_illuminance();

    calibration.set_max_illuminance(illuminance_readings / 10);
}

double light_sensor::read_illuminance(void) {
    // check if min and max value is set
    if (!calibration.get_calibrated()) {
        uart::write_msg("Warning: Read illuminance value prior to calibration!");
    }

    uint16_t raw_illu = read_raw_illuminance();

    // interpole the value of the light sensor with the min and max values
    double min = calibration.get_min_illuminance(), max = calibration.get_max_illuminance();
    double interpol_value = (raw_illu - min) / (max - min);
    // return a percentage value
    interpol_value *= 100;

    if (CLIPPING)
    {
        if (interpol_value > 100)
            interpol_value = 100;
        if (interpol_value < 0)
            interpol_value = 0;
    }

    return interpol_value;
}

uint16_t light_sensor::read_raw_illuminance(void) {

    // Start ADC if it is not running already
    if ( !HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc), HAL_ADC_STATE_REG_BUSY) )
        HAL_ADC_Start(&hadc);

    // Wait for a new conversion
    HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);

    return HAL_ADC_GetValue(&hadc);
}
