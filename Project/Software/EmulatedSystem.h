#ifndef EMULATEDSYSTEM_H
#define EMULATEDSYSTEM_H
#include "System.h"

class EmulatedSystem : public System
{
    public:
        EmulatedSystem(HardwareEmulator* hardware);
        //Application -> System
        void displayImage(std::vector<DisplayItem> image) override;
        std::string getSystemTime() override;
        
    private:
        //HardwareEmulator* setHardwarePointer(HardwareEmulator* hardware);
        HardwareEmulator* hardware = nullptr;
};
#endif
