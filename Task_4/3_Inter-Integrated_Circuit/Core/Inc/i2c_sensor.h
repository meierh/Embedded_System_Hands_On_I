//
// Created by nico on 13.07.24.
//

#ifndef INC_3_INTER_INTEGRATED_CIRCUIT_I2C_SENSOR_H
#define INC_3_INTER_INTEGRATED_CIRCUIT_I2C_SENSOR_H


#include "main.h"
#include "array.h"
#include <vector>


class i2c_sensor {
public:

    uint16_t address;
    i2c_sensor() {

    }

    template<uint16_t sizeTemp>
    void write(array<sizeTemp>& data) {
        //HAL_I2C_Master_Transmit(&hi2c1, address, data.data(), data.size(), 1000);
        //HAL_I2C_Master_Transmit_IT(&hi2c1, address, data.data(), data.size());
    }

    template<uint16_t sizeTemp>
    void read(array<sizeTemp>& data) {
        HAL_I2C_Master_Receive(&hi2c1, address, data.data(), data.size(), 1000);
        //HAL_I2C_Master_Receive_IT(&hi2c1, address, data.data(), data.size());
    }

    void writeVector(std::vector<uint8_t>&  data) {
        //HAL_I2C_Master_Transmit(&hi2c1, address, data.data(), data.size(), 1000);
        //HAL_I2C_Master_Transmit_IT(&hi2c1, address, data.data(), data.size());

        while(HAL_I2C_Mem_Write(&hi2c1, address, 0x0F, 1, data.data(), 1,10000) != HAL_OK);
    }


    void readVector(std::vector<uint8_t>& data) {
        uint16_t newAddress = 0xb9;

        HAL_StatusTypeDef state = HAL_I2C_IsDeviceReady(&hi2c1, newAddress, 5, 1000);

        if(state != HAL_OK) {
            bool error = 1;
            uint32_t errorCode = HAL_I2C_GetError(&hi2c1);
            Error_Handler();
        }

        if(HAL_I2C_Mem_Read(&hi2c1, newAddress, 0x0F, I2C_MEMADD_SIZE_8BIT, data.data(), 1, 1000) != HAL_OK) {
            Error_Handler();
        }
    }



};


#endif //INC_3_INTER_INTEGRATED_CIRCUIT_I2C_SENSOR_H
