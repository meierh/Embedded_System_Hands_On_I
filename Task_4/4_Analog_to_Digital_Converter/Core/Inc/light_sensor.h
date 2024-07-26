//
// Created by nico on 25.07.24.
//

#ifndef INC_4_ANALOG_TO_DIGITAL_CONVERTER_LIGHT_SENSOR_H
#define INC_4_ANALOG_TO_DIGITAL_CONVERTER_LIGHT_SENSOR_H

#include "main.h"


class light_sensor{

    uint32_t min_illuminance = -1;
    uint32_t max_illuminance = -1;

public:
    light_sensor();

    void set_max_illuminance(void);
    void set_min_illuminance(void);

    bool check_bounders(void);

    double read_illuminance(void);

private:
    uint32_t read_raw_illuminance(void);


};


#endif //INC_4_ANALOG_TO_DIGITAL_CONVERTER_LIGHT_SENSOR_H
