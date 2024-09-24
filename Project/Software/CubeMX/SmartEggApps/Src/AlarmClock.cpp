#include "AlarmClock.h"
#include "System.h"

AlarmClock::AlarmClock
(
    System* system
):
Application(system),
alarmHours(0),
alarmMinutes(0)
{
    modeStatus.characters = "AlarmClock";
    
    minText = DisplayItem(35,15,DisplayItem::Font16,"Hour",255);
    secText = DisplayItem(35,75,DisplayItem::Font16,"Min",255);
    
    timeHourClock = DisplayItem(55,20,DisplayItem::Font24,"00",255);
    timeSeparatorClock = DisplayItem(55,57,DisplayItem::Font24,":",255);
    timeMinClock = DisplayItem(55,75,DisplayItem::Font24,"00",255);
    
    timeHourAlarm = DisplayItem(80,20,DisplayItem::Font24,"00",255);
    timeSeparatorAlarm = DisplayItem(80,57,DisplayItem::Font24,":",255);
    timeMinAlarm = DisplayItem(80,75,DisplayItem::Font24,"00",255);
    
    remainTimeText = DisplayItem(103,12,DisplayItem::Font8,"Remaining",255);
    remaingTimeHour = DisplayItem(113,12,DisplayItem::Font12,"00",128);
    remaingTimeHourSuffix = DisplayItem(113,27,DisplayItem::Font12,"h",128);
    remaingTimeMinutes = DisplayItem(113,35,DisplayItem::Font12,"00",128);
    remaingTimeMinutesSuffix = DisplayItem(113,50,DisplayItem::Font12,"m",128);
    
    timeMinUnderline = DisplayItem(91,20,91,50,128);
    timeMinUnderline.setType(DisplayItem::ItemType::Empty);
    timeSecUnderline = DisplayItem(91,76,91,106,128);
    timeSecUnderline.setType(DisplayItem::ItemType::Empty);
    
    timesUpLabel.characters = "Time's up";
    
    leftButtonLabel.characters = "Hour";
    leftButtonLabel.offsetW -= 5 ;
    centerButtonLabel.characters = "Enter";
    rightButtonLabel.characters = "Min";

    updateClock();
    displayCommand();
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
                    timeMinUnderline.setType(DisplayItem::ItemType::Line);
                }
                else if(status==SetMin)
                {
                    status = SetHour;
                    timeMinUnderline.setType(DisplayItem::ItemType::Line);
                    timeSecUnderline.setType(DisplayItem::ItemType::Empty);
                }
                else if(status==Alarm)
                {
                    unsetTimesUp();
                    status = End;
                }
                break;
            }
            case BtnCenterClick:
            {
                if(status==SetHour || status==SetMin)
                {
                    
                    std::cout<<"alarmHours:"<<alarmHours<<std::endl;
                    std::cout<<"alarmMinutes:"<<alarmMinutes<<std::endl;

                    uint remainingAlarmMinutes = minutesToHourMins({alarmHours,alarmMinutes});
                    uint clockHours = std::stoi(timeHourClock.characters);
                    uint clockMinutes = std::stoi(timeMinClock.characters);
                    uint timeMinutes = clockHours*60+clockMinutes;
                    
                    std::cout<<"timeHourClock.characters:"<<timeHourClock.characters<<std::endl;
                    std::cout<<"timeHourClock.characters:"<<timeMinClock.characters<<std::endl;
                    
                    std::cout<<"remainingAlarmMinutes:"<<remainingAlarmMinutes<<std::endl;
                    std::cout<<"timeMinutes:"<<timeMinutes<<std::endl;
                    if(remainingAlarmMinutes>=timeMinutes)
                        remainingMinutes = remainingAlarmMinutes-timeMinutes;
                    else
                        remainingMinutes = 24*60-(timeMinutes-remainingAlarmMinutes);
                    timeMinUnderline.setType(DisplayItem::ItemType::Empty);
                    timeSecUnderline.setType(DisplayItem::ItemType::Empty);
                    status = Run;
                }
                else if(status==Alarm)
                {
                    unsetTimesUp();
                    status = End;
                }
                break;
            }
            case BtnRightClick:
            {
                if(status==Run || status==End)
                {
                    status = SetMin;
                    timeSecUnderline.setType(DisplayItem::ItemType::Line);
                }
                else if(status==SetHour)
                {
                    status = SetMin;
                    timeMinUnderline.setType(DisplayItem::ItemType::Empty);
                    timeSecUnderline.setType(DisplayItem::ItemType::Line);
                }
                else if(status==Alarm)
                {
                    unsetTimesUp();
                    status = End;
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
                else if(status==Alarm)
                {
                    unsetTimesUp();
                    status = End;
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
                else if(status==Alarm)
                {
                    unsetTimesUp();
                    status = End;
                }
                break;
            }
            case OnePeriod:
            {
                if(status==Run)
                {
                    if(alarmHours==currTime.getHour() && alarmMinutes==currTime.getMinute())
                    {
                        setTimesUp();
                        status = Alarm;
                        remainingMinutes = 0;
                    }
                    else
                    {
                        uint alarmTimeMinsAfterFullHour = alarmMinutes;
                        uint currTimeMinsToFullHour = 60-currTime.getMinute();
                        uint alarmTimeHoursAfterMidnight = alarmHours;
                        uint currTimeHoursToMidnight = 24-currTime.getHour();
                                               
                        uint deltaHours;
                        if(alarmHours<currTime.getHour())
                            deltaHours = alarmTimeHoursAfterMidnight+currTimeHoursToMidnight-1;
                        else
                            deltaHours = alarmHours-currTime.getHour();

                        uint deltaMinutes;
                        if(alarmMinutes<currTime.getMinute())
                            deltaMinutes = alarmTimeMinsAfterFullHour+currTimeMinsToFullHour-1;
                        else
                            deltaMinutes = alarmMinutes-currTime.getMinute();
                        
                        remainingMinutes = deltaHours*60 + deltaMinutes;
                    }
                }
                break;
            }
            default:
                break;
        }
        displayCommand();
    }
}

void AlarmClock::setTimesUp()
{
    minText.setType(DisplayItem::Empty);
    secText.setType(DisplayItem::Empty);
    
    timeHourClock.setType(DisplayItem::Empty);
    timeSeparatorClock.setType(DisplayItem::Empty);
    timeMinClock.setType(DisplayItem::Empty);
    
    timeHourAlarm.setType(DisplayItem::Empty);
    timeSeparatorAlarm.setType(DisplayItem::Empty);
    timeMinAlarm.setType(DisplayItem::Empty);
    
    Application::setTimesUp();
}

void AlarmClock::unsetTimesUp()
{
    minText.setType(DisplayItem::Text);
    secText.setType(DisplayItem::Text);
    
    timeHourClock.setType(DisplayItem::Text);
    timeSeparatorClock.setType(DisplayItem::Text);
    timeMinClock.setType(DisplayItem::Text);
    
    timeHourAlarm.setType(DisplayItem::Text);
    timeSeparatorAlarm.setType(DisplayItem::Text);
    timeMinAlarm.setType(DisplayItem::Text);
    
    Application::unsetTimesUp();
}

void AlarmClock::onPeriod()
{
    inputActions.push(Action::OnePeriod);
}

void AlarmClock::displayCommand()
{
    updateClock();
    writeHours(alarmHours);
    writeMinutes(alarmMinutes);
    std::pair<uint,uint> hourMins = minutesToHourMins(remainingMinutes);
    writeRemainingHours(hourMins.first);
    writeRemainingMinutes(hourMins.second);
    collectItems();
    Application::displayCommand(displayImage);
}

void AlarmClock::updateClock()
{
    Application::updateClock();
    timeHourClock.characters = time.characters.substr(0,2);
    timeMinClock.characters = time.characters.substr(3,2);
}

void AlarmClock::fireAlarm()
{
    std::cout<<"Fire Alarm"<<std::endl;
}

std::pair<uint,uint> AlarmClock::minutesToHourMins
(
    uint remainingMinutes
)
{
    return {remainingMinutes/60,remainingMinutes%60};
}

uint AlarmClock::minutesToHourMins(std::pair<uint,uint> hourMin)
{
    return hourMin.first*60+hourMin.second;
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
