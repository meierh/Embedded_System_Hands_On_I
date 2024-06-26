#ifndef UART_H
#define UART_H

#include <bitset>
#include <fstream>
#include <vector>
#include <iostream>
#include <pigpio.h>

#define FRAME_SIZE 7

void uart_send(std::vector<bool> data);
void uart_receive(std::vector<bool>& data, uint size);

void uart_frame_send(std::bitset<FRAMSE_SIZE> frame);
void uart_frame_receive(std::bitset<FRAMSE_SIZE>& frame);

bool partiy(const std::bitset<FRAMSE_SIZE>& frame);

#endif
