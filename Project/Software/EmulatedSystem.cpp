#include "EmulatedSystem.h"
#include <iostream>

void EmulatedSystem::work()
{
    if(app)
        app->work();
}

void EmulatedSystem::button1Press()
{
    if(app)
        app->onButton1Click();
}

void EmulatedSystem::button2Press()
{
    if(app)
        app->onButton2Click();
}

void EmulatedSystem::button3Press()
{
    if(app)
        app->onButton3Click();
}

void EmulatedSystem::button4Press()
{
    if(app)
        app->onButton4Click();
}

void EmulatedSystem::rotate(Direction dir)
{
    if(app)
        app->onRotationClick(static_cast<std::int8_t>(dir));
}

void EmulatedSystem::displayString
(
    std::tuple<std::uint8_t,std::uint8_t,System::Font,std::string> string
)
{
    hardware->displayString(string);
}

