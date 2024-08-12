#include <iostream>
#include "EmulatedSystem.h"
#include "HardwareEmulator.h"

int main(int argc, char** argv)
{   
    HardwareEmulator hardware;
    EmulatedSystem sys(&hardware);
    HardwareEmulator::sys = &sys;
    hardware.show();
    return Fl::run();
}
