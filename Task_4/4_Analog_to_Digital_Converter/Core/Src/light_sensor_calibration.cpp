//
// Created by Tobias Henig on 27.07.24.
//

#include "light_sensor_calibration.h"

light_sensor_calibration::light_sensor_calibration() : min_illuminance(0), max_illuminance(0), min_calibrated(false), max_calibrated(false)
{
}

bool light_sensor_calibration::get_calibrated() const
{
    return min_calibrated && max_calibrated;
}

uint16_t light_sensor_calibration::get_min_illuminance() const
{
    if (min_calibrated)
        return min_illuminance;
    return 0;
}

uint16_t light_sensor_calibration::get_max_illuminance() const
{
    if (max_calibrated)
        return max_illuminance;
    return (1 << 12) - 1; // maximum possible ADC value
}

void light_sensor_calibration::set_min_illuminance(uint16_t illuminance)
{
    bool recalibration = min_calibrated;

    min_illuminance = illuminance;

    min_calibrated = true;
    update_leds(recalibration);
}

void light_sensor_calibration::set_max_illuminance(uint16_t illuminance)
{
    bool recalibration = max_calibrated;

    max_illuminance = illuminance;

    max_calibrated = true;
    update_leds(recalibration);
}

void light_sensor_calibration::update_leds(bool recalibration)
{
    //TODO
}
