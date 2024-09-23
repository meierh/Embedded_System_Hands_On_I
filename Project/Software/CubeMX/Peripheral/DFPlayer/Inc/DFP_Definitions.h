//
// Created by Tobias Henig on 22.09.24.
//

#ifndef DFP_DEFINITIONS_H
#define DFP_DEFINITIONS_H

#include "main.h"


namespace DFP {
    /**
     * Available message types regarding the datasheet of the DFPlayer Mini
     */
    enum class MessageType : uint8_t
    {
        RESET = 0x0C,

        SET_AUDIO_AMPLIFICATION = 0x10,
        SET_DAC_STATE = 0x1A,
        SET_DEVICE = 0x09,

        SET_PLAY = 0x0D,
        SET_PAUSE = 0x0E,
        SET_STOP = 0x16,
        SET_PLAY_NEXT = 0x01,
        SET_PLAY_PREV = 0x02,

        SET_PLAY_TRACK_ROOT = 0x03,
        SET_PLAY_FOLDER = 0x0F, // Play one of track 001-255 in folder 01-99
        SET_PLAY_FOLDER_EXTENDED = 0x14, // Play one of track 001-3000 in folder 01-15
        SET_PLAY_MP3_FOLDER = 0x12,

        SET_VOLUME_UP = 0x04,
        SET_VOLUME_DOWN = 0x05,
        SET_VOLUME = 0x06,

        SET_EQ = 0x07,

        SET_PLAY_REPEAT = 0x08,
        SET_ALL_REPEAT = 0x11,
        SET_FOLDER_REPEAT = 0x17,
        SET_CURRENT_REPEAT = 0x19,
        SET_RANDOM = 0x18,

        SET_SLEEP = 0x0A,

        SET_ADVERT_PLAY = 0x13,
        SET_ADVERT_STOP = 0x15,


        QUERY_ONLINE_DEVICE = 0x3F,
        QUERY_STATUS = 0x42,
        QUERY_VOLUME = 0x43,
        QUERY_EQ = 0x44,

        QUERY_TRACKS_COUNT_USB = 0x47,
        QUERY_TRACKS_COUNT_SD = 0x48,
        QUERY_TRACKS_IN_FOLDER = 0x4E,
        QUERY_CURRENT_TRACK_USB = 0x4B,
        QUERY_CURRENT_TRACK_SD = 0x4C,
        QUERY_FOLDERS = 0x4F,

        RETURNED_ERROR = 0x40,
        RETURNED_ACK = 0x41,
    };

    /**
     * Available equalizer types regarding the datasheet of the DFPlayer Mini
     */
    enum class EqualizerType : uint8_t
    {
        NORMAL = 0,
        POP = 1,
        ROCK = 2,
        JAZZ = 3,
        CLASSIC = 4,
        BASS = 5,
    };

    /**
     * Possible status bytes sent by the DFPlayer Mini
     */
    enum class ActiveStatus : uint8_t
    {
        STATUS_ACTIVE = 0x02,
        STATUS_SLEEPING = 0x10
    };

    /**
     * Possible status bytes sent by the DFPlayer Mini
     */
    enum class PlayingStatus : uint8_t
    {
        STATUS_STOPPED = 0x00,
        STATUS_PLAYING = 0x01,
        STATUS_PAUSED = 0x02,
    };
}

#endif //DFP_DEFINITIONS_H
