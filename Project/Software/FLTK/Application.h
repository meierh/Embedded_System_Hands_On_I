#ifndef APPLICATION_H
#define APPLICATION_H

#include <cstdint>
#include <queue>
#include <string>
#include <chrono>
#include <tuple>
#include <functional>
#include "DisplayItem.h"
#include "DateTime.h"

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
        System* system = nullptr;
    public:
        virtual void speakerCommand()=0;
        virtual void displayCommand()=0;
        
    protected:
        void displayCommand(const std::vector<DisplayItem>& items);
        std::vector<DisplayItem> displayImage;
        virtual void collectItems();
        
        const DisplayItem modeText;
        DisplayItem modeStatus;
        
        std::array<std::array<DisplayItem,3>,4> batteryStatus;
        void setBatteryBar(uint8_t barInd);
        void unsetBatteryBar(uint8_t barInd);
                
        DisplayItem time;
        
        DisplayItem buttonLine;
        DisplayItem leftCenterButtonLine;
        DisplayItem centerRightButtonLine;
        DisplayItem leftButtonLabel;
        DisplayItem centerButtonLabel;
        DisplayItem rightButtonLabel;
        
        DisplayItem timesUpLabel;
        virtual void setTimesUp();
        virtual void unsetTimesUp();
        
        DateTime currTime;
        virtual void updateClock();
        virtual void updateBattery();
};

#endif
