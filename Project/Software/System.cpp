#include "System.h"
#include <iostream>

System::System()
{
    app = std::make_unique<Application>(this);
}

void System::work()
{
}

void System::button1Press()
{
}

void System::button2Press()
{
}

void System::button3Press()
{
}

void System::button4Press()
{
}

void System::rotate(Direction dir)
{
}

void System::displayString
(
    std::tuple<std::uint8_t,std::uint8_t,System::Font,std::string> string
)
{
    
}
