#include "Application.h"

void Application::work()
{
    if(!inputActions.empty())
    {
        Action action = inputActions.front();
        inputActions.pop();
        switch(action)
        {
            case Btn1Click:
            case Btn2Click:
            case Btn3Click:
            case Btn4Click:
            case RotateClock:
            case RotateAntiClock:
            default:
                break;
        }
    }
}

void Application::onButton1Click()
{
    inputActions.push(Action::Btn1Click);
}

void Application::onButton2Click()
{
    inputActions.push(Action::Btn2Click);
}

void Application::onButton3Click()
{
    inputActions.push(Action::Btn3Click);
}

void Application::onButton4Click()
{
    inputActions.push(Action::Btn4Click);
}

void Application::onRotationClick(std::int8_t dir)
{
    if(dir==1)
        inputActions.push(Action::RotateClock);
    else
        inputActions.push(Action::RotateAntiClock);
}

void Application::speakerCommand(){}

void Application::displayCommand(std::string str)
{
    
}
