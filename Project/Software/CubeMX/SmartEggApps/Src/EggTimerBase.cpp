#include "EggTimerBase.h"
#include "System.h"

EggTimerBase::EggTimerBase
(
    System* system
):
Application(system),
peroidCounter(0),
setMinutes(0),
setSeconds(0)
{   
    minText = DisplayItem(40,15,DisplayItem::Font16,"Min",255);
    secText = DisplayItem(40,70,DisplayItem::Font16,"Sec",255);
    timeMin = DisplayItem(70,20,DisplayItem::Font24,"00",255);
    timeSeparator = DisplayItem(70,57,DisplayItem::Font24,":",255);
    timeSec = DisplayItem(70,75,DisplayItem::Font24,"00",255);
    
    eggText = DisplayItem(110,2,DisplayItem::Font16,"Egg",255);
}

void EggTimerBase::onPeriod()
{
    inputActions.push(Action::OnePeriod);
    /*peroidCounter++;
    if(peroidCounter>=100)
    {
        peroidCounter=0;
        inputActions.push(Action::OnePeriod);
    }*/
}

void EggTimerBase::displayCommand()
{
    updateClock();
    writeMinutes(setMinutes);
    writeSeconds(setSeconds);
    collectItems();
    displayCommand(displayImage);
    
    for(DisplayItem item : displayImage)
        std::cout<<" Disp:"<<item.to_string()<<std::endl;
}

void EggTimerBase::displayCommand(std::vector<DisplayItem> items)
{
    if(system!=nullptr)
        system->displayImage(displayImage);
}

void EggTimerBase::speakerCommand()
{
}

void EggTimerBase::collectItems()
{
    displayImage.clear();
    
    Application::collectItems();
        
    displayImage.push_back(minText);
    displayImage.push_back(secText);
    
    displayImage.push_back(timeMin);
    displayImage.push_back(timeSeparator);
    displayImage.push_back(timeSec);
    
    displayImage.push_back(eggText);
}

std::pair<uint,uint> EggTimerBase::secondsToMinSecs
(
    uint totalSeconds
)
{
    uint minutes = totalSeconds / 60;
    uint seconds = totalSeconds - (minutes*60);
    return {minutes,seconds};
}

uint EggTimerBase::secondsToMinSecs
(
    std::pair<uint,uint> minSecs
)
{
    return minSecs.first*60+minSecs.second;
}

void EggTimerBase::writeMinutes(uint minutes)
{
    if(minutes<10)
        timeMin.characters = "0"+std::to_string(minutes);
    else
        timeMin.characters = std::to_string(minutes);
}

void EggTimerBase::writeSeconds(uint seconds)
{
    if(seconds<10)
        timeSec.characters = "0"+std::to_string(seconds);
    else
        timeSec.characters = std::to_string(seconds);
}
