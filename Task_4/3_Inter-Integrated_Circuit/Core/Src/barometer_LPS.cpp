//
// Created by nico on 13.07.24.
//

#include "barometer_LPS.h"
#include <cstdint>
#include <vector>

#define SA0_LOW_ADDRESS  0b1011100
#define SA0_HIGH_ADDRESS 0b1011101



barometer_LPS::barometer_LPS() : i2c_sensor() {
    address = SA0_HIGH_ADDRESS;
}


uint8_t barometer_LPS::readWhoAmI(void) {

    /*array<1> msg;
    msg[0] = regAddr::WHO_AM_I;
    write<1>(msg);

    array<1> sensorData;
    read<1>(sensorData);
    uint8_t result = sensorData[0];

    return result;*/



    //std::vector<uint8_t> msg;
    //msg.push_back(regAddr::WHO_AM_I);
    //writeVector(msg);

    std::vector<uint8_t> revMsg;
    readVector(revMsg);

    return revMsg.at(0);
}

int32_t barometer_LPS::readPressure(void) {
    array<1> msg;
    msg[0] = regAddr::PRESS_OUT_XL;
    write<1>(msg);


    array<3> sensorData;
    read<3>(sensorData);
    int32_t result = ((int32_t) (int8_t)sensorData[2]) << 16 | (uint16_t)sensorData[1] << 8 | sensorData[0];  //@todo

    return result;
}

float barometer_LPS::readPressureMillibars(void) {
    return (float) readPressure() / 4096; //@todo
}
