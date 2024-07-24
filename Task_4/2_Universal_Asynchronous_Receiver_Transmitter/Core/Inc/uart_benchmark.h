//
// Created by Tobias Henig on 24.07.24.
//

#ifndef UART_BENCHMARK_H
#define UART_BENCHMARK_H

/**
 * Benchmark settings. These need to be the same as in main.py
 *
 * TEST_DATA_AMOUNT - number of Receive/Transmit transactions per baudrate
 * TEST_DATA_WIDTH  - number of bytes per transaction
 * UART_BAUDRATES   - the baudrates to test
 */
#define TEST_DATA_WIDTH 10
#define TEST_DATA_AMOUNT 1000
#define UART_BAUDRATES {9600, 19200, 57600, 115200}

void uart_echo_with_baudrate(uint32_t baudrate);
void uart_benchmark();

#endif //UART_BENCHMARK_H
