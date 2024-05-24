#include "SWD.h"

SWD::SWD
(
    std::string logName,
    bool throwExcep
):
throwExcep(throwExcep)
{
    log = std::ofstream(logName);
    log<<"----------------------SWD setup----------------------"<<'\n';
    reset();
    log<<"-------------------SWD setup done--------------------"<<'\n';
}

ACK SWD::write
(
    AccessRegister APnDP,
    std::bitset<2> A,
    std::bitset<32> WDATA
)
{
    std::bitset<8> req;
    req[0] = true;                          // Start bit
    req[1] = APnDP==AccessRegister::DebugPort
                             ? 0b0 : 0b1;   // AP / DP bit
    req[2] = 0b0;                           // Read / Write bit
    req[3] = A[0];                          // Address bit 2
    req[4] = A[1];                          // Address bit 3
    req[5] = parity({req[1],req[2],
                     req[3],req[4]});       // Parity bit
    req[6] = 0b0;                           // Stop bit
    req[7] = 0b1;                           // Park bit
    request(req);
    
    turnaround();
    
    ACK acknowledgement = acknowledge();
    
    turnaround();

    if(acknowledgement!=ACK::OK)
    {
        return acknowledgement;
    }
    
    std::bitset<33> data;
    std::vector<bool> bits;
    for(std::size_t i=0; i<data.size()-1; i++)
    {
        bits.push_back(WDATA[i]);
        data[i] = WDATA[i];
    }
    data[32] = parity(bits);
    sendData(data);
    
    return acknowledgement;
}

ACK SWD::read
(
    AccessRegister APnDP,
    std::bitset<2> A,
    std::bitset<32>& RDATA
)
{
    std::bitset<8> req;
    req[0] = true;                          // Start bit
    req[1] = APnDP==AccessRegister::DebugPort
                             ? 0b0 : 0b1;   // AP / DP bit
    req[2] = 0b1;                           // Read / Write bit
    req[3] = A[0];                          // Address bit 2
    req[4] = A[1];                          // Address bit 3
    req[5] = parity({req[1],req[2],
                     req[3],req[4]});       // Parity bit
    req[6] = 0b0;                           // Stop bit
    req[7] = 0b1;                           // Park bit
    request(req);
    
    turnaround();
    
    ACK acknowledgement = acknowledge();

    if(acknowledgement!=ACK::OK)
    {
        turnaround();
        return acknowledgement;
    }
       
    std::bitset<33> data;
    recvData(data);
    std::vector<bool> bits;
    for(std::size_t i=0; i<data.size()-1; i++)
    {
        bits.push_back(data[i]);
        RDATA[i] = data[i];
    }
    bool par = parity(bits);
    
    if(par==data[32])
    {
        if(throwExcep)
            throw std::logic_error("Invalid read data");
        else
            log<<"Read data has invalid parity"<<'\n';
    }
    
    return acknowledgement;
}

ACK SWD::readIDCODE
(
    std::bitset<32>& IDCODE
)
{
    return read(AccessRegister::DebugPort,{0b00},IDCODE);
}

void SWD::reset()
{
    lineReset();
    JTAG_to_SWD();
    lineReset();
    std::bitset<32> IDCODE;
    readIDCODE(IDCODE);
}

void SWD::request
(
    std::bitset<8> req
)
{
    for(int i=req.size()-1; i>=0; i--)
    {
        bool bit = req[i];
        writeBit(bit);
    }
    log<<"Request: "<<req.to_string()<<"  AP/DP:"<<req[1]<<" R/W:"<<req[2]<<" Address:"<<req[3]<<req[4]<<" Parity:"<<req[5]<<'\n';
}

ACK SWD::acknowledge()
{
    std::bitset<3> ack;
    for(int i=ack.size()-1; i>=0; i--)
    {
        bool bit;
        readBit(bit);
        ack[i] = bit;
    }
    log<<"Acknowledge: "<<ack.to_string()<<'\n';
    
    if(ack==0b100)
        return ACK::OK;
    else if(ack==0b010)
        return ACK::WAIT;
    else if(ack==0b001)
        return ACK::FAULT;
    else
    {
        if(throwExcep)
            throw std::logic_error("Invalid ACK sequence received");
        else
            log<<"Invalid ACK sequence received"<<'\n';
    }
    return ACK::FAULT;
}

void SWD::sendData
(
    std::bitset<33> data
)
{
    for(int i=data.size()-1; i>=0; i--)
    {
        bool bit = data[i];
        writeBit(bit);
    }
    std::string dataString = data.to_string();
    dataString.pop_back();
    log<<"Send: "<<dataString<<"  Parity:"<<data[data.size()-1]<<'\n';
}

void SWD::recvData
(
    std::bitset<33>& data
)
{
    for(int i=data.size()-1; i>=0; i--)
    {
        bool bit;
        readBit(bit);
        data[i] = bit;
    }
    std::string dataString = data.to_string();
    dataString.pop_back();
    log<<"Receive: "<<dataString<<"  Parity:"<<data[data.size()-1]<<'\n';
}

void SWD::turnaround
(
    uint cycles
)
{
    for(int i=0; i<cycles; i++)
        empty();
}

void SWD::lineReset()
{
    for(int i=0; i<51; i++)
        writeBit(true);
    log<<"Line Reset"<<'\n';
}

void SWD::JTAG_to_SWD()
{
    
    log<<"JTAG_to_SWD"<<'\n';
}

void SWD::empty()
{
    // Clock -> 1 toggleClock
    // Clock -> 0 toggleClock
}

void SWD::writeBit(bool bit)
{
    // Write bit to pin
    // Clock -> 1 toggleClock
    // Clock -> 0 toggleClock
}

void SWD::readBit(bool& bit)
{
    // Clock -> 1 toggleClock()
    // Read bit from pin
    // Clock -> 0 toggleClock
}

void SWD::toggleClock()
{
    // delay(...)
    /* if (clock=0)
     *  clock = 1
     * else
     *  clock = 0;
     */
    // delay(...)
}

bool SWD::parity
(
    const std::vector<bool>& bits
)
{
    uint numberBitsSet = 0;
    for(int i=0; i<bits.size(); i++)
        if(bits[i])
            numberBitsSet++;
    return numberBitsSet%2;
}
