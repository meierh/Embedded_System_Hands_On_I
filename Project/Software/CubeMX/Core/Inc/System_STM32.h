#ifndef SYSTEM_STM32_H
#define SYSTEM_STM32_H

#include <vector>
#include "System.h"
#include "GUI_Paint.h"
#include "OLED_1in5.h"
#include "DS3231.h"


class System_STM32 : public System
{
    public:
        System_STM32();
        ~System_STM32();
        
        // Application -> System
        void displayImage(std::vector<DisplayItem> image) override;
        DateTime getSystemTime() override;
        int8_t getSeconds() override;
        void setSystemTime(DateTime newTime) override;
        // void playSound(uint8_t soundFile, uint8_t soundFolder) override;
};
#endif
