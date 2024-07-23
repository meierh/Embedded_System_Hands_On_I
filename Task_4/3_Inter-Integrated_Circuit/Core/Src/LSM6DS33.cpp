//
// Created by Tobias Henig on 23.07.24.
//

#include "LSM6DS33.h"

#define DS33_SA0_LOW_ADDRESS  0b1101010
#define DS33_SA0_HIGH_ADDRESS 0b1101011

bool LSM6DS33::alreadyReset = false;

LSM6DS33::LSM6DS33() : i2c_sensor(DS33_SA0_HIGH_ADDRESS)
{
}

void LSM6DS33::read_accelerometer(double& x, double& y, double& z)
{
    // auto-increment is enabled by default by IF_INC =1 in CTRL3_C
    // we read OUTX_L_XL, OUTX_H_XL, OUTY_L_XL, OUTY_H_XL, OUTZ_L_XL, OUTZ_H_XL
    std::vector<uint8_t> sensorData;
    readVector(sensorData, OUTX_L_XL, 6);

    // combine high and low bytes
    auto raw_x = static_cast<int16_t>(sensorData[1] << 8 | sensorData[0]);
    auto raw_y = static_cast<int16_t>(sensorData[3] << 8 | sensorData[2]);
    auto raw_z = static_cast<int16_t>(sensorData[5] << 8 | sensorData[4]);

    // convert to [m / s^2]
    // first convert to mg (see LA_so on p. 15 in datasheet) with factor 0.061, then to m/s^2 with factor 9.81e-3
    x = static_cast<double>(raw_x) * 0.061 * 9.81e-3;
    y = static_cast<double>(raw_y) * 0.061 * 9.81e-3;
    z = static_cast<double>(raw_z) * 0.061 * 9.81e-3;
}

uint8_t LSM6DS33::readWHO_AM_I()
{
    return readByte(WHO_AM_I);
}

void LSM6DS33::reset_and_config()
{
    if (alreadyReset)
        return;

    // Accelerometer
    // 0x80 = 0b10000000
    /**
     * [7-4] ODR_XL         Output data rate and power mode selection. Default value: 0000 (see Table 44)
     *                      ODR_XL = 1000 (1.66 kHz (high performance));
     * [3-1] FS_XL          Accelerometer full-scale selection. Default value: 00.
                            (00: ±2 g; 01: ±16 g; 10: ±4 g; 11: ±8 g)
                            FS_XL = 00 (+/-2 g full scale)
     * [1-0] BW_XL          Anti-aliasing filter bandwidth selection. Default value: 00
                            (00: 400 Hz; 01: 200 Hz; 10: 100 Hz; 11: 50 Hz)
    */
    writeByte(0x80, CTRL1_XL);

    // Gyro
    // 0x80 = 0b010000000
    /**
     * [7-4] ODR_G          Gyroscope output data rate selection. Default value: 0000 (Refer toTable 46)
     *                      ODR_G = 1000 (1.66 kHz (high performance));
     * [3-1] FS_G           Gyroscope full-scale selection. Default value: 00
     *                      (00: 245 dps; 01: 500 dps; 10: 1000 dps; 11: 2000 dps)
     *                      FS_G = 00 (245 dps)
     * [1-0] FS_125         Gyroscope full-scale at 125 dps. Default value: 0
     +                      (0: disabled; 1: enabled)
    */
    writeByte(0x80, CTRL2_G);

    // Common
    // 0x44 = 0b01000100
    /**
     * [7] BOOT         Reboot memory content. Default value: 0 (0: normal mode; 1: reboot memory content(1))
     * [6] BDU          Block Data Update. Default value: 0
     *                  (0: continuous update; 1: output registers not updated until MSB and LSB have been read)
     * [5] H_LACTIVE    Interrupt activation level. Default value: 0
     *                  (0: interrupt output pads active high; 1: interrupt output pads active low)
     * [4] PP_OD        Push-pull/open-drain selection on INT1 and INT2 pads. Default value: 0
     *                  (0: push-pull mode; 1: open-drain mode)
     + [3] SIM          SPI Serial Interface Mode selection. Default value: 0
     +                  (0: 4-wire interface; 1: 3-wire interface).
     * [2] IF_INC       Register address automatically incremented during a multiple byte access with a
     *                  serial interface (I2C or SPI). Default value: 1
     *                  (0: disabled; 1: enabled)
     *                  IF_INC = 1 (automatically increment register address)
     + [1] BLE          Big/Little Endian Data selection. Default value 0
     *                  (0: data LSB @ lower address; 1: data MSB @ lower address)
     * [0] SW_RESET     Software reset. Default value: 0
     +                  (0: normal mode; 1: reset device)
     +                  This bit is cleared by hardware after next flash boot.
    */
    writeByte(0x44, CTRL3_C);

    alreadyReset = true;
}

void LSM6DS33::wait()
{
    // Once the last bit in STATUS_REG is set, a new acceleration value can be read
    while ((readByte(STATUS_REG) & 0b1) != 0b1);

    // We do not wait for a gyroscope value here as this is not required in the task
}

std::vector<uint16_t> LSM6DS33::readGyroRaw() {
    std::vector<uint8_t> sensorData;
    std::vector<uint8_t> msg;
    // auto-increment is enabled by default by IF_INC =1 in CTRL3_C
    // we read OUTX_L_G, OUTX_H_G, OUTY_L_G, OUTY_H_G, OUTZ_L_G, OUTZ_H_G
    msg.push_back(regAddr::OUTX_L_G);
    std::vector<uint8_t> recMsg;
    readVector(recMsg, OUTX_L_G, 6);

    std::vector<uint16_t> result;

    // recMsg[0]= OUTX_L_G
    // recMsg[1]= OUTX_H_G
    // recMsg[2]= OUTY_L_G
    // recMsg[3]= OUTY_H_G
    // recMsg[4]= OUTZ_L_G
    // recMsg[5]= OUTZ_H_G

    // combine high and low bytes
    uint16_t x = (int16_t)(recMsg[1] << 8 | recMsg[0]);
    uint16_t y = (int16_t)(recMsg[3] << 8 | recMsg[2]);
    uint16_t z = (int16_t)(recMsg[5] << 8 | recMsg[4]);
    result.push_back(x);
    result.push_back(y);
    result.push_back(z);

    return result;
}
