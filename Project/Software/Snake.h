#ifndef SNAKE_H
#define SNAKE_H

#include <cstdint>
#include <string>
#include "Application.h"
#include <array>

class Snake : public Application
{
    public:
        Snake(System* system);
        ~Snake() = default;

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
        uint onPeriodCount;
        
        enum GameStatus{Initial,Play,End};
        GameStatus status = Initial;
       
        DisplayItem baseItem;
        DisplayItem endItem;
        
        DisplayItem bottomLine;
        DisplayItem topLine;
        
        int wSpacing = 4;
        std::array<DisplayItem,31> gridW;
        int hSpacing = 5;
        std::array<DisplayItem,18> gridH;

        uint peroidCounter;
                
        void collectItems() override;
};


#endif
