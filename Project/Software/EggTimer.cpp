#include "EggTimer.h"
#include "System.h"

EggTimer::EggTimer
(
    System* system
):
EggTimerBase(system)
{
    modeStatus.characters = "EggTimer";
    
    timeMinUnderline = DisplayItem(81,8,81,52,128);
    timeMinUnderline.setType(DisplayItem::ItemType::Empty);
    timeSecUnderline = DisplayItem(81,72,81,116,128);
    timeSecUnderline.setType(DisplayItem::ItemType::Empty);
        
    displayCommand();
    std::cout<<"Setup EggTimer"<<std::endl;
    updateClock();
}

void EggTimer::work()
{
    if(!inputActions.empty())
    {
        Action action = inputActions.front();
        inputActions.pop();
        switch(action)
        {
            case BtnLeftClick:
            {
                if(status==Run || status==End)
                {
                    status = SetMin;
                    std::pair<uint,uint> minSecs = secondsToMinSecs(remainingSeconds);
                    setMinutes = minSecs.first;
                    setSeconds = minSecs.second;
                    timeMinUnderline.setType(DisplayItem::ItemType::Line);
                }
                else if(status==SetSec)
                {
                    timeMinUnderline.setType(DisplayItem::ItemType::Line);
                    timeSecUnderline.setType(DisplayItem::ItemType::Empty);
                    status = SetMin;
                }
                break;
            }
            case BtnCenterClick:
            {
                if(status==SetMin || status==SetSec)
                {
                    remainingSeconds = secondsToMinSecs({setMinutes,setSeconds});
                    timeMinUnderline.setType(DisplayItem::ItemType::Empty);
                    timeSecUnderline.setType(DisplayItem::ItemType::Empty);
                    status = Run;
                }
                break;
            }
            case BtnRightClick:
            {
                if(status==Run || status==End)
                {
                    status = SetSec;
                    std::pair<uint,uint> minSecs = secondsToMinSecs(remainingSeconds);
                    setMinutes = minSecs.first;
                    setSeconds = minSecs.second;
                    timeSecUnderline.setType(DisplayItem::ItemType::Line);
                }
                else if(status==SetMin)
                {
                    timeMinUnderline.setType(DisplayItem::ItemType::Empty);
                    timeSecUnderline.setType(DisplayItem::ItemType::Line);
                    status = SetSec;
                }
                break;
            }
            case RotateClock:
            {
                if(status==SetMin)
                {
                    setMinutes = (setMinutes+1)%60;
                    writeMinutes(setMinutes);
                }
                else if(status==SetSec)
                {
                    setSeconds = (setSeconds+1)%60;
                    writeSeconds(setSeconds);
                }
                break;
            }
            case RotateAntiClock:
            {
                if(status==SetMin)
                {
                    if(setMinutes<1)
                        setMinutes = 59;
                    else
                        setMinutes--;
                    
                    writeMinutes(setMinutes);
                }
                else if(status==SetSec)
                {
                    if(setSeconds<1)
                        setSeconds = 59;
                    else
                        setSeconds--;
                    
                    writeSeconds(setSeconds);
                }
                break;
            }
            case OnePeriod:
            {
                if(status==Run)
                {
                    std::cout<<"Run"<<std::endl;
                    if(remainingSeconds>0)
                    {
                        remainingSeconds--;
                        std::pair<uint,uint> secMins = secondsToMinSecs(remainingSeconds);
                        setMinutes = secMins.first;
                        setSeconds = secMins.second;
                        std::cout<<"Rem:"<<remainingSeconds<<std::endl;
                        displayCommand();
                    }
                    else
                        status = End;
                }
                else if(status==SetMin || status==SetSec)
                {
                    displayCommand();
                }
                break;
            }
            default:
                break;
        }
        displayCommand();
    }
}

void EggTimer::collectItems()
{
    displayImage.clear();
    
    EggTimerBase::collectItems();
        
    displayImage.push_back(timeMinUnderline);
    displayImage.push_back(timeSecUnderline);
}
