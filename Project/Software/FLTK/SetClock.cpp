#include "SetClock.h"
#include "System.h"

SetClock::SetClock
(
    System* system
):
Application(system),
periodCounter(0)
{
    modeStatus.characters = "Set Clock";
    leftButtonLabel.characters = "<<-";
    centerButtonLabel.characters = "Enter";
    centerButtonLabel.offsetW += 5 ;
    rightButtonLabel.characters = "->>";
    
    clockHour = DisplayItem(60,15,DisplayItem::Font24,"00",55);
    clockSeparator = DisplayItem(60,45,DisplayItem::Font24,":",55);
    clockMin = DisplayItem(60,55,DisplayItem::Font24,"00",55);
    clockSec = DisplayItem(60,90,DisplayItem::Font20,"00",55);
    
    clockDay = DisplayItem(90,15,DisplayItem::Font16,"01",55);
    clockDaySeparator = DisplayItem(90,37,DisplayItem::Font16,".",55);
    clockMonth = DisplayItem(90,46,DisplayItem::Font16,"01",55);
    clockMonthSeparator = DisplayItem(90,67,DisplayItem::Font16,".",55);
    clockYear = DisplayItem(90,75,DisplayItem::Font16,"2000",55);
        
    setToCurrentTime();
    writeDate();
    
    std::cout<<"Setup SetClock"<<std::endl;
}

void SetClock::work()
{
    if(!inputActions.empty())
    {
        Action action = inputActions.front();
        inputActions.pop();
        switch(action)
        {
            case BtnLeftClick:
            {
                if(status==Entered)
                {
                    status=SetTime;
                    highlight();
                }
                else
                {
                    changeIndex--;
                    changeIndex = std::max(0,changeIndex);
                    unhighlight();
                    highlight();
                }
                break;
            }
            case BtnCenterClick:
            {
                if(status==Entered)
                {
                }
                else
                {
                    system->setSystemTime(DateTime(year,month,day,hour,minute,second,0,-1));
                    status=Entered;
                    unhighlight();
                }
                break;
            }
            case BtnRightClick:
            {
                if(status==Entered)
                {
                    status=SetTime;
                    highlight();
                }
                else
                {
                    changeIndex++;
                    changeIndex = std::min(5,changeIndex);
                    unhighlight();
                    highlight();
                }
                break;
            }
            case RotateClock:
            {
                if(status==SetTime)
                {
                    print();
                    switch(changeIndex)
                    {
                        case 0:
                            hour++;
                            break;
                        case 1:
                            minute++;
                            break;
                        case 2:
                            second++;
                            std::cout<<"incr"<<std::endl;
                            break;
                        case 3:
                            day++;
                            break;
                        case 4:
                            month++;
                            break;
                        case 5:
                            year++;
                            break;
                        default:
                            break;
                    }
                    print();
                    restrictTime();
                    writeDate();
                    print();
                }
                break;
            }
            case RotateAntiClock:
            {
                if(status==SetTime)
                {
                    print();
                    switch(changeIndex)
                    {
                        case 0:
                            hour--;
                            break;
                        case 1:
                            minute--;
                            break;
                        case 2:
                            second--;
                            std::cout<<"decr:"<<second<<std::endl;
                            break;
                        case 3:
                            day--;
                            break;
                        case 4:
                            month--;
                            break;
                        case 5:
                            year--;
                            break;
                        default:
                            break;
                    }
                    print();
                    restrictTime();
                    writeDate();
                    print();
                }
                break;
            }
            case OnePeriod:
            {
                break;
            }
            default:
                break;
        }
        displayCommand();
    }
}

void SetClock::onPeriod()
{
    inputActions.push(Action::OnePeriod);
    /*periodCounter++;
    if(periodCounter>=100)
    {
        periodCounter=0;
        inputActions.push(Action::OnePeriod);
    }*/
}

void SetClock::displayCommand()
{
    updateClock();
    collectItems();
    Application::displayCommand(displayImage);
}

void SetClock::writeDate()
{
    clockYear.characters = std::to_string(year);
    if(month<10)
        clockMonth.characters = "0"+std::to_string(month);
    else
        clockMonth.characters = std::to_string(month);
    if(day<10)
        clockDay.characters = "0"+std::to_string(day);
    else
        clockDay.characters = std::to_string(day);

    if(hour<10)
        clockHour.characters = "0"+std::to_string(hour);
    else
        clockHour.characters = std::to_string(hour);
    if(minute<10)
        clockMin.characters = "0"+std::to_string(minute);
    else
        clockMin.characters = std::to_string(minute);
    if(second<10)
        clockSec.characters = "0"+std::to_string(second);
    else
        clockSec.characters = std::to_string(second);
}

void SetClock::restrictTime()
{
    std::cout<<"day:"<<day<<std::endl;
    year = std::min<int>(2100,std::max<int>(1900,year));
    month = std::min<int>(12,std::max<int>(1,month));
    day = std::min<int>(31,std::max<int>(1,day));
    hour = std::min<int>(23,std::max<int>(0,hour));
    minute = std::min<int>(59,std::max<int>(0,minute));
    second = std::min<int>(59,std::max<int>(0,second));
    std::cout<<"day:"<<day<<std::endl;
    restrictDay();
    std::cout<<"day:"<<day<<std::endl;
}

void SetClock::setToCurrentTime()
{
    DateTime time = system->getSystemTime();
    year = time.getYear();
    month = time.getMonth();
    day = time.getDay();
    hour = time.getHour();
    minute = time.getMinute();
    second = time.getSecond();
    restrictTime();
}

void SetClock::restrictDay()
{
    switch(month)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            day = std::min<int>(31,std::max<int>(1,day));
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            day = std::min<int>(30,std::max<int>(1,day));
            break;
        case 2:
        {
            if(year%4==0)
            {
                if(year%100==0)
                {
                    if(year%400==0)
                    {
                        day = std::min<int>(29,std::max<int>(1,day));
                        break;
                    }
                    day = std::min<int>(28,std::max<int>(1,day));
                    break;
                }
                day = std::min<int>(29,std::max<int>(1,day));
                break;
            }
            else
                day = std::min<uint>(28,std::max<uint>(1,day));
            break;
        }
    }
}

void SetClock::highlight()
{
    switch(changeIndex)
    {
        case 0:
            clockHour.intensity = 255;
            break;
        case 1:
            clockMin.intensity = 255;
            break;
        case 2:
            clockSec.intensity = 255;
            break;
        case 3:
            clockDay.intensity = 255;
            break;
        case 4:
            clockMonth.intensity = 255;
            break;
        case 5:
            clockYear.intensity = 255;
            break;
        default:
            break;
    }
}

void SetClock::unhighlight()
{
    clockHour.intensity = 30;
    clockMin.intensity = 30;
    clockSec.intensity = 30;
    clockYear.intensity = 30;
    clockMonth.intensity = 30;
    clockDay.intensity = 30;
}

void SetClock::print()
{
    writeDate();
    std::cout<<changeIndex<<"|"<<clockHour.characters<<":"<<clockMin.characters<<":"<<clockSec.characters<<" | "<<clockDay.characters<<"."<<clockMonth.characters<<"."<<clockYear.characters<<std::endl;
}

void SetClock::collectItems()
{
    displayImage.clear();

    Application::collectItems();
    
    displayImage.push_back(clockHour);
    displayImage.push_back(clockSeparator);
    displayImage.push_back(clockMin);
    displayImage.push_back(clockSec);
    
    displayImage.push_back(clockDay);
    displayImage.push_back(clockDaySeparator);
    displayImage.push_back(clockMonth);
    displayImage.push_back(clockMonthSeparator);
    displayImage.push_back(clockYear);
}
