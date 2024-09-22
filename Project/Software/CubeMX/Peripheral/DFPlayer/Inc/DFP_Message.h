//
// Created by Tobias Henig on 21.09.24.
//

#ifndef DFP_MESSAGE_H
#define DFP_MESSAGE_H

#include "main.h"
#include "DFP_Definitions.h"
using namespace DFP;

class DFP_Message
{
public:
    /**
     * fixed bytes for all messages
     */
    static constexpr uint8_t START_BYTE = 0x7E;
    static constexpr uint8_t VERSION_BYTE = 0xFF;
    static constexpr uint8_t LENGTH_BYTE = 0x06;
    static constexpr uint8_t END_BYTE = 0xEF;

    /**
     * fixed message properties
     */
    static constexpr size_t MSG_LENGTH = 10;
    static constexpr size_t VERSION_OFFSET = 1;
    static constexpr size_t LENGTH_OFFSET = 2;
    static constexpr size_t CMD_OFFSET = 3;
    static constexpr size_t FEEDBACK_OFFSET = 4;
    static constexpr size_t PARAM_OFFSET = 5;
    static constexpr size_t CHECKSUM_OFFSET = 7;

    /**
     * Generates a messages with given command and parameter
     * @param type Type of the message (one of the MessageType enum)
     * @param param Parameter byte
     * @param feedback if set to 1, the DFPlayer will send an ACK message after receiving this message
     */
    DFP_Message(MessageType type, uint16_t param, uint8_t feedback);

    /**
     * Generates an empty message
     */
    DFP_Message() = default;

    /**
     * Destructor
     */
    ~DFP_Message() = default;

    /**
     * Returns the message's parameter
     * @return Parameter bytes
     */
    [[nodiscard]] uint16_t getParam() const;

    /**
     * Returns the type of the message
     * @return Type of the message (one of the MessageType enum)
     */
    MessageType getType();

    /**
     * Checks whether the checksum of the message is correct
     * @return true iff the checksum matches the message's version, length, command, feedback and parameter bytes
     */
    [[nodiscard]] bool isValid() const;

    /**
     * Returns a pointer to the message's buffer
     * This is used to let HAL functions write directly to the buffer without copying
     * @return A pointer to the buffer
     */
    uint8_t* buffer();

private:
    /**
     * The buffer containing the actual message
     */
    uint8_t _buffer[MSG_LENGTH]{};

    /**
     * Calculates the checksum of the message and writes it to the buffer
     * @return The calculated checksum based on the version, length, command, feedback and parameter bytes
     */
    [[nodiscard]] uint16_t calculateChecksum() const;
};

#endif //DFP_MESSAGE_H
