#ifndef SMARTEGG_H
#define SMARTEGG_H
#include "EggTimer.h"
#include <cstdint>
#include <string>
#include <array>

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
        void displayCommand(std::vector<DisplayItem> items);
        
    protected:        
        
    private:
        uint peroidCounter;
        uint computePerfectEggTime();
        
        void collectItems();
        
        DisplayItem eggText;
        DisplayItem eggSize;
        DisplayItem eggIniTemp;
        DisplayItem eggPressure;
        DisplayItem eggEndTemp;
        
        enum SmartEggStatus{SetSize,SetIniTemp,SetPressure,SetEndTemp,Run,End};
        SmartEggStatus status = End;
        
        std::array<std::string,4> sizes = {"S","M","L","XL"};
        int sizeInd = 1;
        
        int lowerBoundIniTemp=-9;
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
