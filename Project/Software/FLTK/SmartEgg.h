#ifndef SMARTEGG_H
#define SMARTEGG_H
#include "EggTimer.h"
#include <cstdint>
#include <string>
#include <array>
#include <cmath>

constexpr double pi = 3.14159265358979323846;

class SmartEgg : public EggTimerBase
{
    public:
        SmartEgg(System* system);
        /**
         *  Internal action
         */
        void work() override;
        
        /**
         *  Input action
         */
        void onPeriod() override;
        
        /**
         *  Output action
         */
        void speakerCommand() override;
        void displayCommand() override;
        
    protected:        
        void setTimesUp() override;
        void unsetTimesUp() override;
        
    private:
        uint peroidCounter;
        uint computePerfectEggTime();
        float computeBoilingTemperature();
        
        void collectItems();
        
        DisplayItem eggText;
        DisplayItem eggSize;
        DisplayItem eggIniTemp;
        DisplayItem eggPressure;
        DisplayItem eggEndTemp;
        
        enum SmartEggStatus{SetSize,SetIniTemp,SetPressure,SetEndTemp,Run,Alarm,End};
        SmartEggStatus status = End;
        
        std::array<std::string,4> sizes = {"S","M","L","XL"};
        std::array<double,4> sizesWeight = {48,58,68,78};
        int sizeInd = 1;
        
        int lowerBoundIniTemp=0;
        int upperBoundIniTemp=40;
        int setIniTemp = 7;
        
        uint lowerBoundPressure=500;
        uint upperBoundPressure=9999;
        uint setPressure = 1013;
        
        int lowerBoundEndTemp=30;
        int upperBoundEndTemp=99;
        int setEndTemp = 72;
        
        void writeSize(std::string size);
        void writeIniTemp(int temp);
        void writePressure(uint pressure);
        void writeEndTemp(int temp);
};


#endif
