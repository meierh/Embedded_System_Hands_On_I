#ifndef SNAKE_H
#define SNAKE_H

#include <cstdint>
#include <string>
#include "Application.h"
#include <list>
#include <array>

class Snake : public Application
{
    public:
        Snake(System* system);
        ~Snake() = default;

        /**
         *  Internal action
         */
        void work() override;
        
        /**
         *  Input action
         */
        void onPeriod() override;
        
        /**
         *  Output action
         */
        void displayCommand() override;
        void displayCommand(std::vector<DisplayItem> items);
    
    private:
        
        enum GameStatus{Initial,Play,End};
        GameStatus status = Initial;
       
        DisplayItem baseItem;
        DisplayItem endItem;
        
        DisplayItem bottomLine;
        DisplayItem topLine;
        
        std::array<DisplayItem,31> drawGridW();
        std::array<DisplayItem,17> drawGridH();
        int wSpacing = 4;
        const std::array<DisplayItem,31> gridW;
        int hSpacing = 5;
        const std::array<DisplayItem,17> gridH;
        
        enum SquareItem {SnakeBody, Food, Empty};
        std::array<std::array<SquareItem,30>,16> playGridSet;
        
        using Square = std::pair<int,int>;
        std::list<Square> snake;
        enum SnakeDirection{Left,Forward,Right};
        SnakeDirection nextDirection = Forward;

        void updateGameStatus();
        void printBoard(std::vector<DisplayItem>& boardItems);
        
        uint peroidCounter;
                
        void collectItems() override;
};


#endif
