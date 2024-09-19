#include "SpaceInvaders.h"
#include "System.h"

SpaceInvaders::SpaceInvaders
(
    System* system
):
Application(system),
baseItem(54,15,DisplayItem::Font12,"The only good alien \n    is a dead alien",255),
endItem(70,10,DisplayItem::Font24,"Wasted",255),
topLine(15,0,15,128,255),
bottomLine(105,0,105,128,255),
playerPosition(64),
player(97,playerPosition-10,{5,20},255,2),
alienVelocity(1),
spawnInterval(500),
onPeriodCount(0),
peroidCounter(0),
alienReachedBottom(false)
{
    player.setType(DisplayItem::Empty);
    modeStatus.characters = "SpaceInvaders";
    leftButtonLabel.characters = "Start";
    centerButtonLabel.characters = "";
    rightButtonLabel.characters = "Fire";
    endItem.setType(DisplayItem::ItemType::Empty);    

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
                
                //Move projectiles
                for(std::tuple<int,int>& pr : projectiles)
                {
                    std::get<0>(pr) = std::get<0>(pr)-moveLen(projectileSpeed);
                }
                
                //Remove out of area projectiles
                for(auto iter=projectiles.begin(); iter!=projectiles.end(); )
                {
                    if(std::get<0>(*iter)<15)
                        iter = projectiles.erase(iter);
                    else
                        iter++;
                }
                
                //Spawn aliens
                spawnAliens();
                
                //Move aliens
                for(std::tuple<int,int,int,int>& alien : aliens)
                {
                    std::get<0>(alien) = std::get<0>(alien)+moveLen(std::get<2>(alien));
                    std::get<1>(alien) = std::get<1>(alien)+moveLen(std::get<3>(alien));
                }
                
                // Remove and redirect out of bound aliens
                for(auto iter=aliens.begin(); iter!=aliens.end(); iter++)
                {
                    int h = std::get<0>(*iter);
                    int w = std::get<1>(*iter);
                                        
                    if(h>103)
                    {
                        alienReachedBottom = true;
                        iter = aliens.erase(iter);
                    }
                    else if(w>120)
                    {
                        std::get<3>(*iter) = -1*std::abs(std::get<3>(*iter));
                    }
                    else if(w<0)
                    {
                        std::get<3>(*iter) = std::abs(std::get<3>(*iter));
                    }
                }
                
                // Remove hit aliens and projectiles
                for(auto iterAlien=aliens.begin(); iterAlien!=aliens.end(); )
                {
                    int alienBottom = std::get<0>(*iterAlien);
                    int alienLeft = std::get<1>(*iterAlien);
                    int alienTop = alienBottom+alienDimH;
                    int alienRight = alienLeft+alienDimW;
                    
                    bool alienErased = false;
                    
                    for(auto iterProj=projectiles.begin(); iterProj!=projectiles.end(); )
                    {
                        int projectileBottom = std::get<0>(*iterProj);
                        int projectileLeft = std::get<1>(*iterProj);
                        int projectileTop = projectileBottom+projectileDimH;
                        int projectileRight = projectileLeft+projectileDimW;
                        
                        std::function<bool(int h,int w)> isInside =
                        [&](int h, int w)
                        {
                            if(w<alienRight && w>alienLeft && h<alienBottom && h>alienTop)
                                return true;
                            else 
                                return false;
                        };
                        if(isInside(projectileBottom,projectileLeft) ||
                           isInside(projectileBottom,projectileRight) ||
                           isInside(projectileTop,projectileLeft) ||
                           isInside(projectileTop,projectileRight) )
                        {
                            std::cout<<"---------------------Alien hit---------------------------"<<std::endl;
                            iterProj = projectiles.erase(iterProj);
                            alienErased = true;
                            break;
                        }
                        else
                            iterProj++;
                    }
                    
                    if(alienErased)
                        iterAlien = aliens.erase(iterAlien);
                    else
                        iterAlien++;
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
    if(onPeriodCount%4==0)
        return speed;
    else
        return 0;
}

void SpaceInvaders::drawPlayer()
{
    player.offsetW = playerPosition-10;
}

void SpaceInvaders::drawProjectiles()
{
    for(std::tuple<int,int> pr : projectiles)
    {
        displayImage.push_back(DisplayItem(std::get<0>(pr),std::get<1>(pr),{projectileDimH,projectileDimW},150,4));
    }
}

void SpaceInvaders::spawnAliens()
{
    std::cout<<"Spawn:"<<onPeriodCount<<" | "<<onPeriodCount%spawnInterval<<"  int:"<<spawnInterval<<std::endl;
    if(onPeriodCount%50==0)
    {
        std::cout<<"Spawn"<<std::endl;
        aliens.push_back({15,64,1,3});
    }
}

void SpaceInvaders::drawAliens()
{
    for(std::tuple<int,int,int,int> al : aliens)
    {
        displayImage.push_back(DisplayItem(std::get<0>(al),std::get<1>(al),{alienDimH,alienDimW},150,4));
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
    
    drawProjectiles();
    drawAliens();
}
