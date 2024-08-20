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
        void speakerCommand() override;
        void displayCommand() override;
        void displayCommand(std::vector<DisplayItem> items);
    
    private:
        enum GameStatus{Initial,Play,End};
        GameStatus status = Initial;
       
        DisplayItem baseItem;
        DisplayItem endItem;
        
        DisplayItem bottomLine;
        DisplayItem topLine;

        uint peroidCounter;
        
        int playerPosition;
        DisplayItem player;
        void drawPlayer();
        
        const uint projectileSpeed = 20;
        std::list<std::tuple<uint,uint>> projectiles;
        void drawProjectiles();
        
        std::list<std::tuple<uint,uint,uint,uint>> aliens;
        uint alienVelocity;
        uint spawnInterval;
        
        void collectItems() override;
};


#endif
