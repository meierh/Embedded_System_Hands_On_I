#include "HardwareEmulator.h"

HardwareEmulator::HardwareEmulator
(
):
Fl_Window(250,310,"Eieruhr"),
display(61,61,128,128),
btn1( 30, 210, 40,40,"Btn1"),
btn2( 80, 210, 40,40,"Btn2"),
btn3(130, 210, 40,40,"Btn3"),
btn4(180, 210, 40,40,"Btn4"),
btnClockwise(75,270, 50,30,"<<<"),
btnAnticlockwise(125,270, 50,30,">>>")
{
    display.box(FL_UP_BOX);
    display.labelfont(FL_BOLD + FL_ITALIC);
    display.labelsize(36);
    display.labeltype(FL_SHADOW_LABEL);
    display.color(0xffffff00);
    
    btn1.callback(&HardwareEmulator::btn1Callback);
    btn2.callback(&HardwareEmulator::btn2Callback);
    btn3.callback(&HardwareEmulator::btn3Callback);
    btn4.callback(&HardwareEmulator::btn4Callback);
    
    btnClockwise.callback(&HardwareEmulator::rotateCallback);
    btnAnticlockwise.callback(&HardwareEmulator::rotateCallback);
    
    Fl::add_idle(&HardwareEmulator::idleCallback);
}

System* HardwareEmulator::sys = nullptr;

void HardwareEmulator::displayString
(
    std::tuple<std::uint8_t,std::uint8_t,System::Font,std::string> string
)
{
    std::cout<<std::get<3>(string)<<std::endl;
    char bits[H*W];
    for(uint px=0; px<H*W; px++)
        bits[px] = std::get<0>(string);
    //Fl_Bitmap background(bits,W,H);   
    
    #include "logo.xpm"
    Fl_Pixmap background{logo_xpm};
    display.image(&background);    
}

void HardwareEmulator::btn1Callback(Fl_Widget* wid, long data)
{
    sys->button1Press();
}
void HardwareEmulator::btn2Callback(Fl_Widget* wid, long data)
{
    sys->button2Press();
}
void HardwareEmulator::btn3Callback(Fl_Widget* wid, long data)
{
    sys->button3Press();
}
void HardwareEmulator::btn4Callback(Fl_Widget* wid, long data)
{
    sys->button4Press();
}
void HardwareEmulator::rotateCallback(Fl_Widget* wid, long data)
{
    if(wid->label()=="<<<")
        sys->rotate(System::Direction::Clockwise);
    else if(wid->label()==">>>")
        sys->rotate(System::Direction::Counterclockwise);
    else
        std::cout<<"Error"<<std::endl;
}

void HardwareEmulator::idleCallback(void* data)
{
    sys->work();
}
