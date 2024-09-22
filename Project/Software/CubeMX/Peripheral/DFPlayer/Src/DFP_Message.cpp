//
// Created by Tobias Henig on 21.09.24.
//

#include "DFP_Message.h"

DFP_Message::DFP_Message(MessageType type, uint16_t param, uint8_t feedback)
{
    _buffer[0] = START_BYTE;
    _buffer[VERSION_OFFSET] = VERSION_BYTE;
    _buffer[LENGTH_OFFSET] = LENGTH_BYTE;
    _buffer[CMD_OFFSET] = static_cast<uint8_t>(type);
    _buffer[FEEDBACK_OFFSET] = feedback;
    _buffer[PARAM_OFFSET] = param >> 8;
    _buffer[PARAM_OFFSET + 1] = param & 0xFF;

    uint16_t checksum = calculateChecksum();
    _buffer[CHECKSUM_OFFSET] = checksum >> 8;
    _buffer[CHECKSUM_OFFSET + 1] = checksum & 0xFF;

    _buffer[MSG_LENGTH - 1] = END_BYTE;
}

uint16_t DFP_Message::getParam() const
{
    return _buffer[PARAM_OFFSET] << 8 | _buffer[PARAM_OFFSET + 1];
}

MessageType DFP_Message::getType()
{
    return static_cast<MessageType>(_buffer[CMD_OFFSET]);
}

bool DFP_Message::isValid() const
{
    return calculateChecksum() == (_buffer[CHECKSUM_OFFSET] << 8 | _buffer[CHECKSUM_OFFSET + 1]);
}

uint8_t* DFP_Message::buffer()
{
    return _buffer;
}

uint16_t DFP_Message::calculateChecksum() const
{
    uint16_t checksum = 0;
    for (int i = VERSION_OFFSET; i < CHECKSUM_OFFSET; i++)
    {
        checksum += _buffer[i];
    }

    checksum = ~checksum + 1;

    return checksum;
}
