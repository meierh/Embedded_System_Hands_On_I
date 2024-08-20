#include "Snake.h"
#include "System.h"

Snake::Snake
(
    System* system
):
Application(system),
baseItem(54,20,20,"Welcome",255),
actionItem(74,40,10,"",128),
peroidCounter(0)
{
    modeStatus.characters = "Snake";
    actionItem.setType(DisplayItem::ItemType::Empty);
    displayCommand();
    std::cout<<"Setup Snake"<<std::endl;
}

void Snake::work()
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

void Snake::onPeriod()
{
    peroidCounter++;
    if(peroidCounter>=100)
    {
        peroidCounter=0;
        inputActions.push(Action::OnePeriod);
    }
}

void Snake::displayCommand()
{
    updateClock();
    collectItems();
    displayCommand(displayImage);
}

void Snake::displayCommand(std::vector<DisplayItem> items)
{
    if(system!=nullptr)
        system->displayImage(displayImage);
}

void Snake::speakerCommand()
{
}

void Snake::collectItems()
{
    displayImage.clear();

    Application::collectItems();
    
    displayImage.push_back(baseItem);
    displayImage.push_back(actionItem);    
}
