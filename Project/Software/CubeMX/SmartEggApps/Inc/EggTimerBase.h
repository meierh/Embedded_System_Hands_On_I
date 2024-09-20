#ifndef EGGTIMERBASE_H
#define EGGTIMERBASE_H
#include "Application.h"
#include <cstdint>
#include <string>

class EggTimerBase : public Application
{
    public:
        EggTimerBase(System* system);
        /**
         *  Internal action
         */
        virtual void work()=0;
        
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
        std::pair<uint,uint> secondsToMinSecs(uint remainingSeconds);
        uint secondsToMinSecs(std::pair<uint,uint> minSec);
        uint remainingSeconds = 0;
        uint peroidCounter;
        
        DisplayItem minText;
        DisplayItem secText;

        DisplayItem timeMin;
        DisplayItem timeSeparator;
        DisplayItem timeSec;
        
        DisplayItem eggText;
        
        uint setMinutes;
        uint setSeconds;
        
        void writeMinutes(uint minutes);
        void writeSeconds(uint seconds);

        void collectItems() override;
};


#endif
