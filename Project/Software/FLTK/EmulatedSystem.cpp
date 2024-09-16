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

std::string EmulatedSystem::getSystemTime()
{
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    return std::string(std::ctime(&end_time));
}
