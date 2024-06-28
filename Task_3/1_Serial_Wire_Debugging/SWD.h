#ifndef SWD_H
#define SWD_H

#include <bitset>
#include <fstream>
#include <vector>
#include <iostream>
#include <pigpio.h>
#include <iomanip>

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
        
        /* Creates SWD connection and runs setup
         * logName: File name of log 
         */
        SWD(std::string logName="", bool throwExcep = true);
        ~SWD();
        
        /* Writes data to Microcontroller
         * APnDP: Port type
         * A: Address
         * WDATA: Data paket 
         */
        ACK write(AccessRegister APnDP, std::bitset<2> A, std::bitset<32> WDATA);
        
        /* Reads data from Microcontroller
         * APnDP: Port type
         * A: Address
         * RDATA: Data paket 
         */
        ACK read(AccessRegister APnDP, std::bitset<2> A, std::bitset<32>& RDATA);
        
        // Read IDCODE from Microcontroller
        ACK readIDCODE(std::bitset<32>& IDCODE);
        
        // Runs setup sequence for SWD connection
        void reset();
        
    private:
        // Reads acknowledge sequence
        void request(std::bitset<8> req);
        
        // Reads acknowledge sequence
        ACK acknowledge();
        
        // Writes data paket sized 32 with an parity bit [data:(0::31),parity:(32)]
        void sendData(std::bitset<33> data);
        
        // Reads data paket sized 32 with an parity bit [data:(0::31),parity:(32)]
        void recvData(std::bitset<33>& data);
        
        // Writes empty bits for turnaround period
        void turnaround(uint cycles = TURNAROUND_PEROID);

        // Writes lineReset code to SWD
        void lineReset();
        
        // Writes JTAG code to SWD
        void JTAG_to_SWD();
        
        // Writes empty bit to SWD by toggling the clock without any data
        void empty();
        
        // Writes bit to SWD using SWCLK_GPIO and SWD_GPIO
        void writeBit(bool bit);
        
        // Reads bit from SWD using SWCLK_GPIO and SWD_GPIO
        void readBit(bool& bit);
        
        // Inverts clock by setting SWCLK from 0 to 1 or 1 to 0
        void toggleClock();
        
        // Computes even parity of vector of bool
        bool parity(const std::vector<bool>& bits);

        static const uint TURNAROUND_PEROID = 1;
        const uint SWCLK_GPIO = 5;
        const uint SWD_GPIO = 6;
        uint SWCLK_GPIO_LEVEL = 0;
        const uint DELAY = 1000;
        static bool activeInstance;
        const bool throwExcep;
        std::ofstream log;
}; 

#endif
