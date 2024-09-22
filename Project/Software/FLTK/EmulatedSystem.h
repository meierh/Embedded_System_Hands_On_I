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
                        
        void playerTurnOn();
        void playerTurnOff();
        PlayingStatus mp3Status;
        const uint16_t trackNumber = 12;
        uint16_t trackID;
        uint8_t volume;
        void playerControl(PlayerAction action);
        void playerGetState(PlayingStatus &status, uint8_t &volume, uint16_t &numberOfTracks, uint16_t &currentTrack);
        
    private:
        HardwareEmulator* hardware = nullptr;
};
#endif
