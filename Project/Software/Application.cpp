#include "System.h"
#include "Application.h"

Application::Application
(
    System* system
):
system(system)
{
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
