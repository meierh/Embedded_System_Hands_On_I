#ifndef EGGTIMER_H
#define EGGTIMER_H
#include "EggTimerBase.h"
#include <cstdint>
#include <string>

class EggTimer : public EggTimerBase
{
    public:
        EggTimer(System* system);
        /**
         *  Internal action
         */
        void work() override;
                           
    private:
        enum EggTimerStatus{SetMin,SetSec,Run,End,Base};
        EggTimerStatus status = End;
        
        void collectItems() override;
                
        DisplayItem timeMinUnderline;
        DisplayItem timeSecUnderline;
};


#endif
