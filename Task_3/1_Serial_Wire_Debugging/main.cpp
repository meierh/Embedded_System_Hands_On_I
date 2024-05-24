#include "SWD.h"

int main(int argc, char *argv[])
{
    SWD instance("Logfile",false);
    
    std::bitset<32> IDCODE;
    instance.readIDCODE(IDCODE);
    
    instance.write(AccessRegister::DebugPort,0b10,0x12345678);

    std::bitset<32> rData;
    instance.read(AccessRegister::DebugPort,0b11,rData);
}
