#include "SmartEgg.h"
#include "System.h"

SmartEgg::SmartEgg
(
    System* system
):
Application(system),
baseItem(32,32,10,"Hello",255),
peroidCounter(0)
{
    displayCommand();
}

void SmartEgg::work()
{
    if(!inputActions.empty())
    {
        Action action = inputActions.front();
        inputActions.pop();
        displayImage.push_back(baseItem);
        switch(action)
        {
            case BtnLeftClick:
            {
                DisplayItem actionItem(48,32,10,"BtnLeftClick",255);
                displayImage.push_back(actionItem);
                break;
            }
            case BtnCenterClick:
            {
                DisplayItem actionItem(48,32,10,"BtnCenterClick",255);
                displayImage.push_back(actionItem);
                break;
            }
            case BtnRightClick:
            {
                DisplayItem actionItem(48,32,10,"BtnRightClick",255);
                displayImage.push_back(actionItem);
                break;
            }
            case RotateClock:
            {
                DisplayItem actionItem(48,32,10,"RotateClock",255);
                displayImage.push_back(actionItem);
                break;
            }
            case RotateAntiClock:
            {
                DisplayItem actionItem(48,32,10,"RotateAntiClock",255);                                displayImage.push_back(actionItem);
                break;
            }
            case OnePeriod:
                break;
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
    system->displayImage(displayImage);
}

void SmartEgg::speakerCommand()
{
}
