#include "BaseApp.h"
#include "System.h"

BaseApp::BaseApp
(
    System* system
):
Application(system),
baseItem(54,20,DisplayItem::Font20,"Welcome",255),
actionItem(74,30,DisplayItem::Font12,"",128),
peroidCounter(0)
{
    modeStatus.characters = "Base";
    actionItem.setType(DisplayItem::ItemType::Empty);
    std::cout<<"Setup BaseApp"<<std::endl;
}

void BaseApp::work()
{
    volatile int test = 5;
    if(!inputActions.empty())
    {
        Action action = inputActions.front();
        inputActions.pop();
        switch(action)
        {
            case BtnLeftClick:
            {
                actionItem.characters="BtnLeftClick";
                actionItem.offsetW=30;
                actionItem.setType(DisplayItem::ItemType::Text);
                break;
            }
            case BtnCenterClick:
            {
                actionItem.characters="BtnCenterClick";
                actionItem.offsetW=20;
                actionItem.setType(DisplayItem::ItemType::Text);
                break;
            }
            case BtnRightClick:
            {
                actionItem.characters="BtnRightClick";
                actionItem.offsetW=25;
                actionItem.setType(DisplayItem::ItemType::Text);
                break;
            }
            case RotateClock:
            {                
                actionItem.characters="RotateClock";
                actionItem.offsetW=30;
                actionItem.setType(DisplayItem::ItemType::Text);
                break;
            }
            case RotateAntiClock:
            {
                actionItem.characters="RotateAntiClock";
                actionItem.offsetW=18;
                actionItem.setType(DisplayItem::ItemType::Text);
                break;
            }
            case OnePeriod:
            {
                actionItem.setType(DisplayItem::ItemType::Empty);
                break;
            }
            default:
                break;
        }
        displayCommand();
    }
}

void BaseApp::onPeriod()
{
    inputActions.push(Action::OnePeriod);
}

void BaseApp::displayCommand()
{
    collectItems();
    Application::displayCommand(displayImage);
}

void BaseApp::collectItems()
{
    displayImage.clear();

    Application::collectItems();
    
    displayImage.push_back(baseItem);
    displayImage.push_back(actionItem);    
}
