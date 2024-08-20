#ifndef SNAKE_H
#define SNAKE_H

#include <cstdint>
#include <string>
#include "Application.h"

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
        const DisplayItem baseItem;
        DisplayItem actionItem;
        uint peroidCounter;
        
        void collectItems();
};


#endif
