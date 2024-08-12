#ifndef EMULATEDSYSTEM_H
#define EMULATEDSYSTEM_H
#include "System.h"
#include "HardwareEmulator.h"

class EmulatedSystem : public System
{
    public:
        EmulatedSystem(HardwareEmulator* hardware):hardware(hardware){}
        void work();
        
        //System -> Application
        void button1Press();
        void button2Press();
        void button3Press();
        void button4Press();
        void rotate(Direction dir);
        
        //Application -> System
        void displayString(std::tuple<std::uint8_t,std::uint8_t,System::Font,std::string> string);
        
    private:
        HardwareEmulator* hardware;
};
#endif
