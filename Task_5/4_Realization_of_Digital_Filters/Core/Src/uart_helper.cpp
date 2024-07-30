//
// Created by Tobias Henig on 25.07.24.
//

#include "uart_helper.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

// +1 because of null-termination of strings
uint8_t read_buffer[RECEIVE_BUFFER + 1];
char write_string[TRANSMIT_BUFFER + 1];
char pending_write_string[TRANSMIT_BUFFER + 1];
auto write_buffer = reinterpret_cast<uint8_t*>(write_string);

/**
 * true iff a UART tranmission/reception has not yet finished
 */
volatile bool UART_BUSY;

uint32_t uart_read_num()
{
    // Receive one full line up to '\n'
    int i = 0;
    for (; i < RECEIVE_BUFFER; i++)
    {
        HAL_UART_Receive(&huart5, read_buffer + i, 1, HAL_MAX_DELAY);

        if (read_buffer[i] == '\n')
        {
            i++;
            break;
        }
    }

    // Terminate the string
    read_buffer[i] = '\0';

    return atoi(reinterpret_cast<char*>(read_buffer));
}

/**
 * Helper method which handles the UART transmission.
 * Waits until the last transmission has finished and sends the content of write_string.
 */
void uart_write()
{
    // Busy-wait util the last transmission has finished
    while (UART_BUSY) {}

    // Copy the string to write into the buffer used by UART
    // This is neccessary as we do not want to overwrite write_string while a UART transmission is still in progress
    strncpy(write_string, pending_write_string, TRANSMIT_BUFFER + 1);

    // Send the string with the correct length non-blockingly
    if (HAL_UART_Transmit_IT(&huart5, write_buffer, strlen(write_string)) != HAL_OK)
        Error_Handler();

    UART_BUSY = true;
}

void uart_write_id(const char* name, uint8_t id)
{
    snprintf(pending_write_string, TRANSMIT_BUFFER + 1, ">>> %13.13s ID: 0x%2x\n", name, id);
    uart_write();
}

void uart_write_header()
{
    snprintf(pending_write_string, TRANSMIT_BUFFER + 1, " t [s], T [K],   p [Pa],a_x [m/s^2],a_y [m/s^2],a_z [m/s^2],  B_x [T],  B_y [T],  B_z [T]\n");
    uart_write();
}

void uart_write_header_filter_test()
{
    snprintf(pending_write_string, TRANSMIT_BUFFER + 1, " t [s],a_x_raw [m/s^2],a_y_raw [m/s^2],a_z_raw [m/s^2],a_x_filtered [m/s^2],a_y_filtered [m/s^2],a_z_filtered [m/s^2] \n");
    uart_write();
}

void uart_write_data(double* time, double* temperature, double* pressure, double* magnet_x, double* magnet_y, double* magnet_z,
    double* acc_x, double* acc_y, double* acc_z)
{
    snprintf(pending_write_string, TRANSMIT_BUFFER + 1, "%6.2lf,%6.2lf,%9.2lf,    % 7.2lf,    % 7.2lf,    % 7.2lf,% 8lf,% 8lf,% 8lf\n",
        *time, *temperature, *pressure, *acc_x, *acc_y, *acc_z, *magnet_x, *magnet_y, *magnet_z);
    uart_write();
}

void uart_write_filter_test_data(double* time, double* acc_x_raw, double* acc_y_raw, double* acc_z_raw, double* acc_x_filtered, double* acc_y_filtered, double* acc_z_filtered)
{
    snprintf(pending_write_string, TRANSMIT_BUFFER + 1, "%6.2lf, %7.2lf, %7.2lf, %7.2lf, %7.2lf, %7.2lf, %7.2lf\n",
             *time, *acc_x_raw, *acc_y_raw, *acc_z_raw,*acc_x_filtered, *acc_y_filtered, *acc_z_filtered);
    uart_write();
}

void uart_goodbye()
{
    char goodbye[] = GOODBYE_MSG;
    snprintf(pending_write_string, TRANSMIT_BUFFER + 1, "%s\n", goodbye);
    uart_write();
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart)
{
    if (huart == &huart5)
        UART_BUSY = false;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart)
{
    if (huart == &huart5)
        UART_BUSY = false;
}