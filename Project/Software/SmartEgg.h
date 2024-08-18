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
        
    private:
        const DisplayItem baseItem;
        std::vector<DisplayItem> displayImage;
        uint peroidCounter;
};


#endif
