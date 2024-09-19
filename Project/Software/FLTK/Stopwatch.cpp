#include "Stopwatch.h"
#include "System.h"

Stopwatch::Stopwatch
(
    System* system
):
Application(system),
periodCounter(0),
seconds(0),
intermediateSeconds(0)
{
    modeStatus.characters = "Stopwatch";
    leftButtonLabel.characters = "Start";
    centerButtonLabel.characters = "Null";
    centerButtonLabel.offsetW += 5 ;
    rightButtonLabel.characters = "Stop";
    
    timeHour = DisplayItem(60,15,DisplayItem::Font24,"00",255);
    timeSeparator = DisplayItem(60,43,DisplayItem::Font24,":",255);
    timeMin = DisplayItem(60,55,DisplayItem::Font24,"00",255);
    timeSec = DisplayItem(60,90,DisplayItem::Font20,"00",255);
    
    interTimeHour = DisplayItem(90,15,DisplayItem::Font24,"00",128);
    interTimeSeparator = DisplayItem(90,43,DisplayItem::Font24,":",128);
    interTimeMin = DisplayItem(90,55,DisplayItem::Font24,"00",128);
    interTimeSec = DisplayItem(90,90,DisplayItem::Font20,"00",128);
    
    interTimeHour.setType(DisplayItem::Empty);
    interTimeSeparator.setType(DisplayItem::Empty);
    interTimeMin.setType(DisplayItem::Empty);
    interTimeSec.setType(DisplayItem::Empty);
    
    std::cout<<"Setup Stopwatch"<<std::endl;
}

void Stopwatch::work()
{
    if(!inputActions.empty())
    {
        Action action = inputActions.front();
        inputActions.pop();
        switch(action)
        {
            case BtnLeftClick:
            {
                if(status==End)
                {
                    status=Run;
                }
                break;
            }
            case BtnCenterClick:
            {
                if(status==End)
                {
                    seconds = 0;
                    intermediateSeconds = 0;
                    interTimeHour.setType(DisplayItem::Empty);
                    interTimeSeparator.setType(DisplayItem::Empty);
                    interTimeMin.setType(DisplayItem::Empty);
                    interTimeSec.setType(DisplayItem::Empty);
                }
                break;
            }
            case BtnRightClick:
            {
                if(status==Run || status==RunWithIntermed)
                {
                    status=End;
                }
                break;
            }
            case RotateClock:
            case RotateAntiClock:
            {
                if(status==Run || status==RunWithIntermed)
                {
                    if(status==Run)
                    {
                        status=RunWithIntermed;
                        interTimeHour.setType(DisplayItem::Text);
                        interTimeSeparator.setType(DisplayItem::Text);
                        interTimeMin.setType(DisplayItem::Text);
                        interTimeSec.setType(DisplayItem::Text);
                    }
                    intermediateSeconds = seconds;
                }
                break;
            }
            case OnePeriod:
            {
                if(status==Run || status==RunWithIntermed)
                    seconds++;
                break;
            }
            default:
                break;
        }
        displayCommand();
    }
}

void Stopwatch::onPeriod()
{
    periodCounter++;
    if(periodCounter>=100)
    {
        periodCounter=0;
        inputActions.push(Action::OnePeriod);
    }
}

void Stopwatch::displayCommand()
{
    updateClock();
    writeSeconds(seconds);
    writeIntermedSeconds(intermediateSeconds);
    collectItems();
    displayCommand(displayImage);
}

void Stopwatch::displayCommand(std::vector<DisplayItem> items)
{
    if(system!=nullptr)
        system->displayImage(displayImage);
}

void Stopwatch::speakerCommand()
{
}

void Stopwatch::writeSeconds(uint seconds)
{
    uint hours = seconds / 3600;
    if(hours<10)
        timeHour.characters = "0"+std::to_string(hours);
    else
        timeHour.characters = std::to_string(hours);
    
    uint minutes = seconds / 60;
    if(minutes<10)
        timeMin.characters = "0"+std::to_string(minutes);
    else
        timeMin.characters = std::to_string(minutes);
    
    seconds = seconds%60;
    if(seconds<10)
        timeSec.characters = "0"+std::to_string(seconds);
    else
        timeSec.characters = std::to_string(seconds);    
}

void Stopwatch::writeIntermedSeconds(uint intermedSeconds)
{
    uint hours = intermedSeconds / 3600;
    if(hours<10)
        interTimeHour.characters = "0"+std::to_string(hours);
    else
        interTimeHour.characters = std::to_string(hours);
    
    uint minutes = intermedSeconds / 60;
    if(minutes<10)
        interTimeMin.characters = "0"+std::to_string(minutes);
    else
        interTimeMin.characters = std::to_string(minutes);
    
    intermedSeconds = intermedSeconds%60;
    if(intermedSeconds<10)
        interTimeSec.characters = "0"+std::to_string(intermedSeconds);
    else
        interTimeSec.characters = std::to_string(intermedSeconds);  
}

void Stopwatch::collectItems()
{
    displayImage.clear();

    Application::collectItems();
    
    displayImage.push_back(timeHour);
    displayImage.push_back(timeSeparator);
    displayImage.push_back(timeMin);
    displayImage.push_back(timeSec);
    
    displayImage.push_back(interTimeHour);
    displayImage.push_back(interTimeSeparator);
    displayImage.push_back(interTimeMin);
    displayImage.push_back(interTimeSec);
}
