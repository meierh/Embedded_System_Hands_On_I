#ifndef SYSTEM_STM32_H
#define SYSTEM_STM32_H

#include <vector>
#include "System.h"
#include "GUI_Paint.h"
#include "OLED_1in5.h"
#include "DS3231.h"
#include "main.h"


class System_STM32 : public System
{
    public:
        System_STM32();
        ~System_STM32() override;
        
        // Application -> System
        void buttonModePress() override;
        void displayImage(std::vector<DisplayItem> image) override;
        DateTime getSystemTime() override;
        int8_t getSeconds() override;
        void setSystemTime(DateTime newTime) override;

        uint8_t getBattery() override;

        void playerTurnOn() override;
        void playerTurnOff() override;
        void playerControl(PlayerAction action) override;
        void playerGetState(PlayingStatus &status, uint8_t &volume, uint16_t &numberOfTracks, uint16_t &currentTrack) override;

};
#endif
