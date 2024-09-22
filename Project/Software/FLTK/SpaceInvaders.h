#ifndef SPACEINVADERS_H
#define SPACEINVADERS_H

#include <cstdint>
#include <string>
#include "Application.h"
#include <list>

class SpaceInvaders : public Application
{
    public:
        SpaceInvaders(System* system);
        ~SpaceInvaders() = default;

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
        uint onPeriodCount;
        
        enum GameStatus{Initial,Play,End};
        GameStatus status = Initial;
       
        DisplayItem baseItem;
        DisplayItem endItem;
        
        DisplayItem bottomLine;
        DisplayItem topLine;

        uint peroidCounter;
        
        uint moveLen(uint speed);
        
        int playerPosition;
        DisplayItem player;
        void drawPlayer();
                
        const uint projectileSpeed = 5;
        const uint projectileDimH = 3;
        const uint projectileDimW = 3;
        std::list<std::tuple<int,int>> projectiles;
        void drawProjectiles();
        
        uint spawnInterval = 1;
        const uint alienDimH = 10;
        const uint alienDimW = 10;
        void spawnAliens();
        std::list<std::tuple<int,int,int,int>> aliens;
        bool alienReachedBottom;
        uint alienVelocity;
        void drawAliens();
        
        void collectItems() override;
};


#endif
