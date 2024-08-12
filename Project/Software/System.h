#ifndef SYSTEM_H
#define SYSTEM_H
#include <memory>
#include <string>
#include "Application.h"


class System
{
    public:
        System();
        
        virtual void work();
        
        //System -> Application
        virtual void button1Press();
        virtual void button2Press();
        virtual void button3Press();
        virtual void button4Press();
        enum Direction {Clockwise=1,Counterclockwise=-1};
        virtual void rotate(Direction dir);
        
        //Application -> System
        enum Font {Font8,Font12,Font16,Font20,Font24};
        virtual void displayString(std::tuple<std::uint8_t,std::uint8_t,System::Font,std::string> string);
        
    protected:
        std::unique_ptr<Application> app;
};
#endif
