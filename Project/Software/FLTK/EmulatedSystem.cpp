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

void EmulatedSystem::playerTurnOn()
{
    System::playerTurnOn();
    mp3Status = PlayingStatus::STATUS_PAUSED;
    trackID = 1;
    volume = 15;
}

void EmulatedSystem::playerTurnOff()
{
    System::playerTurnOff();
}

void EmulatedSystem::playerControl(PlayerAction action)
{
    switch(action)
    {
        case PlayerAction::PlayAlarm:
            mp3Status = PlayingStatus::STATUS_PLAYING;
            std::cout<<"!!!! ALARM !!!!"<<std::endl;
            break;
        case PlayerAction::StopAlarm:
            mp3Status = PlayingStatus::STATUS_PAUSED;
            std::cout<<"!!!! End ALARM !!!!"<<std::endl;
            break;
        case PlayerAction::Play:
            mp3Status = PlayingStatus::STATUS_PLAYING;
            break;
        case PlayerAction::Pause:
            mp3Status = PlayingStatus::STATUS_PAUSED;
            break;
        case PlayerAction::Next:
            trackID = (trackID==trackNumber) ? 1 : trackID+1;
            break;
        case PlayerAction::Previous:
            trackID = (trackID==1) ? trackNumber : trackID-1;
            break;
        case PlayerAction::IncreaseVolume:
            volume++;
            break;
        case PlayerAction::DecreaseVolume:
            volume--;
            break;
    }
}

void EmulatedSystem::playerGetState
(
    PlayingStatus &status,
    uint8_t &volume,
    uint16_t &numberOfTracks,
    uint16_t &currentTrack
)
{
    status = mp3Status;
    volume = this->volume;
    numberOfTracks = trackNumber;
    currentTrack = trackID;
}
