#ifndef SYSTEM_H
#define SYSTEM_H

#include "DisplayItem.h"
#include "HardwareEmulator.h"
#include "BaseApp.h"
#include "EggTimer.h"
#include "SmartEgg.h"
#include "AlarmClock.h"
#include "Stopwatch.h"
#include "Snake.h"
#include "SpaceInvaders.h"

class System
{
    public:
        System();
        ~System();
        
        virtual void work();
        
        //System -> Application
        void buttonModePress();
        void buttonLeftPress();
        void buttonCenterPress();
        void buttonRightPress();
        enum Direction {Clockwise=1,Counterclockwise=-1};
        void rotate(Direction dir);
        void periodElapsed();
        
        //Application -> System
        virtual void displayImage(std::vector<DisplayItem> image)=0;
        virtual std::string getSystemTime()=0;
        
    protected:
        enum App{BaseApp,SmartEgg,EggTimer,AlarmClock,Stopwatch,Snake,SpaceInvaders,Empty};
        App current = Empty;
        Application* app = nullptr;
};
#endif
