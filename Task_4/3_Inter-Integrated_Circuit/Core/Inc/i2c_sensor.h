//
// Created by nico on 13.07.24.
//

#ifndef INC_3_INTER_INTEGRATED_CIRCUIT_I2C_SENSOR_H
#define INC_3_INTER_INTEGRATED_CIRCUIT_I2C_SENSOR_H

#include "main.h"
#include <vector>


class i2c_sensor {

private:
    uint16_t address;

public:
    /**
     * Checks whether the I2C device is ready
     * @return true iff the state of the I2C device is HAL_I2C_STATE_READY
     */
    static bool checkStateOfI2c();

    /**
     * Creates a new I2C sensor device connection
     * @param address the device address of the sensor
     */
    explicit i2c_sensor(uint16_t address);

    /**
     * Checks whether the sensor is ready
     * @return true iff the sensor is ready
     */
    [[nodiscard]] bool checkDeviceStatus() const;

    /**
     * Writes a variable amount of data to the sensor
     * Caution: The caller has to ensure that the device is able to auto-increment the memory addresses
     * @param data The bytes to be written to the sensor
     * @param memoryAddress The sensor's address of the first byte to be written
     */
    void writeVector(std::vector<uint8_t>& data, uint8_t memoryAddress) const;
    /**
     * Reads a variable amount of data from the sensor
     * Caution: The caller has to ensure that the device is able to auto-increment the memory addresses
     * @param data A vector for the read bytes to be written to
     * @param memoryAddress The sensor's address of the first byte to be read
     * @param size The amount of bytes to be read
     */
    void readVector(std::vector<uint8_t>& data, uint8_t memoryAddress, uint16_t size) const;

    /**
     * Writes one byte of data to the sensor
     * @param data The byte to be written
     * @param memoryAddress The sensor's address of the byte
     */
    void writeByte(uint8_t data, uint8_t memoryAddress) const;
    /**
     * Reads one byte of data from the sensor
     * @param memoryAddress The sensor's address of the byte to be read
     * @return the byte read.
     */
    uint8_t readByte(uint8_t memoryAddress) const;


    /* i2c Error Codes copied from stm32f0xx_hal_i2c.h
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
