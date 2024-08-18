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
    std::cout<<"System::buttonModePress"<<std::endl;
    if(app==nullptr)
        app = new class BaseApp(this);
    else
    {
        delete app;
        app = new class BaseApp(this);
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
    else
        std::cout<<"Error: System::periodElapsed nullptr"<<std::endl;
}
