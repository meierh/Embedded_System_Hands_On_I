#include "UART.h"

std::unique_ptr<std::vector<std::bitset<FRAME_SIZE>>> uart_send(const std::vector<bool>& data)
{
    auto resultPtr = std::make_unique<std::vector<std::bitset<FRAME_SIZE>>>();
    std::vector<std::bitset<FRAME_SIZE>>& 
    for(uint offset=0; offset<data.size(); offset+=FRAME_SIZE)
    {
        std::bitset<FRAME_SIZE> frame = 0;
        uint dataInd=offset;
        for(uint frameInd=0; frameInd<FRAME_SIZE && dataInd<data.size(); frameInd++,dataInd++)
        {
            frame[frameInd] = data[dataInd];
        }
        uart_frame_send(frame);
    }
}

void uart_receive(std::vector<bool>& data, uint size)
{
    for(uint offset=0; offset<size; offset+=FRAMSE_SIZE)
    {
        std::bitset<FRAMSE_SIZE> frame = 0;
        uint dataInd=offset;
        for(uint frameInd=0; frameInd<FRAMSE_SIZE && dataInd<data.size(); frameInd++,dataInd++)
        {
            frame[frameInd] = data[dataInd];
        }
        uart_frame_send(frame);
    }
}

void uart_frame_send(const std::bitset<FRAME_SIZE>& frame)
{
    std::bitset<FRAME_SIZE+3> fullFrameBits;
    fullFrameBits[0] = 0;
    for(uint frameInd=0; frameInd<frame.size(); frameInd++)
        fullFrameBits[frameInd+1] = frame[frameInd];
    fullFrameBits[FRAME_SIZE+2] = 0;
    fullFrameBits[FRAME_SIZE+1] = partiy(frame);
    
    
}

void uart_frame_receive(std::bitset<FRAME_SIZE>& frame)
{
    
}

bool partiy(const std::bitset<FRAME_SIZE>& frame)
{
    uint numberBitsSet = 0;
    for(int i=0; i<frame.size(); i++)
        if(bits[i])
            numberBitsSet++;
    return numberBitsSet%2;
}
