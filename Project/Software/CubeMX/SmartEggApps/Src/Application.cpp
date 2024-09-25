#include "System.h"
#include "Application.h"

Application::Application
(
    System* system
):
modeText(10,2,DisplayItem::Font8,"Mode",255),
modeStatus(10,30,DisplayItem::Font8,"",128),
time(115,100,DisplayItem::Font8,"00:00",255),
buttonLine(118,0,118,128,255),
leftCenterButtonLine(118,43,128,43,255),
centerRightButtonLine(118,85,128,85,255),
leftButtonLabel(127,15,DisplayItem::Font8,"Left",200),
centerButtonLabel(127,52,DisplayItem::Font8,"Center",200),
rightButtonLabel(127,98,DisplayItem::Font8,"Right",200),
timesUpLabel(70,8,DisplayItem::Font20,"XXXXXXX",255),
system(system)
{
    timesUpLabel.setType(DisplayItem::Empty);
    
    int offsetW = 108;
    int offsetH = 9;
    int barHeight = 2;
    for(int batteryStep=0; batteryStep<4; batteryStep++)
    {
        for(int w=0; w<3; w++)
            batteryStatus[batteryStep][w] = DisplayItem(offsetH-barHeight,offsetW+w,offsetH,offsetW+w,255);
        offsetW += 5;
        barHeight += 2;
    }

    collectItems();
}

void Application::close()
{
    while (!inputActions.empty())
    {
        work();
    }
}

void Application::onButtonLeftClick()
{
    inputActions.push(Action::BtnLeftClick);
}

void Application::onButtonCenterClick()
{
    inputActions.push(Action::BtnCenterClick);
}

void Application::onButtonRightClick()
{
    inputActions.push(Action::BtnRightClick);
}

void Application::onRotationClick(std::int8_t dir)
{
    if(dir==1)
        inputActions.push(Action::RotateClock);
    else
        inputActions.push(Action::RotateAntiClock);
}

void Application::onPeriod()
{
    inputActions.push(Action::OnePeriod);
}

void Application::displayCommand(const std::vector<DisplayItem>& items)
{
    if(system!=nullptr)
        system->displayImage(items);
}

void Application::collectItems()
{
    displayImage.clear();
    
    displayImage.push_back(modeText);
    displayImage.push_back(modeStatus);
    
    updateBattery();
    for(int batteryStep=0; batteryStep<4; batteryStep++)
    {
        for(int w=0; w<6; w++)
            displayImage.push_back(batteryStatus[batteryStep][w]);
    }
    
    updateClock();
    displayImage.push_back(time);
    
    displayImage.push_back(buttonLine);
    displayImage.push_back(leftCenterButtonLine);
    displayImage.push_back(centerRightButtonLine);
    displayImage.push_back(leftButtonLabel);
    displayImage.push_back(centerButtonLabel);
    displayImage.push_back(rightButtonLabel);
    displayImage.push_back(timesUpLabel);
}

void Application::setBatteryBar(uint8_t barInd)
{
    for(int w=0; w<3; w++)
        batteryStatus[barInd][w].setType(DisplayItem::Line);
}

void Application::unsetBatteryBar(uint8_t barInd)
{
    for(int w=0; w<3; w++)
        batteryStatus[barInd][w].setType(DisplayItem::Empty);
}

void Application::setTimesUp()
{
    system->playerTurnOn(true);
    system->playerControl(System::PlayerAction::PlayAlarm);
    timesUpLabel.setType(DisplayItem::Text);
}

void Application::unsetTimesUp()
{
    timesUpLabel.setType(DisplayItem::Empty);
    system->playerControl(System::PlayerAction::StopAlarm);
    system->playerTurnOff();
}

void Application::updateClock()
{
    currTime = system->getSystemTime();
    std::string timeString;
    
    if(currTime.getHour()<10)
        timeString += "0"+ std::to_string(currTime.getHour());
    else
        timeString += std::to_string(currTime.getHour());
    timeString += ":";
    if(currTime.getMinute()<10)
        timeString += "0"+ std::to_string(currTime.getMinute());
    else
        timeString += std::to_string(currTime.getMinute());
    
    time.characters = timeString;
}

void Application::updateBattery()
{
    uint8_t batteryStatus = system->getBattery();
    switch(batteryStatus)
    {
        case 0:
            setBatteryBar(0);
            unsetBatteryBar(1);
            unsetBatteryBar(2);
            unsetBatteryBar(3);
            break;
        case 1:
            setBatteryBar(0);
            setBatteryBar(1);
            unsetBatteryBar(2);
            unsetBatteryBar(3);
            break;
        case 2:
            setBatteryBar(0);
            setBatteryBar(1);
            setBatteryBar(2);
            unsetBatteryBar(3);
            break;
        case 3:
            setBatteryBar(0);
            setBatteryBar(1);
            setBatteryBar(2);
            setBatteryBar(3);
            break;
        default:
            break;
    }
}
