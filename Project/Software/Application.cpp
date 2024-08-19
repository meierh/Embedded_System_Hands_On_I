#include "System.h"
#include "Application.h"

Application::Application
(
    System* system
):
modeText(10,2,10,"Mode",255),
modeStatus(10,30,10,"",128),
batteryStatus(10,105,10,"73%",255),
time(122,100,10,"00:00",255),
system(system)
{
    updateClock();
    collectItems();
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

void Application::collectItems()
{
    displayImage.clear();
    displayImage.push_back(modeText);
    displayImage.push_back(modeStatus);
    displayImage.push_back(batteryStatus);
    displayImage.push_back(time);
}

void Application::updateClock()
{
    std::string clock = system->getSystemTime();
    std::string hoursMinutes = clock.substr(11,5);
    time.characters = hoursMinutes;
}
