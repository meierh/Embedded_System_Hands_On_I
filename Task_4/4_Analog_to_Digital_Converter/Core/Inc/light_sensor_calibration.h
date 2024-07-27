//
// Created by Tobias Henig on 27.07.24.
//

#ifndef LIGHT_SENSOR_CALIBRATION_H
#define LIGHT_SENSOR_CALIBRATION_H

#include "main.h"


class light_sensor_calibration {

public:
    light_sensor_calibration();

    /**
     * Check if min and max illuminances have been set up
     * @return true iff min and max illuminances are set
     */
    [[nodiscard]] bool get_calibrated() const;

    /**
     * Returns the min illuminance set up previously
     * @return max illuminance, or 0 if it has not been calibrated.
     */
    uint16_t get_min_illuminance() const;

    /**
     * Returns the max illuminance set up previously
     * @return max illuminance, or 1<<12 - 1 if it has not been calibrated.
     */
    uint16_t get_max_illuminance() const;

    /**
     * Stores the current illuminance value as the minimum illuminance
     */
    void set_min_illuminance(uint16_t illuminance);

    /**
     * Stores the current illuminance value as the maximum illuminance
     */
    void set_max_illuminance(uint16_t illuminance);

private:
    uint16_t min_illuminance;
    uint16_t max_illuminance;
    bool min_calibrated;
    bool max_calibrated;

    //TODO
    static void update_leds(bool recalibration);
};



#endif //LIGHT_SENSOR_CALIBRATION_H
