#ifndef ALARMCLOCK_H
#define ALARMCLOCK_H
#include "SmartEgg.h"
#include <cstdint>
#include <string>

class AlarmClock : public SmartEgg
{
    public:
        AlarmClock(System* system);
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
        void updateClock() override;
        
        std::pair<uint,uint> minutesToHourMins(uint remainingMinutes);
        uint minutesToHourMins(std::pair<uint,uint> minSec);
        
    private:
        uint peroidCounter;
        
        void collectItems();
        
        DisplayItem minText;
        DisplayItem secText;

        DisplayItem timeHourClock;
        DisplayItem timeSeparatorClock;
        DisplayItem timeMinClock;
        
        DisplayItem timeHourAlarm;
        DisplayItem timeSeparatorAlarm;
        DisplayItem timeMinAlarm;
        
        DisplayItem remainTimeText;
        DisplayItem remaingTimeHour;
        DisplayItem remaingTimeHourSuffix;
        DisplayItem remaingTimeMinutes;
        DisplayItem remaingTimeMinutesSuffix;
        
        DisplayItem timeMinUnderline;
        DisplayItem timeSecUnderline;
        
        enum AlarmClockStatus{SetHour,SetMin,Run,End,Base};
        AlarmClockStatus status = End;
        uint remainingMinutes = 0;
        
        uint alarmHours;
        uint alarmMinutes;
        
        void writeHours(uint hours);
        void writeMinutes(uint minutes);
        
        void writeRemainingHours(uint hours);
        void writeRemainingMinutes(uint minutes);
        
};


#endif
