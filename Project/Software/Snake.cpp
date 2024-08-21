#include "Snake.h"
#include "System.h"

Snake::Snake
(
    System* system
):
Application(system),
baseItem(54,15,10,"The only good alien \n    is a dead alien",255),
endItem(70,10,30,"Wasted",255),
topLine(15,0,15,128,255),
bottomLine(105,0,105,128,255),
onPeriodCount(0),
peroidCounter(0)
{
    modeStatus.characters = "Snake";
    leftButtonLabel.characters = "Start";
    centerButtonLabel.characters = "";
    rightButtonLabel.characters = "";
    
    baseItem.setType(DisplayItem::ItemType::Empty);
    endItem.setType(DisplayItem::ItemType::Empty);

    for(int w=0; w<gridW.size(); w++)
    {
        gridW[w] = DisplayItem(15,wSpacing*w+wSpacing,105,wSpacing*w+wSpacing,50);
    }
    for(int h=0; h<gridH.size(); h++)
    {
        gridH[h] = DisplayItem(hSpacing*h+15,0,hSpacing*(h)+15,128,50);
    }
    
    displayCommand();
    std::cout<<"Setup SpaceInvaders"<<std::endl;
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
                break;
            }
            case BtnCenterClick:
            {
                break;
            }
            case BtnRightClick:
            {
                break;
            }
            case RotateClock:
            {                
                break;
            }
            case RotateAntiClock:
            {
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

void Snake::onPeriod()
{
    peroidCounter++;
    if(peroidCounter>=50)
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
    displayImage.push_back(topLine);
    displayImage.push_back(bottomLine);
    
    for(DisplayItem& item : gridW)
        displayImage.push_back(item);
    for(DisplayItem& item : gridH)
        displayImage.push_back(item);
}
