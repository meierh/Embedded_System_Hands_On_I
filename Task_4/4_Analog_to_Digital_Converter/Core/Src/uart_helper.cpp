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

uint32_t uart::read_num()
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
static void write()
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

void uart::write_header()
{
    snprintf(pending_write_string, TRANSMIT_BUFFER + 1, " t [s],E_V [%%]\n");
    write();
}

void uart::write_data(double* time, double* illuminance_percentage)
{
    snprintf(pending_write_string, TRANSMIT_BUFFER + 1, "%6.2lf,% 7.2lf\n", *time, *illuminance_percentage);
    write();
}

void uart::write_calibration(light_sensor_calibration *calibration)
{
    snprintf(pending_write_string, TRANSMIT_BUFFER + 1, ">>> Calibration: min = %7.2lf, max = %7.2lf\n",
        calibration->get_min_illuminance(), calibration->get_max_illuminance());
    write();
}

void uart::write_msg(const char* msg)
{
    snprintf(pending_write_string, TRANSMIT_BUFFER + 1, ">>> %s\n", msg);
    write();
}

void uart::goodbye()
{
    char goodbye[] = GOODBYE_MSG;
    snprintf(pending_write_string, TRANSMIT_BUFFER + 1, "%s\n", goodbye);
    write();
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