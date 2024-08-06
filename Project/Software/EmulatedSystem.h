#ifndef EMULATEDSYSTEM_H
#define EMULATEDSYSTEM_H
#include "System.h"

class EmulatedSystem : public System
{
    public:
        //System -> Application
        void button1Press() override;
        void button2Press() override;
        void button3Press() override;
        void button4Press() override;
        void rotate(Direction dir) override;
        
        //Application -> System
};
#endif
