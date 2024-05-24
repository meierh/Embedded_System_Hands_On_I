#ifndef SWD_H
#define SWD_H

#include <bitset>
#include <fstream>
#include <vector>
#include <iostream>
#include <wiringPi.h>

enum AccessRegister
{
    DebugPort = 0,
    AccessPort = 1
};

enum ACK
{
    OK = 0x100,
    WAIT = 0x010,
    FAULT = 0x001
};

class SWD
{
    public:
        SWD(std::string logName="", bool throwExcep = true);
        
        ACK write(AccessRegister APnDP, std::bitset<2> A, std::bitset<32> WDATA);
        ACK read(AccessRegister APnDP, std::bitset<2> A, std::bitset<32>& RDATA);
        ACK readIDCODE(std::bitset<32>& IDCODE);
        void reset();
        
    private:
        void request(std::bitset<8> req);
        ACK acknowledge();
        void sendData(std::bitset<33> data);
        void recvData(std::bitset<33>& data);
        void turnaround(uint cycles = TURNAROUND_PEROID);

        void lineReset();
        void JTAG_to_SWD();
        
        void empty();
        void writeBit(bool bit);
        void readBit(bool& bit);
        
        bool parity(const std::vector<bool>& bits);

        static const uint TURNAROUND_PEROID = 1;
        const bool throwExcep;
        std::ofstream log;

}; 

#endif
