//
// Created by Tobias Henig on 22.09.24.
//

#ifndef DFP_CONTROLLER_H
#define DFP_CONTROLLER_H

#include "DFP_Definitions.h"
using namespace DFP;

class DFP_Controller
{
    static constexpr auto INIT_EQUALIZER = EqualizerType::ROCK;
    static constexpr uint8_t INIT_VOLUME = 25;
    static constexpr uint8_t FOLDER_MP3 = 1;
    static constexpr uint8_t FOLDER_ALARM = 2;
    static constexpr uint16_t MIN_UPDATE_INTERVAL_TRACK = 7000; // ms
    static constexpr uint16_t MIN_UPDATE_INTERVAL_OTHER = 21000; // ms

public:
    DFP_Controller();
    ~DFP_Controller() = default;

    /**
     * Turns on power to the DFPlayer
     * @param initState if true, initializes the state of to the default values in MP3 player mode.
     *                  Caution: This causes this method to block for about one second.
     */
    void turnOn(bool initState = false);
    /**
     * Turns off power to the DFPlayer
     */
    void turnOff();

    void init();

    /**
     * Starts the player and play the MP3 files in the MP3 folder in a loop
     */
    void startPlayer();
    /**
     * Randomly selects an alarm sound and plays it in a loop
     */
    void playAlarm();

    void stop();

    void play();
    void pause();
    /**
     * Pauses the player if it is playing and plays it if it is paused
     */
    void togglePause();

    void next();
    void previous();
    void changeVolume(int8_t offset);
    /**
     * Repots the current state of the player to the caller
     * @param status reference to the status variable
     * @param volume reference to the volume variable
     * @param numberOfTracks reference to the number of tracks variable
     * @param currentTrack reference to the current track variable
     */
    void getState(PlayingStatus& status, uint8_t& volume, uint16_t& numberOfTracks, uint16_t& currentTrack);

private:
    PlayingStatus status;
    uint8_t volume;
    uint16_t numberOfTracks;
    uint16_t currentTrack;
    uint16_t trackOffset;

    uint32_t lastUpdateTrack, lastUpdateOther;
};


#endif //DFP_CONTROLLER_H
