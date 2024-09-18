#include "System.h"

System::System()
{
    std::cout<<"Setup System"<<std::endl;
}

System::~System()
{
    std::cout<<"~System"<<std::endl;
    if(app!=nullptr)
        delete app;
    std::cout<<"~System done"<<std::endl;    
}

void System::work()
{
    if(app!=nullptr)
        app->work();
}

void System::buttonModePress()
{
    if(app==nullptr)
    {
        app = new class BaseApp(this);
        current = BaseApp;
    }
    else
    {
        delete app;
        switch(current)
        {
            case BaseApp:
                app = new class SmartEgg(this);
                current = SmartEgg;
                break;
            case SmartEgg:
                app = new class EggTimer(this);
                current = EggTimer;
                break;
            case EggTimer:
                app = new class AlarmClock(this);
                current = AlarmClock;
                break;
            case AlarmClock:
                app = new class Stopwatch(this);
                current = Stopwatch;
                break;
            case Stopwatch:
                app = new class SetClock(this);
                current = SetClock;
                break;
            case SetClock:
                app = new class BaseApp(this);
                current = BaseApp;
                break;
            default:
                break;
        }
    }
}

void System::buttonLeftPress()
{
    if(app!=nullptr)
        app->onButtonLeftClick();
    else
        std::cout<<"Error: System::buttonLeftPress nullptr"<<std::endl;
}

void System::buttonCenterPress()
{
    if(app!=nullptr)
        app->onButtonCenterClick();
    else
        std::cout<<"Error: System::buttonCenterPress nullptr"<<std::endl;
}

void System::buttonRightPress()
{
    if(app!=nullptr)
        app->onButtonRightClick();
    else
        std::cout<<"Error: System::buttonRightPress nullptr"<<std::endl;
}

void System::rotate(Direction dir)
{
    if(app!=nullptr)
        app->onRotationClick(static_cast<std::int8_t>(dir));
    else
        std::cout<<"Error: System::rotate nullptr"<<std::endl;
}

void System::periodElapsed()
{
    if(app!=nullptr)
        app->onPeriod();
}
