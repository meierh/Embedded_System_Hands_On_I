//
// Created by Tobias Henig on 21.09.24.
//

#include "DFP_Interface.h"

extern UART_HandleTypeDef DF_UART;


void DFP_Interface::sendMessage(DFP_Message message)
{
    HAL_UART_Transmit(&DF_UART, message.buffer(), DFP_Message::MSG_LENGTH, HAL_MAX_DELAY);
}

bool DFP_Interface::receiveMessage(DFP_Message& received_message)
{
    // Receive start byte
    HAL_UART_Receive(&DF_UART, received_message.buffer(), 1, HAL_MAX_DELAY);

    // Receive version byte until the version byte is received
    // This is necessary because if the previous data from the DFPlayer were not read, the start byte is sent twice with for next message
    while (received_message.buffer()[DFP_Message::VERSION_OFFSET] != DFP_Message::VERSION_BYTE)
    {
        HAL_UART_Receive(&DF_UART, received_message.buffer() + DFP_Message::VERSION_OFFSET, 1, HAL_MAX_DELAY);
    }

    // Receive remaining bytes
    HAL_UART_Receive(&DF_UART, received_message.buffer() + DFP_Message::VERSION_OFFSET + 1, DFP_Message::MSG_LENGTH - DFP_Message::VERSION_OFFSET - 1, HAL_MAX_DELAY);

    // Check if message is valid
    return received_message.isValid() && received_message.getType() != MessageType::RETURNED_ERROR;
}

void DFP_Interface::sendAndACKData(MessageType command, uint16_t commandParameter)
{
    for (int i = 0; i <= MAX_RETRIES; ++i)
    {
        sendMessage(DFP_Message(command, commandParameter, 1));
        DFP_Message received_message;

        receiveMessage(received_message);
        if (received_message.getType() == MessageType::RETURNED_ACK)
        {
            // if ACK was received, the message was successfully sent
            return;
        }
    }
    Error_Handler();
}

uint16_t DFP_Interface::queryAndGetData(MessageType command, uint16_t commandParameter)
{
    for (int i = 0; i <= MAX_RETRIES; ++i)
    {
        sendMessage(DFP_Message(command, commandParameter, 0));
        DFP_Message received_message;

        // if received message contains valid data, return the parameter
        if (receiveMessage(received_message))
        {
            return received_message.getParam();
        }
    }
    Error_Handler();
    return 0;
}

void DFP_Interface::initialize()
{
    uint16_t data = queryAndGetData(MessageType::QUERY_ONLINE_DEVICE, 2);

    // if SD card is not online, throw an error
    if (data != 2)
    {
        Error_Handler();
    }
}

void DFP_Interface::reset()
{
    sendAndACKData(MessageType::RESET, 0);
}

void DFP_Interface::setEQ(EqualizerType eq)
{
    sendAndACKData(MessageType::SET_EQ, static_cast<uint16_t>(eq));
}

void DFP_Interface::play()
{
    sendAndACKData(MessageType::SET_PLAY, 0);
}

void DFP_Interface::pause()
{
    sendAndACKData(MessageType::SET_PAUSE, 0);
}

void DFP_Interface::stop()
{
    sendAndACKData(MessageType::SET_STOP, 0);
}

void DFP_Interface::next()
{
    sendAndACKData(MessageType::SET_PLAY_NEXT, 0);
}

void DFP_Interface::previous()
{
    sendAndACKData(MessageType::SET_PLAY_PREV, 0);
}

void DFP_Interface::playTrackFromFolder_255(uint8_t folder, uint8_t track)
{
    sendAndACKData(MessageType::SET_PLAY_FOLDER, (folder << 8) | track);
}

void DFP_Interface::playTrackFromFolder_3000(uint8_t folder, uint16_t track)
{
    sendAndACKData(MessageType::SET_PLAY_FOLDER_EXTENDED, (folder << 12) | track);
}

void DFP_Interface::playTrackFromMP3Folder(uint16_t track)
{
    sendAndACKData(MessageType::SET_PLAY_MP3_FOLDER, track);
}

void DFP_Interface::playFolderLoop(uint8_t folder)
{
    sendAndACKData(MessageType::SET_FOLDER_REPEAT, folder);
}

void DFP_Interface::playSingleTrackLoop(uint16_t physicalTrack)
{
    sendAndACKData(MessageType::SET_CURRENT_REPEAT, physicalTrack);
}

void DFP_Interface::loopCurrentTrack(bool enabled)
{
    uint16_t enabled_flipped = !enabled;
    sendAndACKData(MessageType::SET_CURRENT_REPEAT, enabled_flipped);
}

void DFP_Interface::volumeUp()
{
    sendAndACKData(MessageType::SET_VOLUME_UP, 0);
}

void DFP_Interface::volumeDown()
{
    sendAndACKData(MessageType::SET_VOLUME_DOWN, 0);
}

void DFP_Interface::setVolume(uint8_t volume)
{
    sendAndACKData(MessageType::SET_VOLUME, volume);
}

bool DFP_Interface::isActive()
{
    uint16_t data = queryAndGetData(MessageType::QUERY_STATUS, 0);
    return static_cast<ActiveStatus>(data >> 8) == ActiveStatus::STATUS_ACTIVE;
}

PlayingStatus DFP_Interface::getPlayingStatus()
{
    uint16_t data = queryAndGetData(MessageType::QUERY_STATUS, 0);
    return static_cast<PlayingStatus>(data & 0xFF);
}

uint8_t DFP_Interface::getVolume()
{
    uint16_t data = queryAndGetData(MessageType::QUERY_VOLUME, 0);
    return data & 0xFF;
}

EqualizerType DFP_Interface::getEQ()
{
    uint16_t data = queryAndGetData(MessageType::QUERY_EQ, 0);
    return static_cast<EqualizerType>(data & 0xFF);
}

uint16_t DFP_Interface::getNumberOfTracksInFolder(uint8_t folder)
{
    return queryAndGetData(MessageType::QUERY_TRACKS_IN_FOLDER, folder);
}

uint16_t DFP_Interface::getNumberOfTracksTotal()
{
    return queryAndGetData(MessageType::QUERY_TRACKS_COUNT_SD, 0);
}

uint8_t DFP_Interface::getNumberOfFolders()
{
    uint16_t data =  queryAndGetData(MessageType::QUERY_FOLDERS, 0);
    return data & 0xFF;
}

uint16_t DFP_Interface::getCurrentTrack()
{
    return queryAndGetData(MessageType::QUERY_CURRENT_TRACK_SD, 0);
}
