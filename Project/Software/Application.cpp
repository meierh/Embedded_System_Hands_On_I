#include "System.h"
#include "Application.h"

void Application::work()
{
    if(!inputActions.empty())
    {
        Action action = inputActions.front();
        inputActions.pop();
        std::tuple<std::uint8_t,std::uint8_t,System::Font,std::string> text;
        switch(action)
        {
            case Btn1Click:
                text = std::make_tuple(10,10,System::Font::Font8,"Btn1");
                break;
            case Btn2Click:
                text = std::make_tuple(20,20,System::Font::Font8,"Btn2");
                break;
            case Btn3Click:
                text = std::make_tuple(30,30,System::Font::Font8,"Btn3");
                break;
            case Btn4Click:
                text = std::make_tuple(40,40,System::Font::Font8,"Btn4");
                break;
            case RotateClock:
                text = std::make_tuple(50,50,System::Font::Font8,"RotateClock");
                break;
            case RotateAntiClock:
                text = std::make_tuple(60,60,System::Font::Font8,"RotateAntiClock");
                break;
            default:
                text = std::make_tuple(65,65,System::Font::Font8,"Empty");
                break;
        }
        sys->displayString(text);
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

void Application::displayCommand()
{
    
}
