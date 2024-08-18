#ifndef SYSTEM_H
#define SYSTEM_H
#include <memory>
#include "DisplayItem.h"
#include "HardwareEmulator.h"
#include "BaseApp.h"
//#include "SmartEgg.h"

class System
{
    public:
        System();
        ~System();
        
        virtual void work();
        
        //System -> Application
        void buttonModePress();
        void buttonLeftPress();
        void buttonCenterPress();
        void buttonRightPress();
        enum Direction {Clockwise=1,Counterclockwise=-1};
        void rotate(Direction dir);
        void periodElapsed();
        
        //Application -> System
        virtual void displayImage(std::vector<DisplayItem> image)=0;
        
    protected:
        enum App{BaseApp,SmartEgg,Empty};
        App current = Empty;
        Application* app = nullptr;
};
#endif
