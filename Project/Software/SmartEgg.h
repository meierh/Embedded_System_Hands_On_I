#ifndef SMARTEGG_H
#define SMARTEGG_H
#include "Application.h"
#include <cstdint>
#include <string>

class SmartEgg : public Application
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
        
    private:
        uint peroidCounter;
        
        void collectItems();
        
        DisplayItem minText;
        DisplayItem secText;

        DisplayItem timeMin;
        DisplayItem timeSeparator;
        DisplayItem timeSec;
        
        DisplayItem eggText;
        DisplayItem eggStatus;
        
        enum SmartEggStatus{SetMin,SetSec,Run,End};
        SmartEggStatus status = End;
        uint remainingSeconds = 0;
        std::pair<uint,uint> secondsToMinSecs(uint remainingSeconds);
        uint secondsToMinSecs(std::pair<uint,uint> minSec);
        
        uint setMinutes;
        uint setSeconds;
        
        void writeMinutes(uint minutes);
        void writeSeconds(uint seconds);
};


#endif
