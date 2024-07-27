//
// Created by Tobias Henig on 27.07.24.
//

#include "light_sensor_calibration.h"

light_sensor_calibration::light_sensor_calibration() : min_illuminance(0), max_illuminance(0), min_calibrated(false), max_calibrated(false)
{
    update_leds(false, false);
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
    update_leds(recalibration, false);
}

void light_sensor_calibration::set_max_illuminance(uint16_t illuminance)
{
    bool recalibration = max_calibrated;

    max_illuminance = illuminance;

    max_calibrated = true;
    update_leds(false, recalibration);
}

void light_sensor_calibration::update_leds(bool recalibration_min, bool recalibration_max) const
{
    // LED_1 on iff not calibrated
    if (get_calibrated())
        HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET);

    // LED_0 on iff min calibrated
    if (min_calibrated)
        HAL_GPIO_WritePin(LED_0_GPIO_Port, LED_0_Pin, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(LED_0_GPIO_Port, LED_0_Pin, GPIO_PIN_RESET);

    // LED_2 on iff max calibrated
    if (max_calibrated)
        HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);

    // indicate update of min value
    if (recalibration_min)
    {
        for (int i = 0; i < 4; i++)
        {
            HAL_GPIO_TogglePin(LED_0_GPIO_Port, LED_0_Pin);
            HAL_Delay(100);
        }
    }

    // indicate update of max value
    if (recalibration_max)
    {
        for (int i = 0; i < 4; i++)
        {
            HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
            HAL_Delay(100);
        }
    }
}
