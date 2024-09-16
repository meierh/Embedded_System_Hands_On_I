#include "Snake.h"
#include "System.h"

Snake::Snake
(
    System* system
):
Application(system),
baseItem(54,15,DisplayItem::Font8,"The only good alien \n    is a dead alien",255),
endItem(70,10,DisplayItem::Font24,"Wasted",255),
topLine(15,0,15,128,255),
bottomLine(105,0,105,128,255),
peroidCounter(0),
gridW(drawGridW()),
gridH(drawGridH())
{
    modeStatus.characters = "Snake";
    leftButtonLabel.characters = "Start";
    centerButtonLabel.characters = "";
    rightButtonLabel.characters = "";
    
    baseItem.setType(DisplayItem::ItemType::Empty);
    endItem.setType(DisplayItem::ItemType::Empty);
    
    for(int rowInd=0; rowInd<playGridSet.size(); rowInd++)
    {
        for(int colInd=0; colInd<playGridSet[rowInd].size(); colInd++)
        {
            playGridSet[rowInd][colInd] = Empty;
        }
    }
    
    snake.push_back({20,10});
    snake.push_back({21,10});
    
    updateGameStatus();
    
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
                nextDirection = Left;
                break;
            }
            case BtnCenterClick:
            {
                break;
            }
            case BtnRightClick:
            {
                nextDirection = Right;
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
                Square first = *(snake.begin());
                Square second = *(++(snake.begin()));
                Square snakeDir = {first.first-second.first,first.second-second.second};
                
                Square nextSquare = first;
                if(snakeDir.first<0)
                {
                    switch (nextDirection)
                    {
                        case Right:
                            nextSquare.second++;
                            break;
                        case Forward:
                            nextSquare.first--;
                            break;
                        case Left:
                            nextSquare.second--;
                            break;
                    }
                }
                else if(snakeDir.first>0)
                {
                    switch (nextDirection)
                    {
                        case Right:
                            nextSquare.second--;
                            break;
                        case Forward:
                            nextSquare.first++;
                            break;
                        case Left:
                            nextSquare.second++;
                            break;
                    }
                }
                else
                {
                    if(snakeDir.second<0)
                    {
                        switch (nextDirection)
                        {
                            case Right:
                                nextSquare.first++;
                                break;
                            case Forward:
                                nextSquare.second--;
                                break;
                            case Left:
                                nextSquare.first--;
                                break;
                        }
                    }
                    else if(snakeDir.second>0)
                    {
                        switch (nextDirection)
                        {
                            case Right:
                                nextSquare.first--;
                                break;
                            case Forward:
                                nextSquare.second++;
                                break;
                            case Left:
                                nextSquare.first++;
                                break;
                        }
                    }
                }
                
                snake.push_front(nextSquare);
                if(playGridSet[nextSquare.first][nextSquare.second]==Food)
                {
                    playGridSet[nextSquare.first][nextSquare.second]=Empty;
                }
                else
                {
                    snake.pop_back();
                }
                updateGameStatus();
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

std::array<DisplayItem,31> Snake::drawGridW()
{
    std::array<DisplayItem,31> gridW;
    for(int w=0; w<gridW.size(); w++)
    {
        gridW[w] = DisplayItem(16,wSpacing*w+wSpacing,104,wSpacing*w+wSpacing,50);
    }
    return gridW;
}

std::array<DisplayItem,17> Snake::drawGridH()
{
    std::array<DisplayItem,17> gridH;
    for(int h=0; h<gridH.size(); h++)
    {
        gridH[h] = DisplayItem(hSpacing*h+15+hSpacing,0,hSpacing*(h)+15+hSpacing,128,50);
    }
    return gridH;
}

void Snake::updateGameStatus()
{
    /*
    for(Square& sq : snake)
    {
        if(sq.first<15)
            sq.first=105;
        else if(sq.first>105)
            sq.first=15;
        
        if(sq.second<0)
            sq.second=128;
        else if(sq.second>128)
            sq.second=0;
    }
    */
    
    for(int rowInd=0; rowInd<playGridSet.size(); rowInd++)
    {
        for(int colInd=0; colInd<playGridSet[rowInd].size(); colInd++)
        {
            if(playGridSet[rowInd][colInd]==SnakeBody)
                playGridSet[rowInd][colInd] = Empty;
        }
    }
    
    for(Square& sq : snake)
    {
        playGridSet[sq.first][sq.second]=SnakeBody;
    }
}

void Snake::printBoard
(
    std::vector<DisplayItem>& boardItems
)
{
    for(int rowInd=0; rowInd<playGridSet.size(); rowInd++)
    {
        for(int colInd=0; colInd<playGridSet[rowInd].size(); colInd++)
        {
            if(playGridSet[rowInd][colInd]==SnakeBody)
                boardItems.push_back(DisplayItem(rowInd*wSpacing+16,colInd*hSpacing,{5,4},255,4));
        }
    }
}

void Snake::collectItems()
{
    displayImage.clear();

    Application::collectItems();
    
    displayImage.push_back(baseItem);
    displayImage.push_back(topLine);
    displayImage.push_back(bottomLine);
    
    for(const DisplayItem& item : gridW)
        displayImage.push_back(item);
    for(const DisplayItem& item : gridH)
        displayImage.push_back(item);
    
    printBoard(displayImage);
}
