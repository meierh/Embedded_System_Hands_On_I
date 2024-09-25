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
        blocked = true;
        app->close();
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
                app = new class MP3Player(this);
                current = MP3Player;
                break;
            case MP3Player:
                app = new class BaseApp(this);
                current = BaseApp;
                break;
            // TestImage: removed from selection

            default:
                break;
        }
        blocked = false;
    }
    periodElapsed();
}

void System::buttonLeftPress()
{
    if(app!=nullptr && !blocked)
        app->onButtonLeftClick();
    else
        std::cout<<"Error: System::buttonLeftPress nullptr"<<std::endl;
}

void System::buttonCenterPress()
{
    if(app!=nullptr && !blocked)
        app->onButtonCenterClick();
    else
        std::cout<<"Error: System::buttonCenterPress nullptr"<<std::endl;
}

void System::buttonRightPress()
{
    if(app!=nullptr && !blocked)
        app->onButtonRightClick();
    else
        std::cout<<"Error: System::buttonRightPress nullptr"<<std::endl;
}

void System::rotate(Direction dir)
{
    if(app!=nullptr && !blocked)
        app->onRotationClick(static_cast<std::int8_t>(dir));
    else
        std::cout<<"Error: System::rotate nullptr"<<std::endl;
}

void System::periodElapsed()
{
    if(app!=nullptr && !blocked)
        app->onPeriod();
}

bool System::playerActive()
{
    return mp3PlayerOn;
}

void System::playerTurnOn(bool initializePlayer)
{
    mp3PlayerOn = true;
}

void System::playerTurnOff()
{
    mp3PlayerOn = false;
}
