#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <cstdint>
#include <string>
#include "Application.h"

class Stopwatch : public Application
{
    public:
        Stopwatch(System* system);
        ~Stopwatch() = default;

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
    
    private:
        uint periodCounter;
        uint seconds;
        uint intermediateSeconds;
        
        void writeSeconds(uint seconds);
        void writeIntermedSeconds(uint intermedSeconds);
        
        enum StopwatchStatus{RunWithIntermed,Run,End};
        StopwatchStatus status = End;
        
        DisplayItem timeHour;
        DisplayItem timeSeparator;
        DisplayItem timeMin;
        DisplayItem timeSec;
        
        DisplayItem interTimeHour;
        DisplayItem interTimeSeparator;
        DisplayItem interTimeMin;
        DisplayItem interTimeSec;
        
        void collectItems();
};


#endif
