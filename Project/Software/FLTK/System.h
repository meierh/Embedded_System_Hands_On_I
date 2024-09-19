#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include "DisplayItem.h"
#include "HardwareEmulator.h"
#include "BaseApp.h"
#include "EggTimer.h"
#include "SmartEgg.h"
#include "AlarmClock.h"
#include "Stopwatch.h"
#include "SetClock.h"
#include "TestImage.h"
#include "DateTime.h"

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
        virtual DateTime getSystemTime()=0;
        virtual int8_t getSeconds() =0;
        virtual void setSystemTime(DateTime newTime)=0;
        // virtual void playSound(uint8_t soundFile, uint8_t soundFolder)=0;
        
    protected:
        enum App{BaseApp,SmartEgg,EggTimer,AlarmClock,Stopwatch,SetClock,TestImage,Empty};
        App current = Empty;
        Application* app = nullptr;
};
#endif
