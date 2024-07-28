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
    double get_min_illuminance() const;

    /**
     * Returns the max illuminance set up previously
     * @return max illuminance, or 1<<12 - 1 if it has not been calibrated.
     */
    double get_max_illuminance() const;

    /**
     * Stores the current illuminance value as the minimum illuminance
     */
    void set_min_illuminance(double illuminance);

    /**
     * Stores the current illuminance value as the maximum illuminance
     */
    void set_max_illuminance(double illuminance);

private:
    double min_illuminance;
    double max_illuminance;
    bool min_calibrated;
    bool max_calibrated;

    /**
     * Update state of board LEDs to show calibration progress
     * - LED_1: on iff not calibrated completely
     * - LED_0: on iff min calibrated
     * - LED_2: on iff max calibrated
     *
     * @param recalibration_min if true, LED_0 gets blinked to indicate update of the calibration
     * @param recalibration_max if true, LED_2 gets blinked to indicate update of the calibration
     */
    void update_leds(bool recalibration_min, bool recalibration_max) const;

    /**
     * Check if min_illuminance < max_illuminance.
     * If not, delete measurements.
     */
    void bounds_check();
};



#endif //LIGHT_SENSOR_CALIBRATION_H
