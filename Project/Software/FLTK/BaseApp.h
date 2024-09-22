#ifndef BASEAPP_H
#define BASEAPP_H

#include <cstdint>
#include <string>
#include "Application.h"

class BaseApp : public Application
{
    public:
        BaseApp(System* system);
        ~BaseApp() = default;

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
    
    private:
        const DisplayItem baseItem;
        DisplayItem actionItem;
        uint peroidCounter;
        
        void collectItems();
};


#endif
