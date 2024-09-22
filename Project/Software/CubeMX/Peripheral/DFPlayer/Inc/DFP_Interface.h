//
// Created by Tobias Henig on 21.09.24.
//

#ifndef DFP_INTERFACE_H
#define DFP_INTERFACE_H

#include "DFP_Message.h"
#include "main.h"
#include "DFP_Definitions.h"

using namespace DFP;

#define DF_UART huart2


class DFP_Interface {
public:
    /** Implementations of control functions for the DFPlayer Mini **/
    static void initialize();

    static void reset();
    static void setEQ(EqualizerType eq);

    static void play();
    static void pause();
    static void stop();
    static void next();
    static void previous();

    static void playTrackFromFolder_255(uint8_t folder, uint8_t track); // supports playing tracks 1..255 from folders 1..99
    static void playTrackFromFolder_3000(uint8_t folder, uint16_t track); // supports playing tracks 1..3000 from folders 1..15
    static void playTrackFromMP3Folder(uint16_t track);
    static void playFolderLoop(uint8_t folder);
    static void playSingleTrackLoop(uint16_t physicalTrack); // physical track means the order of the tracks on the SD card

    static void loopCurrentTrack(bool enabled); // to enable this, the track must currently be playing (status == 0x0201)

    static void volumeUp();
    static void volumeDown();
    static void setVolume(uint8_t volume);

    static bool isActive();
    static PlayingStatus getPlayingStatus();

    static uint8_t getVolume();
    static EqualizerType getEQ();

    static uint16_t getNumberOfTracksInFolder(uint8_t folder);
    static uint16_t getNumberOfTracksTotal();
    static uint8_t getNumberOfFolders();
    static uint16_t getCurrentTrack();

private:
    /** Basic UART communication functions for the DFPlayer Mini **/

    /**
     * Send a message to the DFPlayer via UART
     * @param message the message to send
     */
    static void sendMessage(DFP_Message message);

    /**
     * Receive a message from the DFPlayer via UART
     * @param received_message reference to the message object to store the received message
     * @return true iff a valid message (no error, valid checksum) was received
     */
    static bool receiveMessage(DFP_Message& received_message);


    /** Communication flow handling **/

    /**
     * Number of retries after an invalid ACK or an invalid response
     */
    static constexpr uint8_t MAX_RETRIES = 3;

    /**
     * Send data to the DFPlayer Mini and wait for an ACK message.
     * Waiting for an ACK message is good to ensure that the DFPlayer Mini is ready to receive the next message.
     * - sends a message with the given command and parameter
     * - waits for the response
     * - if the response is an ACK message, returns
     * - if the response is an invalid message, calls Error_Handler()
     * @param command the type of the message to send
     * @param commandParameter the parameter of the message to send
     */
    static void sendAndACKData(MessageType command, uint16_t commandParameter);

    /**
     * Read data from the DFPlayer Mini:
     * - sends a query message with the given command and parameter
     * - waits for the response
     * - if the response is valid, returns the parameter of the response
     * - if the response is invalid, calls Error_Handler()
     * @param command the type of the query message
     * @param commandParameter the parameter of the query message
     * @return the parameter of the response message
     */
    static uint16_t queryAndGetData(MessageType command, uint16_t commandParameter);
};

#endif //DFP_INTERFACE_H
