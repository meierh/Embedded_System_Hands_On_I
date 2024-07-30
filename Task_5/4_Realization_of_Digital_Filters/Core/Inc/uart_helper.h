//
// Created by Tobias Henig on 25.07.24.
//

#ifndef UART_HELPER_H
#define UART_HELPER_H

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

/**
 * Reads a string via UART until a '\n' is received and convert the string to an unsinged int
 * @return The received number
 */
uint32_t uart_read_num();

/**
 * Writes the ID (1 Byte) of a sensor via UART
 * @param name The name of the sensor
 * @param id The sensor's ID
 */
void uart_write_id(const char* name, uint8_t id);

/**
 * Writes a header for the measurements (row lables with units) via UART
 */
void uart_write_header();

/**
 * Writes a header for filter-testings for the measurements (row lables with units) via UART
 */
void uart_write_header_filter_test();

/**
 * Writes one dataset of measurements as one line via UART
 * @param time Measurement time in s
 * @param temperature Measured temperature in K
 * @param pressure Measured preassure in Pa
 * @param magnet_x Measured magnetic flux density (x-axis) in T
 * @param magnet_y Measured magnetic flux density (y-axis) in T
 * @param magnet_z Measured magnetic flux density (z-axis) in T
 * @param acc_x Measured acceleration (x-axis) in m/s^2
 * @param acc_y Measured acceleration (y-axis) in m/s^2
 * @param acc_z Measured acceleration (z-axis) in m/s^2
 */
void uart_write_data(double* time, double* temperature, double* pressure, double* magnet_x, double* magnet_y, double* magnet_z, double* acc_x, double* acc_y, double* acc_z);

/**
 * write one dataset of raw and filtered acc via uart
 * @param time
 * @param acc_x_raw
 * @param acc_y_raw
 * @param acc_z_raw
 * @param acc_x_filtered
 * @param acc_y_filtered
 * @param acc_z_filtered
 */
void uart_write_filter_test_data(double* time, double* acc_x_raw, double* acc_y_raw, double* acc_z_raw, double* acc_x_filtered, double* acc_y_filtered, double* acc_z_filtered);

/**
 * Writes GOODBYE_MSG via UART
 */
void uart_goodbye();

#endif //UART_HELPER_H
