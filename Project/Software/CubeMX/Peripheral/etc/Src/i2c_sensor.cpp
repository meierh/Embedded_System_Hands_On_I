//
// Created by nico on 13.07.24.
//

#include "i2c_sensor.h"

#include <cstdio>

bool i2c_sensor::checkStateOfI2c() {
    // check state of i2c
    HAL_I2C_StateTypeDef i2c_state = HAL_I2C_GetState(&hi2c1);

    return i2c_state == HAL_I2C_STATE_READY;
}

i2c_sensor::i2c_sensor(uint16_t address) : address(address)
{

}

bool i2c_sensor::checkDeviceStatus() const
{
    // check if device is ready
    HAL_StatusTypeDef state = HAL_I2C_IsDeviceReady(&hi2c1, address << 1, 5, 1000);

    return state == HAL_OK;
}

void i2c_sensor::writeVector(std::vector<uint8_t>& data, uint8_t memoryAddress) const
{
    // to write to the device, we have to shift the 7 bit address to a 8 bit address. The last bit does not matter for the HAL functions.
    uint16_t deviceAddress = address << 1;

    if (!checkStateOfI2c())
        Error_Handler_Message("I2C peripheral    not ready");

    if (!checkDeviceStatus())
    {
        char errorMessage[37];
        snprintf(errorMessage, 37, "I2C device %x     not ready", static_cast<int>(address));
        Error_Handler_Message(errorMessage);
    }

    if (HAL_I2C_Mem_Write(&hi2c1, deviceAddress, memoryAddress, I2C_MEMADD_SIZE_8BIT, &data.at(0), data.size(), 10000) != HAL_OK) {
        uint32_t errorCode = HAL_I2C_GetError(&hi2c1);

        char errorMessage[37];
        snprintf(errorMessage, 37, "I2C write %x/%x   error %x", static_cast<int>(address), static_cast<int>(memoryAddress), static_cast<int>(errorCode));
        Error_Handler_Message(errorMessage);
    }
}

void i2c_sensor::readVector(std::vector<uint8_t>& data, uint8_t memoryAddress, uint16_t size) const
{
    // to read from the device, we have to shift the 7 bit address to a 8 bit address. The last bit does not matter for the HAL functions.
    uint16_t readAddress = ((address << 1) | 0x1);

    if (!checkStateOfI2c())
        Error_Handler_Message("I2C peripheral    not ready");

    if (!checkDeviceStatus())
    {
        char errorMessage[37];
        snprintf(errorMessage, 37, "I2C device %x     not ready", static_cast<int>(address));
        Error_Handler_Message(errorMessage);
    }

    // read the mem value and push the result to the data vector
    uint8_t pData[size];
    if (HAL_I2C_Mem_Read(&hi2c1, readAddress, memoryAddress, I2C_MEMADD_SIZE_8BIT, pData, size, 10000) != HAL_OK) {
        uint32_t errorCode = HAL_I2C_GetError(&hi2c1);

        char errorMessage[37];
        snprintf(errorMessage, 37, "I2C read %x/%x    error %x", static_cast<int>(address), static_cast<int>(memoryAddress), static_cast<int>(errorCode));
        Error_Handler_Message(errorMessage);
    }

    // push every value of the buffer to the vector
    for(int i = 0; i < size; i++) {
        data.push_back(pData[i]);
    }
}

void i2c_sensor::writeByte(uint8_t data, uint8_t memoryAddress) const
{
    // to write to the device, we have to shift the 7 bit address to a 8 bit address. The last bit does not matter for the HAL functions.
    uint16_t deviceAddress = address << 1;

    if (!checkStateOfI2c())
        Error_Handler_Message("I2C peripheral    not ready");

    if (!checkDeviceStatus())
    {
        char errorMessage[37];
        snprintf(errorMessage, 37, "I2C device %x     not ready", static_cast<int>(address));
        Error_Handler_Message(errorMessage);
    }

    if (HAL_I2C_Mem_Write(&hi2c1, deviceAddress, memoryAddress, I2C_MEMADD_SIZE_8BIT, &data, 1, 10000) != HAL_OK) {
        uint32_t errorCode = HAL_I2C_GetError(&hi2c1);

        char errorMessage[37];
        snprintf(errorMessage, 37, "I2C write %x/%x   error %x", static_cast<int>(address), static_cast<int>(memoryAddress), static_cast<int>(errorCode));
        Error_Handler_Message(errorMessage);
    }
}

uint8_t i2c_sensor::readByte(uint8_t memoryAddress) const
{
    // to read from the device, we have to shift the 7 bit address to a 8 bit address. The last bit does not matter for the HAL functions.
    uint16_t readAddress = ((address << 1) | 0x1);

    uint8_t data;

    if (!checkStateOfI2c())
        Error_Handler_Message("I2C peripheral    not ready");

    if (!checkDeviceStatus())
    {
        char errorMessage[37];
        snprintf(errorMessage, 37, "I2C device %x     not ready", static_cast<int>(address));
        Error_Handler_Message(errorMessage);
    }

    if (HAL_I2C_Mem_Read(&hi2c1, readAddress, memoryAddress, I2C_MEMADD_SIZE_8BIT, &data, 1, 10000) != HAL_OK) {
        uint32_t errorCode = HAL_I2C_GetError(&hi2c1);

        char errorMessage[37];
        snprintf(errorMessage, 37, "I2C read %x/%x    error %x", static_cast<int>(address), static_cast<int>(memoryAddress), static_cast<int>(errorCode));
        Error_Handler_Message(errorMessage);
    }

    return data;
}

