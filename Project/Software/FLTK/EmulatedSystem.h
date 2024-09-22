#ifndef EMULATEDSYSTEM_H
#define EMULATEDSYSTEM_H
#include <chrono>
#include <ctime>
#include "System.h"
#include "HardwareEmulator.h"

class EmulatedSystem : public System
{
    public:
        EmulatedSystem(HardwareEmulator* hardware);
        
        //Application -> System
        void displayImage(std::vector<DisplayItem> image) override;
        DateTime getSystemTime() override;
        int8_t getSeconds() override {return 0;};
        void setSystemTime(DateTime newTime) override;
        uint8_t getBattery() override;
        uint8_t batteryStatus = 0;
        
    private:
        HardwareEmulator* hardware = nullptr;
};
#endif
