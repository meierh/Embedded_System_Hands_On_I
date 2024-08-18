#include <iostream>
#include "EmulatedSystem.h"
#include "HardwareEmulator.h"

int main(int argc, char** argv)
{   
    HardwareEmulator hardware;
    EmulatedSystem sys(&hardware);
    std::cout<<"del EmulatedSystem"<<std::endl;
    hardware.show();
    return Fl::run();
}

//#include "Example.h"
