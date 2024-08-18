#include "BaseApp.h"
#include "System.h"

BaseApp::BaseApp
(
    System* system
):
Application(system),
baseItem(54,40,20,"Hello",255),
peroidCounter(0)
{
    std::cout<<"Created Base app"<<std::endl;
    displayImage = {baseItem};
    displayCommand();
    std::cout<<"Setup BaseApp"<<std::endl;
}

void BaseApp::work()
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
                DisplayItem actionItem(54,70,10,"BtnLeftClick",128);
                displayImage.push_back(actionItem);
                break;
            }
            case BtnCenterClick:
            {
                DisplayItem actionItem(54,70,10,"BtnCenterClick",128);
                displayImage.push_back(actionItem);
                break;
            }
            case BtnRightClick:
            {
                DisplayItem actionItem(54,70,10,"BtnRightClick",128);
                displayImage.push_back(actionItem);
                break;
            }
            case RotateClock:
            {
                DisplayItem actionItem(54,70,10,"RotateClock",128);
                displayImage.push_back(actionItem);
                break;
            }
            case RotateAntiClock:
            {
                DisplayItem actionItem(54,70,10,"RotateAntiClock",128);                                displayImage.push_back(actionItem);
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

void BaseApp::onPeriod()
{
    peroidCounter++;
    if(peroidCounter>=100)
    {
        peroidCounter=0;
        inputActions.push(Action::OnePeriod);
    }
}

void BaseApp::displayCommand()
{
    if(system!=nullptr)
        system->displayImage(displayImage);
    else
        std::cout<<"Error: BaseApp::displayCommand nullptr"<<std::endl;
}

void BaseApp::speakerCommand()
{
}
