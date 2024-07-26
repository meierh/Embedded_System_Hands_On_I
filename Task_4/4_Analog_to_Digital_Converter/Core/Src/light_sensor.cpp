//
// Created by nico on 25.07.24.
//

#include "light_sensor.h"

light_sensor::light_sensor() {

}

void light_sensor::set_max_illuminance(void) {
    max_illuminance = read_raw_illuminance();
}

void light_sensor::set_min_illuminance(void) {
    min_illuminance = read_raw_illuminance();
}

bool light_sensor::check_bounders(void) {
   return min_illuminance != -1 && max_illuminance != -1;
}

double light_sensor::read_illuminance(void) {
    // check if min and max value is set
    if (!check_bounders()) {
        return -1;
    }

    uint32_t raw_illu = read_raw_illuminance();

    // interpole the value of the light sensor with the min and max values
    double interpol_value = 0 + ((1 - 0) / (max_illuminance - min_illuminance)) * (raw_illu - min_illuminance);

    return interpol_value;
}

uint32_t light_sensor::read_raw_illuminance(void) {
    return HAL_ADC_GetValue(&hadc);
}
