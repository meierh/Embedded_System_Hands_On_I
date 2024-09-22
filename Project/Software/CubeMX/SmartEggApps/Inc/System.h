#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include "DisplayItem.h"
#include "BaseApp.h"
#include "EggTimer.h"
#include "SmartEgg.h"
#include "AlarmClock.h"
#include "Stopwatch.h"
#include "SetClock.h"
#include "TestImage.h"
#include "DateTime.h"

class System
{
    public:
        System();
        virtual ~System();

        enum class PlayerAction:uint8_t{InitAlarm,InitPlayer,PlayAlarm,TogglePause,Next,Previous,IncreaseVolume,DecreaseVolume,Stop};
        enum class PlayingStatus:uint8_t{STATUS_STOPPED=0x00,STATUS_PLAYING=0x01,STATUS_PAUSED=0x02};

        virtual void work();
        
        //System -> Application
        virtual void buttonModePress();
        void buttonLeftPress();
        void buttonCenterPress();
        void buttonRightPress();
        enum Direction {Clockwise=1,Counterclockwise=-1};
        void rotate(Direction dir);
        void periodElapsed();
        
        //Application -> System
        virtual void displayImage(std::vector<DisplayItem> image)=0;
        virtual DateTime getSystemTime()=0;
        virtual int8_t getSeconds() =0;
        virtual void setSystemTime(DateTime newTime)=0;

        /**
         * Measures the current charge level of the battery.
         * @return The current charge level of the battery: 0-3 (0: empty, 3: full)
         */
        virtual uint8_t getBattery()=0;

        /**
         * Turns on the MP3 Player
         */
        virtual void playerTurnOn()=0;

        /**
         * Turns off the MP3 Player
         */
        virtual void playerTurnOff()=0;

        /**
         * Performs an action on the MP3 Player
         * @param action The action to perform
         */
        virtual void playerControl(PlayerAction action)=0;

        /**
         * Updates the state of the MP3 Player
         * @param status Indicates whether the player is stopped, playing or paused
         * @param volume The current volume of the player
         * @param numberOfTracks The total number of tracks available in the current mode
         * @param currentTrack The index of the currently playing track (in 1 ... numberOfTracks)
         */
        virtual void playerGetState(PlayingStatus &status, uint8_t &volume, uint16_t &numberOfTracks, uint16_t &currentTrack)=0;

    protected:
        enum App{BaseApp,SmartEgg,EggTimer,AlarmClock,Stopwatch,SetClock,TestImage,Empty};
        App current = Empty;
        Application* app = nullptr;
        bool blocked = false;
};
#endif
