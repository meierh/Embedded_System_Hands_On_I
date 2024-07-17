#define LIS3MDL_SA1_LOW_ADDRESS   0b0011100
#define LIS3MDL_SA1_HIGH_ADDRESS  0b0011110

#include "magnetometer_LIS3MDL.h"

magnetometer_LIS3MDL::magnetometer_LIS3MDL() : i2c_sensor() {
    address = LIS3MDL_SA1_HIGH_ADDRESS;
}

uint8_t magnetometer_LIS3MDL::readWhoAmI(void) {
    std::vector<uint8_t> msg;
    msg.push_back(regAddr::WHO_AM_I);

    std::vector<uint8_t> revMsg;
    readVector(revMsg, msg, 1);

    return revMsg.at(0);
}

/**
 * enable and setup the sensor
 */
void magnetometer_LIS3MDL::enableSensor() {
    std::vector<uint8_t> msg;
    std::vector<uint8_t> value;

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
    msg.push_back(regAddr::CTRL_REG1);
    value.push_back(0xF0);
    writeVector(value, msg);

    msg.pop_back();
    value.pop_back();

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
    //
    msg.push_back(regAddr::CTRL_REG2);
    value.push_back(0x00);
    writeVector(value, msg);

    msg.pop_back();
    value.pop_back();

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
    msg.push_back(regAddr::CTRL_REG3);
    value.push_back(0x00);
    writeVector(value, msg);

    msg.pop_back();
    value.pop_back();

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
    msg.push_back(regAddr::CTRL_REG4);
    value.push_back(0x0C);
    writeVector(value, msg);

    msg.pop_back();
    value.pop_back();

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
    msg.push_back(regAddr::CTRL_REG5);
    value.push_back(0x40);
    writeVector(value, msg);
}

/**
 * read the magnetic field and return a vector with the results
 * The value of magnetic field is expressed as two’s complement.
 * [0] = x-Value
 * [1] = y-value
 * [2] = z-value
 * @return
 */
std::vector<uint8_t> magnetometer_LIS3MDL::readMagneticField(void) {
    std::vector<uint8_t> sensorData;
    std::vector<uint8_t> msg;
    // assert MSB to enable register address auto-increment
    // we read OUT_X_L, OUT_Y_L, OUT_Z_L
    msg.push_back(regAddr::OUT_X_L | (1 << 7));
    std::vector<uint8_t> recMsg;
    readVector(recMsg, msg, 3);

    return recMsg;
}

/**
 * read the temperature value
 * @return Temperature sensor data. The value of temperature is expressed as two’s complement.
 */
uint8_t magnetometer_LIS3MDL::readTemperature(void) {
    std::vector<uint8_t> sensorData;
    std::vector<uint8_t> msg;
    // assert MSB to enable register address auto-increment
    // we read TEMP_OUT_L
    msg.push_back(regAddr::TEMP_OUT_L);
    std::vector<uint8_t> recMsg;
    readVector(recMsg, msg, 1);

    return recMsg.at(0);
}

