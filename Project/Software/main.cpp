#include <iostream>
#include "System.h"
#include "HardwareEmulator.h"

int main(int argc, char** argv)
{   
    System sys;
    HardwareEmulator hardware;
    HardwareEmulator::sys = &sys;
    hardware.show();
    return Fl::run();
}
