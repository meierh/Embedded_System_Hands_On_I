//
// Created by Tobias Henig on 24.07.24.
//

#include "main.h"
#include "uart_benchmark.h"

uint8_t rx_buff[TEST_DATA_WIDTH];
extern UART_HandleTypeDef huart5;

void uart_echo_with_baudrate(uint32_t baudrate)
{
    // set up UART to the correct baudrate
    huart5.Init.BaudRate = baudrate;
    if (HAL_UART_Init(&huart5) != HAL_OK)
        Error_Handler();

    // echo back received symbols
    for (int i = 0; i < TEST_DATA_AMOUNT; i++)
    {
        HAL_UART_Receive(&huart5, rx_buff, sizeof(rx_buff), HAL_MAX_DELAY);
        HAL_UART_Transmit(&huart5, rx_buff, sizeof(rx_buff), HAL_MAX_DELAY);
    }
}

void uart_benchmark()
{
    uint32_t baudrates[] = UART_BAUDRATES;
    for (int i = 0; i < sizeof(baudrates) / sizeof(baudrates[0]); i++)
        uart_echo_with_baudrate(baudrates[i]);
}
