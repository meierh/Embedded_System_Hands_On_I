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

    void turnOn();
    void turnOff();

    void init();

    void startPlayer();
    void playAlarm();

    void stop();

    void play();
    void pause();
    void togglePause();

    void next();
    void previous();
    void changeVolume(int8_t offset);
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
