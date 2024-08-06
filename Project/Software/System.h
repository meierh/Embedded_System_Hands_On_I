#ifndef SYSTEM_H
#define SYSTEM_H
#include <memory>
#include "Application.h"

class System
{
    public:
        virtual void work();
        
        //System -> Application
        virtual void button1Press();
        virtual void button2Press();
        virtual void button3Press();
        virtual void button4Press();
        enum Direction {Clockwise=1,Counterclockwise=-1};
        virtual void rotate(Direction dir);
        
        //Application -> System
        
    protected:
        std::unique_ptr<Application> app;
};
#endif
