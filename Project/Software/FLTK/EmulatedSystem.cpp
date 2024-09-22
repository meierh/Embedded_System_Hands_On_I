#include "EmulatedSystem.h"

EmulatedSystem::EmulatedSystem
(
    HardwareEmulator* hardware
):
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
    std::cout<<"EmulatedSystem::displayImage:"<<image.size()<<std::endl;
    if(hardware!=nullptr)
        hardware->displayImage(image);
    else
        std::cout<<"Error: EmulatedSystem::displayImage nullptr"<<std::endl;
}

DateTime EmulatedSystem::getSystemTime()
{
    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    DateTime timeUnix(time);
    return timeUnix;
}

void EmulatedSystem::setSystemTime(DateTime newTime)
{
    std::string time = std::to_string(newTime.getHour())+":"+std::to_string(newTime.getMinute());
    std::cout<<"Set time to:"<<time<<std::endl;
}

uint8_t EmulatedSystem::getBattery()
{
    batteryStatus++;
    batteryStatus = batteryStatus>3?0:batteryStatus;
    return batteryStatus;
}
