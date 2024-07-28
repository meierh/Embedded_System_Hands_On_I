//
// Created by Tobias Henig on 25.07.24.
//

#ifndef UART_HELPER_H
#define UART_HELPER_H

#include "light_sensor_calibration.h"
#include "main.h"

/**
 * Buffer for receiving number of measurements and delay values
 * (maximum amount of bytes to be stored after receiving)
 */
#define RECEIVE_BUFFER 100

/**
 * Buffer for transmitting measured values
 * (maximum amount of bytes to be stored before tranmission)
 */
#define TRANSMIT_BUFFER 1000

/**
 * Message which indicates the end of UART communication
 */
#define GOODBYE_MSG ">>> End of measurements."

namespace uart
{
    /**
     * Reads a string via UART until a '\n' is received and convert the string to an unsinged int
     * @return The received number
     */
    uint32_t read_num();

    /**
     * Writes a header for the measurements (row lables with units) via UART
     */
    void write_header();

    /**
     * Writes one dataset of measurements as one line via UART
     * @param time Measurement time in s
     * @param illuminance_percentage Current illuminance value relative to calibration
     */
    void write_data(double* time, double *illuminance_percentage);

    /**
     * Writes the calibration values via UART
     * @param calibration Pointer to calibration object
     */
    void write_calibration(light_sensor_calibration *calibration);

    /**
     * Writes a general message via UART
     * @param msg a general message
     */
    void write_msg(const char* msg);

    /**
     * Writes GOODBYE_MSG via UART
     */
    void goodbye();

}
#endif //UART_HELPER_H
