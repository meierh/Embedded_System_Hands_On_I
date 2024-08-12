#ifndef APPLICATION_H
#define APPLICATION_H

#include <cstdint>
#include <queue>
#include <iostream>

class System;

class Application
{
    public:
        Application(System* sys):sys(sys){}
        virtual ~Application() = default;

        /**
         *  Internal action
         */
        virtual void work();
        
        /**
         *  Input action
         */
        virtual void onButton1Click();
        virtual void onButton2Click();
        virtual void onButton3Click();
        virtual void onButton4Click();
        virtual void onRotationClick(std::int8_t dir); 
        enum Action {Btn1Click=0,Btn2Click,Btn3Click,Btn4Click,RotateClock,RotateAntiClock};
        std::queue<Action> inputActions;
        
        /**
         *  Output action
         */
        virtual void speakerCommand();
        virtual void displayCommand();
        
    private:
        System* sys;
};

class SmartEgg : public Application
{
};

class Timer : public Application
{
};


#endif
