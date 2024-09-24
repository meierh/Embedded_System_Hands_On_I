//
// Created by Tobias Henig on 24.09.24.
//

#include "Battery_ADC.h"

Battery_ADC::Battery_ADC() : lastUpdate(0), currentBars(3), currentMeasurement(0), initialized(false)
{
}

uint8_t Battery_ADC::getValue()
{
    // early-return if we are already at the lowest level
    if (this->currentBars == 0)
        return this->currentBars;

    // update the ADC measurement
    this->readADC();

    // add constant voltage drop and DFPlayer voltage drop if the DFPlayer is on
    uint16_t correctedMeasurement = this->currentMeasurement + VOLTAGE_DROP_CONSTANT;
    if (HAL_GPIO_ReadPin(DFPlayer_On_Off_GPIO_Port, DFPlayer_On_Off_Pin) == GPIO_PIN_SET)
        correctedMeasurement += VOLTAGE_DROP_DFPLAYER;

    // update state
    switch (this->currentBars)
    {
    case 3:
        if (correctedMeasurement < THRESHOLD_3_BARS)
            this->currentBars = 2;
        break;

    case 2:
        if (correctedMeasurement < THRESHOLD_2_BARS)
            this->currentBars = 1;
        break;

    case 1:
        if (correctedMeasurement < THRESHOLD_1_BAR)
            this->currentBars = 0;
        break;
    }

    return this->currentBars;
}

void Battery_ADC::readADC()
{
    // Calibration of the ADC if ADC has not been initialized yet
    if (!this->initialized)
    {
        if (HAL_ADCEx_Calibration_Start(&BATTERY_ADC_HANDLE) != HAL_OK)
            Error_Handler();
        this->initialized = true;
    }

    // Read a new value only if the minimum update interval has passed
    if (initialized && (HAL_GetTick() - this->lastUpdate < MIN_UPDATE_INTERVAL))
        return;

    // Get a new measurement from the ADC
    uint32_t measurementSum = 0;
    for (int i = 0; i < ADC_NUM_MEASUREMENTS; ++i)
    {
        HAL_ADC_Start(&BATTERY_ADC_HANDLE);
        if (HAL_ADC_PollForConversion(&BATTERY_ADC_HANDLE, ADC_TIMEOUT) != HAL_OK)
            Error_Handler();

        measurementSum += HAL_ADC_GetValue(&BATTERY_ADC_HANDLE);
    }

    this->currentMeasurement = measurementSum / ADC_NUM_MEASUREMENTS;
    this->lastUpdate = HAL_GetTick();
}

float Battery_ADC::getVoltage()
{
    // Update the ADC measurement
    this->readADC();

    // Formula: V_BAT(ADC) = (2 * ADC * 3.3) / 4095
    return (2.0f * this->currentMeasurement * 3.3f) / 4095.0f;
}
