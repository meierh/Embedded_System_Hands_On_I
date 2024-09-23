#ifndef TESTIMAGE_H
#define TESTIMAGE_H

#include <cstdint>
#include <string>
#include "Application.h"

class TestImage : public Application
{
    public:
        TestImage(System* system);
        ~TestImage() = default;

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
        
        void collectItems();
};


#endif
