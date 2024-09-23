#ifndef MP3PLAYER_H
#define MP3PLAYER_H

#include <cstdint>
#include <string>
#include "Application.h"

class MP3Player : public Application
{
    public:
        MP3Player(System* system);
        ~MP3Player();

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
        uint periodCounter;

        uint16_t trackNumber;
        uint16_t trackID;
        uint8_t volume;
        bool playing;
        
        void readMP3Info();
        void displayMP3Info();
        void displayVolume();
        
        enum SetClockStatus{SetTime,Entered};
        SetClockStatus status = Entered;
                
        DisplayItem trackLabel;
        DisplayItem trackIDLabel;
        DisplayItem trackSeparatorLabel;
        DisplayItem trackNumberLabel;
        
        std::array<DisplayItem,62> volumeLabels;
        
        void collectItems();
};


#endif
