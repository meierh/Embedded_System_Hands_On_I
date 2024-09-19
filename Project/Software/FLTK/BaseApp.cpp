#include "BaseApp.h"
#include "System.h"

BaseApp::BaseApp
(
    System* system
):
Application(system),
baseItem(54,20,DisplayItem::Font20,"Welcome",255),
actionItem(74,40,DisplayItem::Font12,"",128),
peroidCounter(0)
{
    modeStatus.characters = "Base";
    actionItem.setType(DisplayItem::ItemType::Empty);
    std::cout<<"Setup BaseApp"<<std::endl;
}

void BaseApp::work()
{
    if(!inputActions.empty())
    {
        Action action = inputActions.front();
        inputActions.pop();
        switch(action)
        {
            case BtnLeftClick:
            {
                actionItem.characters="BtnLeftClick";
                actionItem.setType(DisplayItem::ItemType::Text);
                break;
            }
            case BtnCenterClick:
            {
                actionItem.characters="BtnCenterClick";
                actionItem.setType(DisplayItem::ItemType::Text);
                break;
            }
            case BtnRightClick:
            {
                actionItem.characters="BtnRightClick";
                actionItem.setType(DisplayItem::ItemType::Text);
                break;
            }
            case RotateClock:
            {                
                actionItem.characters="RotateClock";
                actionItem.setType(DisplayItem::ItemType::Text);
                break;
            }
            case RotateAntiClock:
            {
                actionItem.characters="RotateAntiClock";
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
    peroidCounter++;
    if(peroidCounter>=100)
    {
        peroidCounter=0;
        inputActions.push(Action::OnePeriod);
    }
}

void BaseApp::displayCommand()
{
    updateClock();
    collectItems();
    displayCommand(displayImage);
}

void BaseApp::displayCommand(std::vector<DisplayItem> items)
{
    if(system!=nullptr)
        system->displayImage(displayImage);
}

void BaseApp::speakerCommand()
{
}

void BaseApp::collectItems()
{
    displayImage.clear();

    Application::collectItems();
    
    displayImage.push_back(baseItem);
    displayImage.push_back(actionItem);    
}
