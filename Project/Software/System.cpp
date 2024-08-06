#include "System.h"
#include <iostream>

void System::work()
{
    if(app)
        app->work();
}

void System::button1Press()
{
    if(app)
        app->onButton1Click();
}

void System::button2Press()
{
    if(app)
        app->onButton2Click();
}

void System::button3Press()
{
    if(app)
        app->onButton3Click();
}

void System::button4Press()
{
    if(app)
        app->onButton4Click();
}

void System::rotate(Direction dir)
{
    if(app)
        app->onRotationClick(static_cast<std::int8_t>(dir));
}
