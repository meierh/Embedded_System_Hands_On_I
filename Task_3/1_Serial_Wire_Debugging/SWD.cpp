#include "SWD.h"

bool SWD::activeInstance = false;

SWD::SWD
(
    std::string logName,
    bool throwExcep
):
throwExcep(throwExcep)
{
    if(!activeInstance)
    {        
        activeInstance = true;
        log = std::ofstream(logName);        
        log<<"----------------------SWD setup----------------------"<<'\n';
        if (gpioInitialise() < 0)
        {
            log<<"------------------SWD setup failed------------------"<<'\n';
            throw std::logic_error("pigpio initialisation failed");        
        }
        else
        {
            if(gpioSetMode(SWCLK_GPIO, PI_OUTPUT) != 0)
                throw std::logic_error("SWCLK_GPIO mode setting failed");
            if(gpioSetMode(SWD_GPIO, PI_INPUT) != 0)
                throw std::logic_error("SWD_GPIO mode setting failed");
            gpioWrite(SWCLK_GPIO, SWCLK_GPIO_LEVEL);
            reset();
            log<<"-------------------SWD setup done--------------------"<<'\n';
        }
    }
    else
        throw std::logic_error("SWD must be a singleton");        
}

SWD::~SWD()
{
    gpioTerminate();
    activeInstance = false;
}

ACK SWD::write
(
    AccessRegister APnDP,
    std::bitset<2> A,
    std::bitset<32> WDATA
)
{
    writeBit(false); // insert an idle bit at the beginning s.t. the start bit can be recognised

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
    writeBit(false); // insert an idle bit at the beginning s.t. the start bit can be recognised

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
    for(std::size_t i=1; i<data.size(); i++)
    {
        bits.push_back(data[i]);
        RDATA[RDATA.size()-i]=data[i];
    }

    bool par = parity(bits);
    if(par!=data[0])
    {
        if(throwExcep)
            throw std::logic_error("Invalid read data");
        else
            log<<"Read data has invalid parity"<<'\n';
    }

    log<<" → Read: 0x"<<std::hex<<std::setw(8)<<std::setfill('0')<<RDATA.to_ullong()<<std::endl;
    
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
    if(gpioSetMode(SWD_GPIO, PI_OUTPUT) != 0)
        throw std::logic_error("SWD_GPIO mode setting for request failed");
    
    for(int i=0; i<req.size(); i++)
    {
        bool bit = req[i];
        writeBit(bit);
    }
    log<<"Request: "<<req.to_string()<<"  AP/DP:"<<req[1]<<" R/W:"<<req[2]<<" Address:"<<req[3]<<req[4]<<" Parity:"<<req[5]<<'\n';
}

ACK SWD::acknowledge()
{
    if(gpioSetMode(SWD_GPIO, PI_INPUT) != 0)
        throw std::logic_error("SWD_GPIO mode setting for acknowledgement failed");
    
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
    if(gpioSetMode(SWD_GPIO, PI_INPUT) != 0)
        throw std::logic_error("SWD_GPIO mode setting for sending failed");
    
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
    if(gpioSetMode(SWD_GPIO, PI_INPUT) != 0)
        throw std::logic_error("SWD_GPIO mode setting for receiving failed");
    
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
    if (SWCLK_GPIO_LEVEL == 1)
        toggleClock();
    
    if (gpioGetMode(SWD_GPIO) == PI_INPUT)
        gpioSetMode(SWD_GPIO, PI_OUTPUT);
    else
        gpioSetMode(SWD_GPIO, PI_INPUT);

    for(int i=0; i<cycles; i++)
        empty();
}

void SWD::lineReset()
{
    if(gpioSetMode(SWD_GPIO, PI_OUTPUT) != 0)
        throw std::logic_error("SWD_GPIO mode setting for lineReset failed");
    for(int i=0; i<51; i++)
        writeBit(true);
    writeBit(false);
    log<<"Line Reset"<<'\n';
}

void SWD::JTAG_to_SWD()
{
    if(gpioSetMode(SWD_GPIO, PI_OUTPUT) != 0)
        throw std::logic_error("SWD_GPIO mode setting for JTAG_to_SWD failed");
    std::bitset<16> jtag_to_swd_sequence = 0x79e7;
    for(int i=jtag_to_swd_sequence.size()-1; i>=0; i--)
    {
        bool bit = jtag_to_swd_sequence[i];
        writeBit(bit);
    }
    log<<"JTAG_to_SWD"<<'\n';
}

void SWD::empty()
{
    toggleClock();
    toggleClock();
}

void SWD::writeBit(bool bit)
{
    if(SWCLK_GPIO_LEVEL==0)
        toggleClock();
    
    int level = bit;
    if(gpioWrite(SWD_GPIO,level) != 0) // Write bit to pin
        throw std::logic_error("SWD_GPIO write failed");

    toggleClock();
}

void SWD::readBit(bool& bit)
{
    if(SWCLK_GPIO_LEVEL==0)
        toggleClock();
    
    int level = gpioRead(SWD_GPIO);    // Write bit to pin
    bit = level;
    toggleClock();
}

void SWD::toggleClock()
{
    //gpioDelay(DELAY);
    if(SWCLK_GPIO_LEVEL==0)
    {
        if(gpioWrite(SWCLK_GPIO,1) != 0) // Write high to clock
            throw std::logic_error("SWCLK_GPIO write failed");
        SWCLK_GPIO_LEVEL=1;
    }
    else
    {
        if(gpioWrite(SWCLK_GPIO,0) != 0) // Write low to clock
            throw std::logic_error("SWCLK_GPIO write failed");
        SWCLK_GPIO_LEVEL=0;
    }
    gpioDelay(DELAY);
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
