#ifndef SETCLOCK_H
#define SETCLOCK_H

#include <cstdint>
#include <string>
#include "Application.h"

class SetClock : public Application
{
    public:
        SetClock(System* system);
        ~SetClock() = default;

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
        void displayCommand() override;
        void displayCommand(std::vector<DisplayItem> items);
    
    private:
        uint periodCounter;

        int year;
        int month;
        int day;
        int hour;
        int minute;
        int second;
        
        int changeIndex = 0;
        
        void writeDate();
        void setToCurrentTime();
        void restrictTime();
        void restrictDay();

        void highlight();
        void unhighlight();
        
        void print();
        
        enum SetClockStatus{SetTime,Entered};
        SetClockStatus status = Entered;
                
        DisplayItem clockHour;
        DisplayItem clockSeparator;
        DisplayItem clockMin;
        DisplayItem clockSec;
        
        DisplayItem clockDay;
        DisplayItem clockDaySeparator;
        DisplayItem clockMonth;
        DisplayItem clockMonthSeparator;
        DisplayItem clockYear;
        
        void collectItems();
};


#endif
