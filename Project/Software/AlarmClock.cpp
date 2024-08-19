#include "AlarmClock.h"
#include "System.h"

AlarmClock::AlarmClock
(
    System* system
):
SmartEgg(system,SmartEgg::SmartEggStatus::Base)
{
    modeStatus.characters = "AlarmClock";
    
    minText = DisplayItem(25,15,10,"Min",255);
    secText = DisplayItem(25,70,10,"Sec",255);
    
    timeHourClock = DisplayItem(55,15,35,"00",255);
    timeSeparatorClock = DisplayItem(55,55,35,":",255);
    timeMinClock = DisplayItem(55,70,35,"00",255);
    
    timeHourAlarm = DisplayItem(95,15,35,"00",255);
    timeSeparatorAlarm = DisplayItem(95,55,35,":",255);
    timeMinAlarm = DisplayItem(95,70,35,"00",255);
    
    remainTimeText = DisplayItem(115,2,10,"Remaining",255);
    remaingTimeHour = DisplayItem(125,12,8,"00",128);
    remaingTimeHourSuffix = DisplayItem(125,20,8,"h",128);
    remaingTimeMinutes = DisplayItem(125,30,8,"00",128);
    remaingTimeMinutesSuffix = DisplayItem(125,38,8,"m",128);
    
    timeMinUnderline = DisplayItem(96,16,96,48,128);
    timeMinUnderline.setType(DisplayItem::ItemType::Empty);
    timeSecUnderline = DisplayItem(96,72,96,104,128);
    timeSecUnderline.setType(DisplayItem::ItemType::Empty);
        
    displayCommand();
    std::cout<<"Setup AlarmClock"<<std::endl;
    updateClock();
}

void AlarmClock::work()
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
                    status = SetHour;
                    std::pair<uint,uint> minSecs = minutesToHourMins(remainingMinutes);
                    alarmHours = minSecs.first;
                    alarmMinutes = minSecs.second;
                    timeMinUnderline.setType(DisplayItem::ItemType::Line);
                }
                break;
            }
            case BtnCenterClick:
            {
                if(status==SetHour || status==SetMin)
                {
                    remainingSeconds = minutesToHourMins({alarmHours,alarmMinutes});
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
                    status = SetMin;
                    std::pair<uint,uint> minSecs = secondsToMinSecs(remainingSeconds);
                    alarmHours = minSecs.first;
                    alarmMinutes = minSecs.second;
                    timeSecUnderline.setType(DisplayItem::ItemType::Line);
                }
                break;
            }
            case RotateClock:
            {
                if(status==SetHour)
                {
                    alarmHours = (alarmHours+1)%24;
                    writeHours(alarmHours);
                }
                else if(status==SetMin)
                {
                    alarmMinutes = (alarmMinutes+1)%60;
                    writeMinutes(alarmMinutes);
                }
                break;
            }
            case RotateAntiClock:
            {
                if(status==SetHour)
                {
                    if(alarmHours<1)
                        alarmHours = 23;
                    else
                        alarmHours--;
                    
                    writeHours(alarmHours);
                }
                else if(status==SetMin)
                {
                    if(alarmMinutes<1)
                        alarmMinutes = 59;
                    else
                        alarmMinutes--;
                    
                    writeMinutes(alarmMinutes);
                }
                break;
            }
            case OnePeriod:
            {
                if(status==Run)
                {
                    std::cout<<"Run"<<std::endl;
                    if(remainingMinutes>0)
                    {
                        remainingMinutes--;
                        std::pair<uint,uint> hourMins = minutesToHourMins(remainingMinutes);
                        writeRemainingHours(hourMins.first);
                        writeRemainingMinutes(hourMins.second);
                        std::cout<<"Rem:"<<remainingMinutes<<std::endl;
                        displayCommand();
                    }
                    else
                    {
                        std::cout<<"ALARM!!!!"<<std::endl;
                        status = End;
                    }
                }
                else if(status==SetHour || status==SetMin)
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

void AlarmClock::onPeriod()
{
    peroidCounter++;
    if(peroidCounter>=60*100)
    {
        peroidCounter=0;
        inputActions.push(Action::OnePeriod);
    }
}

void AlarmClock::displayCommand()
{
    updateClock();
    collectItems();
    displayCommand(displayImage);
}

void AlarmClock::displayCommand(std::vector<DisplayItem> items)
{
    
    if(system!=nullptr)
        system->displayImage(displayImage);
}

void AlarmClock::updateClock()
{
    Application::updateClock();
    timeHourClock.characters = time.characters.substr(0,2);
    timeMinClock.characters = time.characters.substr(3,2);
}

std::pair<uint,uint> AlarmClock::minutesToHourMins
(
    uint remainingMinutes
)
{
    return secondsToMinSecs(remainingMinutes);
}

uint AlarmClock::minutesToHourMins(std::pair<uint,uint> hourMin)
{
    return secondsToMinSecs(hourMin);
}

void AlarmClock::speakerCommand()
{
}

void AlarmClock::collectItems()
{
    displayImage.clear();
    
    Application::collectItems();
        
    displayImage.push_back(minText);
    displayImage.push_back(secText);
    
    displayImage.push_back(timeHourClock);
    displayImage.push_back(timeSeparatorClock);
    displayImage.push_back(timeMinClock);
    
    displayImage.push_back(timeHourAlarm);
    displayImage.push_back(timeSeparatorAlarm);
    displayImage.push_back(timeMinAlarm);
    
    displayImage.push_back(remainTimeText);
    displayImage.push_back(remaingTimeHour);
    displayImage.push_back(remaingTimeHourSuffix);
    displayImage.push_back(remaingTimeMinutes);
    displayImage.push_back(remaingTimeMinutesSuffix);
    
    displayImage.push_back(timeMinUnderline);
    displayImage.push_back(timeSecUnderline);
}

void AlarmClock::writeHours(uint hours)
{
    if(hours<10)
        timeHourAlarm.characters = "0"+std::to_string(hours);
    else
        timeHourAlarm.characters = std::to_string(hours);
}

void AlarmClock::writeMinutes(uint minutes)
{
    if(minutes<10)
        timeMinAlarm.characters = "0"+std::to_string(minutes);
    else
        timeMinAlarm.characters = std::to_string(minutes);
}

void AlarmClock::writeRemainingHours(uint hours)
{
    if(hours<10)
        remaingTimeHour.characters = "0"+std::to_string(hours);
    else
        remaingTimeHour.characters = std::to_string(hours);
}

void AlarmClock::writeRemainingMinutes(uint minutes)
{    
    if(minutes<10)
        remaingTimeMinutes.characters = "0"+std::to_string(minutes);
    else
        remaingTimeMinutes.characters = std::to_string(minutes);
}
