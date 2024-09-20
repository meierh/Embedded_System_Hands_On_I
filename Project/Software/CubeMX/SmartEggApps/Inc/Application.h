#ifndef APPLICATION_H
#define APPLICATION_H

#include <cstdint>
#include <queue>
#include <string>
#include <chrono>
#include "DisplayItem.h"

class System;

class Application
{
    public:
        Application(System* system);
        virtual ~Application() = default;

        /**
         *  Internal action
         */
        virtual void work()=0;

        void close();
                
        /**
         *  Input action
         */
        virtual void onButtonLeftClick();
        virtual void onButtonCenterClick();
        virtual void onButtonRightClick();
        virtual void onRotationClick(std::int8_t dir);
        virtual void onPeriod();
        enum Action {BtnLeftClick,BtnCenterClick,BtnRightClick,RotateClock,RotateAntiClock,OnePeriod};
    protected:
        std::queue<Action> inputActions;
        
        /**
         *  Output action
         */
    protected:
        System* system = nullptr;
    public:
        virtual void speakerCommand()=0;
        virtual void displayCommand()=0;
        
    protected:
        std::vector<DisplayItem> displayImage;
        virtual void collectItems();
        
        const DisplayItem modeText;
        DisplayItem modeStatus;
        
        DisplayItem batteryStatus;
                
        DisplayItem time;
        
        DisplayItem buttonLine;
        DisplayItem leftCenterButtonLine;
        DisplayItem centerRightButtonLine;
        DisplayItem leftButtonLabel;
        DisplayItem centerButtonLabel;
        DisplayItem rightButtonLabel;
        
        virtual void updateClock();
};

#endif
