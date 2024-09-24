//
// Created by Tobias Henig on 24.09.24.
//

#ifndef BATTERY_ADC_H
#define BATTERY_ADC_H

#include "main.h"

extern ADC_HandleTypeDef hadc1;
#define BATTERY_ADC_HANDLE hadc1

class Battery_ADC {
    /**
     * Minimum time between each ADC measurement
     * Used to reduce the number of measurements and thus resource usage
     */
    static constexpr uint32_t MIN_UPDATE_INTERVAL = 10000; // ms

    /**
     * Timeout for the ADC measurement
     */
    static constexpr uint32_t ADC_TIMEOUT = 1500; // ms

    /**
     * The number of measurements per ADC read
     * To avoid noise, the battery level is measured multiple times and the average is taken
     */
    static constexpr uint8_t ADC_NUM_MEASUREMENTS = 10;

    /**
     * Threshold between 3 and 2 bars
     * Raw ADC value, calculated via (4095 * 3.91) / 6.6
     */
    static constexpr uint16_t THRESHOLD_3_BARS = 2425; // = 3.91 V

    /**
     * Threshold between 2 and 1 bars
     * Raw ADC value, calculated via (4095 * 3.68) / 6.6
     */
    static constexpr uint16_t THRESHOLD_2_BARS = 2283; // = 3.68 V

    /**
     * Threshold between 1 and 0 bars
     * Raw ADC value, calculated via (4095 * 3.51) / 6.6
     */
    static constexpr uint16_t THRESHOLD_1_BAR  = 2178; // = 3.51 V

    /**
     * Constant voltage drop compared to STM running at 64 MHz with no additional peripherals
     */
    static constexpr uint16_t VOLTAGE_DROP_CONSTANT = 10; // = 0.016 V

    /**
     * Additional voltage drop when the DFPlayer is on
     */
    static constexpr uint16_t VOLTAGE_DROP_DFPLAYER = 93; // = 0.15 V

public:
    /**
     * Constructor
     * Calibrates the ADC
     */
    Battery_ADC();
    ~Battery_ADC() = default;

    /**
     * Returns the current battery level
     * @return battery level (0-3 bars)
     */
    uint8_t getValue();

    /**
     * Returns the current voltage of the battery
     * @return the current voltage of the battery
     */
    float getVoltage();

private:
    uint32_t lastUpdate;
    uint8_t currentBars;
    uint16_t currentMeasurement;
    bool initialized;

    void readADC();
};

#endif //BATTERY_ADC_H
