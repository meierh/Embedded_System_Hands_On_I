#include "EmulatedSystem.h"

EmulatedSystem::EmulatedSystem
(
    HardwareEmulator* hardware
):
//hardware(setHardwarePointer(hardware)),
System(),
hardware(hardware)
{
    if(hardware==nullptr)
        std::cout<<"Error: No hardware given!"<<std::endl;
    hardware->setSystem(this);
    
    std::cout<<"Setup EmulatedSystem"<<std::endl;
}

void EmulatedSystem::displayImage(std::vector<DisplayItem> image)
{
    if(hardware!=nullptr)
        hardware->displayImage(image);
    else
        std::cout<<"Error: EmulatedSystem::displayImage nullptr"<<std::endl;
}
