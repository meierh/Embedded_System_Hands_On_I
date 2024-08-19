#include "SmartEgg.h"
#include "System.h"

SmartEgg::SmartEgg
(
    System* system
):
Application(system),
peroidCounter(0)
{
    modeStatus = DisplayItem(10,30,10,"SmartEgg",128);
    
    minText = DisplayItem(40,15,20,"Min",255);
    secText = DisplayItem(40,70,20,"Sec",255);
    timeMin = DisplayItem(80,5,45,"00",255);
    timeSeparator = DisplayItem(80,55,45,":",255);
    timeSec = DisplayItem(80,70,45,"00",255);
    
    eggText = DisplayItem(110,2,20,"Egg",255);
    eggStatus = DisplayItem(125,2,8,"M, 7°C, 123m, 72°C",128);
        
    displayCommand();
    std::cout<<"Setup SmartEgg"<<std::endl;
    updateClock();
}

void SmartEgg::work()
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
                }
                break;
            }
            case BtnCenterClick:
            {
                if(status==SetMin || status==SetSec)
                {
                    remainingSeconds = secondsToMinSecs({setMinutes,setSeconds});
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
                        setMinutes = 60;
                    else
                        setMinutes--;
                    
                    writeMinutes(setMinutes);
                }
                else if(status==SetSec)
                {
                    if(setSeconds<1)
                        setSeconds = 60;
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
                        writeMinutes(secMins.first);
                        writeSeconds(secMins.second);
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

void SmartEgg::onPeriod()
{
    peroidCounter++;
    if(peroidCounter>=100)
    {
        peroidCounter=0;
        inputActions.push(Action::OnePeriod);
    }
}

void SmartEgg::displayCommand()
{
    updateClock();
    collectItems();
    displayCommand(displayImage);
}

void SmartEgg::displayCommand(std::vector<DisplayItem> items)
{
    system->displayImage(displayImage);
}

void SmartEgg::speakerCommand()
{
}

void SmartEgg::collectItems()
{
    displayImage.clear();
    
    Application::collectItems();
    
    displayImage.push_back(modeStatus);
    
    displayImage.push_back(minText);
    displayImage.push_back(secText);
    
    displayImage.push_back(timeMin);
    displayImage.push_back(timeSeparator);
    displayImage.push_back(timeSec);
    
    displayImage.push_back(eggText);
    displayImage.push_back(eggStatus);
}

std::pair<uint,uint> SmartEgg::secondsToMinSecs
(
    uint totalSeconds
)
{
    uint minutes = totalSeconds / 60;
    uint seconds = totalSeconds - (minutes*60);
    return {minutes,seconds};
}

uint SmartEgg::secondsToMinSecs
(
    std::pair<uint,uint> minSecs
)
{
    return minSecs.first*60+minSecs.second;
}

void SmartEgg::writeMinutes(uint minutes)
{
    if(minutes<10)
        timeMin.characters = "0"+std::to_string(minutes);
    else
        timeMin.characters = std::to_string(minutes);
}

void SmartEgg::writeSeconds(uint seconds)
{
    if(seconds<10)
        timeSec.characters = "0"+std::to_string(seconds);
    else
        timeSec.characters = std::to_string(seconds);
}
