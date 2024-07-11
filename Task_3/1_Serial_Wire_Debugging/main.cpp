#include "SWD.h"

int main(int argc, char *argv[])
{
    SWD instance("Logfile",false);
    
    std::bitset<32> IDCODE;
    instance.readIDCODE(IDCODE);
    
    printf("IDCODE = %#010x\n", IDCODE.to_ulong());
}
