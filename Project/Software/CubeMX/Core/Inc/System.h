#ifndef SYSTEM_H
#define SYSTEM_H

#include "DisplayItem.h"
#include "DateTime.h"

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
        virtual void buttonModePress();
        virtual void buttonLeftPress();
        virtual void buttonCenterPress();
        virtual void buttonRightPress();
        enum Direction {Clockwise=1,Counterclockwise=-1};
        virtual void rotate(Direction dir);
        virtual void periodElapsed();
        
        //Application -> System
        virtual void displayImage(std::vector<DisplayItem> image)=0;
        virtual DateTime getSystemTime()=0;
        virtual int8_t getSeconds() =0;
        virtual void setSystemTime(DateTime newTime)=0;
        // virtual void playSound(uint8_t soundFile, uint8_t soundFolder)=0;

        
    protected:
        enum App{BaseApp,SmartEgg,EggTimer,AlarmClock,Stopwatch,Snake,SpaceInvaders,Empty};
        App current = Empty;
        Application* app = nullptr;
};
#endif
