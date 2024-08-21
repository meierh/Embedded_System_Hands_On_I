#include "SpaceInvaders.h"
#include "System.h"

SpaceInvaders::SpaceInvaders
(
    System* system
):
Application(system),
baseItem(54,15,10,"The only good alien \n    is a dead alien",255),
endItem(70,10,30,"Wasted",255),
topLine(15,0,15,128,255),
bottomLine(105,0,105,128,255),
playerPosition(64),
player(97,playerPosition-10,{5,20},255,2),
alienVelocity(1),
spawnInterval(500),
onPeriodCount(0),
peroidCounter(0)
{
    modeStatus.characters = "SpaceInvaders";
    leftButtonLabel.characters = "Start";
    centerButtonLabel.characters = "";
    rightButtonLabel.characters = "Fire";
    endItem.setType(DisplayItem::ItemType::Empty);    
    displayCommand();
    std::cout<<"Setup SpaceInvaders"<<std::endl;
}

void SpaceInvaders::work()
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
                    status = Initial;
                    endItem.setType(DisplayItem::Empty);
                    baseItem.setType(DisplayItem::Text);
                    playerPosition = 64;
                    projectiles.clear();
                    aliens.clear();
                    alienVelocity = 1;
                    spawnInterval = 500;
                }
                else if(status==Initial)
                {
                    baseItem.setType(DisplayItem::Empty);
                    player.setType(DisplayItem::Rectangle);
                    status = Play;
                }
                else
                {
                    status = Initial;
                    endItem.setType(DisplayItem::Empty);
                    baseItem.setType(DisplayItem::Text);
                    playerPosition = 64;
                    projectiles.clear();
                    aliens.clear();
                    alienVelocity = 1;
                    spawnInterval = 500;
                }
                break;
            }
            case BtnCenterClick:
            {

                break;
            }
            case BtnRightClick:
            {
                std::tuple<int,int> newProjectile = {95,playerPosition-2};
                projectiles.push_back(newProjectile);
                break;
            }
            case RotateClock:
            {             
                if(status==Play)
                {
                    playerPosition+=5;
                    if(playerPosition>128)
                        playerPosition=128;
                }
                break;
            }
            case RotateAntiClock:
            {
                if(status==Play)
                {
                    playerPosition-=5;
                    if(playerPosition<0)
                        playerPosition=0;
                }
                break;
            }
            case OnePeriod:
            {
                onPeriodCount++;
                for(std::tuple<int,int>& pr : projectiles)
                {
                    std::get<0>(pr) = std::get<0>(pr)-moveLen(projectileSpeed);
                }
                for(auto iter=projectiles.begin(); iter!=projectiles.end(); iter++)
                {
                    if(std::get<0>(*iter)<15)
                        iter = projectiles.erase(iter);
                }
                break;
            }
            default:
                break;
        }
        displayCommand();
    }
}

void SpaceInvaders::onPeriod()
{
    peroidCounter++;
    if(peroidCounter>=3)
    {
        peroidCounter=0;
        inputActions.push(Action::OnePeriod);
    }
}

void SpaceInvaders::displayCommand()
{
    updateClock();
    drawPlayer();
    collectItems();
    displayCommand(displayImage);
}

void SpaceInvaders::displayCommand(std::vector<DisplayItem> items)
{
    if(system!=nullptr)
        system->displayImage(displayImage);
}

void SpaceInvaders::speakerCommand()
{
}

uint SpaceInvaders::moveLen(uint speed)
{
    return speed;
}

void SpaceInvaders::drawPlayer()
{
    player.offsetW = playerPosition-10;
}

void SpaceInvaders::drawProjectiles()
{
    for(std::tuple<int,int> pr : projectiles)
    {
        displayImage.push_back(DisplayItem(std::get<0>(pr),std::get<1>(pr),{8,4},150,4));
    }
}

void SpaceInvaders::collectItems()
{
    displayImage.clear();

    Application::collectItems();
    
    displayImage.push_back(baseItem);
    displayImage.push_back(endItem);
    displayImage.push_back(bottomLine);
    displayImage.push_back(topLine);
    displayImage.push_back(player);
    
    for(std::tuple<int,int>& pr : projectiles)
    {
        displayImage.push_back(DisplayItem(std::get<0>(pr),std::get<1>(pr),{3,3},255,5));
    }
}
