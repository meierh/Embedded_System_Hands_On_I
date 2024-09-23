//
// Created by Tobias Henig on 22.09.24.
//

#include "DFP_Controller.h"

#include <cstdlib>

#include "main.h"
#include "DFP_Interface.h"

DFP_Controller::DFP_Controller() :
    status(PlayingStatus::STATUS_STOPPED),
    volume(0),
    numberOfTracks(0),
    currentTrack(0),
    trackOffset(0),
    lastUpdateTrack(0),
    lastUpdateOther(0)
{
}

void DFP_Controller::turnOn(bool initState)
{
    HAL_GPIO_WritePin(DFPlayer_On_Off_GPIO_Port, DFPlayer_On_Off_Pin, GPIO_PIN_SET);

    if (initState)
    {
        DFP_Interface::setVolume(INIT_VOLUME);
        this->numberOfTracks = DFP_Interface::getNumberOfTracksInFolder(FOLDER_MP3);
        this->currentTrack = 1;
    }
}

void DFP_Controller::turnOff()
{
    HAL_GPIO_WritePin(DFPlayer_On_Off_GPIO_Port, DFPlayer_On_Off_Pin, GPIO_PIN_RESET);
}

void DFP_Controller::init()
{
    DFP_Interface::initialize();
    DFP_Interface::setVolume(INIT_VOLUME);
    DFP_Interface::setEQ(INIT_EQUALIZER);

    // Premature state update
    this->volume = INIT_VOLUME;
    this->status = PlayingStatus::STATUS_STOPPED;
}

void DFP_Controller::startPlayer()
{
    this->numberOfTracks = DFP_Interface::getNumberOfTracksInFolder(FOLDER_MP3);

    // Start playing the first track in the folder containing the MP3 files
    DFP_Interface::playFolderLoop(FOLDER_MP3);

    // Wait until the player is actually playing
    while (DFP_Interface::getPlayingStatus() != PlayingStatus::STATUS_PLAYING)
        ;

    // Get the current (physical) track number s.t. we know the offset of this folder
    this->trackOffset = DFP_Interface::getCurrentTrack() - 1;

    // Premature state update
    this->status = PlayingStatus::STATUS_PLAYING;
    this->currentTrack = 1;
}

void DFP_Controller::playAlarm()
{
    this->numberOfTracks = DFP_Interface::getNumberOfTracksInFolder(FOLDER_ALARM);

    // Select random track from the alarm folder
    srand(HAL_GetTick());
    uint16_t selectedTrack = (rand() % this->numberOfTracks) + 1;

    // Play the selected track and wait until it is actually playing
    DFP_Interface::playTrackFromFolder_3000(FOLDER_ALARM, selectedTrack);
    while (DFP_Interface::getPlayingStatus() != PlayingStatus::STATUS_PLAYING)
        ;

    // Loop the selected alarm track
    DFP_Interface::loopCurrentTrack(true);

    // Premature state update
    this->status = PlayingStatus::STATUS_PLAYING;
    this->currentTrack = selectedTrack;
}

void DFP_Controller::stop()
{
    DFP_Interface::loopCurrentTrack(false);
    DFP_Interface::stop();

    // Premature state update
    this->status = PlayingStatus::STATUS_STOPPED;
}

void DFP_Controller::play()
{
    DFP_Interface::play();

    // Wait until the player is actually playing again
    while (DFP_Interface::getPlayingStatus() != PlayingStatus::STATUS_PLAYING)
        ;

    // Premature state update
    this->status = PlayingStatus::STATUS_PLAYING;
}

void DFP_Controller::pause()
{
    DFP_Interface::pause();

    // Wait until the player is actually paused
    while (DFP_Interface::getPlayingStatus() != PlayingStatus::STATUS_PAUSED)
        ;

    // Premature state update
    this->status = PlayingStatus::STATUS_PAUSED;
}

void DFP_Controller::togglePause()
{
    if (this->status == PlayingStatus::STATUS_PLAYING)
    {
        this->pause();
    }
    else
    {
        this->play();
    }
}

void DFP_Controller::next()
{
    DFP_Interface::next();

    // Wait until the player is actually playing again
    while (DFP_Interface::getPlayingStatus() != PlayingStatus::STATUS_PLAYING)
        ;

    // Premature state update
    this->currentTrack = (this->currentTrack == this->numberOfTracks) ? 1 : this->currentTrack + 1;
    this->status = PlayingStatus::STATUS_PLAYING;
}

void DFP_Controller::previous()
{
    DFP_Interface::previous();

    // Wait until the player is actually playing again
    while (DFP_Interface::getPlayingStatus() != PlayingStatus::STATUS_PLAYING)
        ;

    // Premature state update
    this->currentTrack = (this->currentTrack == 1) ? this->numberOfTracks : this->currentTrack - 1;
    this->status = PlayingStatus::STATUS_PLAYING;
}

void DFP_Controller::changeVolume(int8_t offset)
{
    int8_t newVolume = static_cast<int8_t>(this->volume) + offset;

    // Limit the volume to the range [0, 30]
    if (newVolume > 30)
    {
        newVolume = 30;
    }
    else if (newVolume < 0)
    {
        newVolume = 0;
    }

    DFP_Interface::setVolume(newVolume);

    // Premature state update
    this->volume = newVolume;
}

void DFP_Controller::getState(PlayingStatus& status, uint8_t& volume, uint16_t& numberOfTracks, uint16_t& currentTrack)
{
    // Update the state only if the last update was more than MIN_UPDATE_INTERVAL_* ms ago to save resources
    if (HAL_GetTick() - this->lastUpdateTrack > MIN_UPDATE_INTERVAL_TRACK)
    {
        this->currentTrack = DFP_Interface::getCurrentTrack() - this->trackOffset;
        this->lastUpdateTrack = HAL_GetTick();
    }

    if (HAL_GetTick() - this->lastUpdateOther > MIN_UPDATE_INTERVAL_OTHER)
    {
        this->status = DFP_Interface::getPlayingStatus();
        this->volume = DFP_Interface::getVolume();

        this->lastUpdateOther = HAL_GetTick();
    }

    status = this->status;
    volume = this->volume;
    numberOfTracks = this->numberOfTracks;
    currentTrack = this->currentTrack;
}
