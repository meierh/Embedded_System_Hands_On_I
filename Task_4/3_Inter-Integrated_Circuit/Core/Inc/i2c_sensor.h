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

    static bool checkStateOfI2c() {
        // check state of i2c
        HAL_I2C_StateTypeDef i2c_state = HAL_I2C_GetState(&hi2c1);

        if(i2c_state != HAL_I2C_STATE_READY) {
            // i2c not ready
            return false;
        } else {
            return true;
        }
    }

    static bool checkDeviceStatus(uint16_t readAddress) {
        // check if device is ready
        HAL_StatusTypeDef state = HAL_I2C_IsDeviceReady(&hi2c1, readAddress, 5, 1000);

        if(state != HAL_OK) {
            //uint32_t errorCode = HAL_I2C_GetError(&hi2c1);
            return false;
        }else  {
            return true;
        }
    }


    void writeVector(std::vector<uint8_t>&  data, std::vector<uint8_t>&msg) const {
        // to write to the device, we have to shift the 7 bit address to a 8 bit address. The last bit is 0 for write
        uint16_t readAddress = address << 1;

        if(!checkStateOfI2c()) {
            return;
        }

        if(!checkDeviceStatus(readAddress)){
            return;
        }

        if(HAL_I2C_Mem_Write(&hi2c1, readAddress, msg.at(0), I2C_MEMADD_SIZE_8BIT, &data.at(0), 1, 10000) != HAL_OK) {
            // @todo improve error handling
            uint32_t errorCode = HAL_I2C_GetError(&hi2c1);
            data.push_back(errorCode);
            Error_Handler();
        }
    }

    void readVector(std::vector<uint8_t>& data, std::vector<uint8_t>& msg, uint16_t size) const {
        // to read from the device, we have to shift the 7 bit address to a 8 bit address and set the last bit to 1 for reading
        uint16_t readAddress = ((address << 1) |0x1);

        if(!checkStateOfI2c()) {
            return;
        }

        if(!checkDeviceStatus(readAddress)){
            return;
        }

        // read the mem value and push the result to the data vector
        uint8_t pData[size];
        if(HAL_I2C_Mem_Read(&hi2c1, readAddress, msg.at(0), I2C_MEMADD_SIZE_8BIT, pData, size, 10000) != HAL_OK) {
            //@todo improve error handling
            uint32_t errorCode = HAL_I2C_GetError(&hi2c1);
            data.push_back(errorCode);
            Error_Handler();
        }
        // push every value of the buffer to the vector
        uint8_t size_of_data = sizeof(pData);
        for(int i=0; i< size_of_data; i++) {
            data.push_back(pData[i]);
        };
    }


    /* i2c Error Codes copies from stm32f0xx_hal_i2c.h
    * #define HAL_I2C_ERROR_NONE      (0x00000000U)       !< No error
    * #define HAL_I2C_ERROR_BERR      (0x00000001U)    !< BERR error
    * #define HAL_I2C_ERROR_ARLO      (0x00000002U)    !< ARLO error
    * #define HAL_I2C_ERROR_AF        (0x00000004U)    !< ACKF error
    * #define HAL_I2C_ERROR_OVR       (0x00000008U)    !< OVR error
    * #define HAL_I2C_ERROR_DMA       (0x00000010U)    !< DMA transfer error
    * #define HAL_I2C_ERROR_TIMEOUT   (0x00000020U)    !< Timeout error
    * #define HAL_I2C_ERROR_SIZE      (0x00000040U)    !< Size Management error
    * #define HAL_I2C_ERROR_DMA_PARAM (0x00000080U)    !< DMA Parameter Error
    */

};


#endif //INC_3_INTER_INTEGRATED_CIRCUIT_I2C_SENSOR_H
