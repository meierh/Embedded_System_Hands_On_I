//
// Created by Tobias Henig on 23.07.24.
//

#include "LIS3MDL.h"

#define LIS3MDL_SA1_LOW_ADDRESS   0b0011100
#define LIS3MDL_SA1_HIGH_ADDRESS  0b0011110

bool LIS3MDL::alreadyReset = false;

LIS3MDL::LIS3MDL() : i2c_sensor(LIS3MDL_SA1_HIGH_ADDRESS)
{
    LIS3MDL::reset_and_config();
}

void LIS3MDL::read_magnetometer(double& x, double& y, double& z)
{
    wait();

    std::vector<uint8_t> sensorData;
    // assert MSB to enable register address auto-increment
    readVector(sensorData, OUT_X_L | 1 << 7, 6);

    // concatenate values
    auto raw_x = static_cast<int16_t>(sensorData[1] << 8 | sensorData[0]);
    auto raw_y = static_cast<int16_t>(sensorData[3] << 8 | sensorData[2]);
    auto raw_z = static_cast<int16_t>(sensorData[5] << 8 | sensorData[4]);

    // convert to T
    // first convert to gauss (see GN on p. 8 in datasheet) (divide by 6842), then to T with factor 1e-4
    x = static_cast<double>(raw_x) / 6842 * 1e-4;
    y = static_cast<double>(raw_y) / 6842 * 1e-4;
    z = static_cast<double>(raw_z) / 6842 * 1e-4;
}

uint8_t LIS3MDL::readWHO_AM_I()
{
    return readByte(WHO_AM_I);
}

void LIS3MDL::reset_and_config()
{
    if (alreadyReset)
        return;

    // 0xF0 = 0b11110000
    /**
     * [7] TEMP_EN          Temperature sensor enable. Default value: 0
     *                      (0: temperature sensor disabled; 1: temperature sensor enabled)
     *                      Temp_EN = 1
     * [6-5] OM             X and Y axes operative mode selection. Default value: 00
     *                      (Refer to Table 21)
     *                      OM = 11 (ultra-high-performance mode for X and Y)
     * [4-2] DO             Output data rate selection. Default value: 100
     *                      (Refer to Table 22)
     *                      DO = 100 (10 Hz ODR)
     * [1] FAST_ODRFAST_ODR enables data rates higher than 80 Hz (refer to Table 20).
     *                      Default value: 0
     *                      (0: Fast_ODR disabled; 1: FAST_ODR enabled)
     * [0] ST               Self-test enable. Default value: 0
     *                      (0: self-test disabled; 1: self-test enabled)
     */
    writeByte(0xF0, CTRL_REG1);

    // 0x00 = 0b00000000
    /**
     * [7]                  must be set to ‘0’ for correct functioning of the device
     * [6-5] FS             Full-scale configuration. Default value: 00
     *                      Refer to Table 25
     *                      FS = 00 (+/- 4 gauss full scale)
     * [4]                  must be set to ‘0’ for correct functioning of the device
     * [3] REBOOT           Reboot memory content. Default value: 0
     *                      (0: normal mode; 1: reboot memory content)
     * [2] SOFT_RST         Configuration registers and user register reset function.
     *                      (0: Default value; 1: Reset operation)
     * [1-0]                must be set to ‘0’ for correct functioning of the device
    */
    writeByte(0x00, CTRL_REG2);

    // 0x00 = 0b00000000
    /**
     * [7-6]                must be set to ‘0’ for correct functioning of the device
     * [5] LP               Low-power mode configuration. Default value: 0
     *                      If this bit is ‘1’, DO[2:0] is set to 0.625 Hz and the system performs, for each
     *                      channel, the minimum number of averages. Once the bit is set to ‘0’, the mag-
     *                      netic data rate is configured by the DO bits in CTRL_REG1 (20h) register.
     * [4-3]                must be set to ‘0’ for correct functioning of the device
     * [2] SIM              SPI serial interface mode selection. Default value: 0
     *                      (0: 4-wire interface; 1: 3-wire interface).
     * [1-0] MD             Operating mode selection. Default value: 11
     *                      Refer to Table 28
     *                      MD = 00 (continuous-conversion mode)
    */
    writeByte(0x00, CTRL_REG3);

    // 0x0C = 0b00001100
    /**
    * [7-4]                must be set to ‘0’ for correct functioning of the device
    * [3-2] OMZ            Z-axis operative mode selection.
                           Default value: 00. Refer to Table 31.
                           OMZ = 11 (ultra-high-performance mode for Z)
    * [1] BLE              Big/Little Endian data selection. Default value: 0
                           (0: data LSb at lower address; 1: data MSb at lower address)
    * [0]                  must be set to ‘0’ for correct functioning of the device
    */
    writeByte(0x0C, CTRL_REG4);

    // 0x40 = 0b01000000
    /**
      * [7] FAST_READ           FAST READ allows reading the high part of DATA OUT only in order to increase
      *                         reading efficiency. Default value: 0
      *                         (0: FAST_READ disabled; 1: FAST_READ enabled)
      * [6] BDU                 Block data update for magnetic data. Default value: 0
      *                         (0: continuous update;
      *                         1: output registers not updated until MSb and LSb have been read)
      *                         Default value: 00. Refer to Table 31.
         *                      BDU = 1 (block data update)
      * [5-0]                   must be set to ‘0’ for correct functioning of the device
     */
    writeByte(0x40, CTRL_REG5);

    alreadyReset = true;
}

void LIS3MDL::wait()
{
    // Once the last for bits in STATUS_REG are set, a new magnetic value can be read
    while ((readByte(STATUS_REG) & 0b1111) != 0b1111);
}
