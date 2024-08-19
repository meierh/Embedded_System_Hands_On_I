#include "System.h"
#include "HardwareEmulator.h"

HardwareEmulator::HardwareEmulator
(
):
Fl_Double_Window(250,310,"Eieruhr"),
display(61,61,128,128),
btnMode(10, 110, 45,45,"Mode"),
btnLeft(40, 210, 50,45,"Left"),
btnCenter(100, 210,50,45,"Center"),
btnRight(160, 210, 50,45,"Right"),
btnClockwise(75,270, 50,30,"<<<"),
btnAnticlockwise(125,270, 50,30,">>>")
{       
    this->end();
    this->show();
    displayImage({});
    
    time = std::chrono::system_clock::now();
    
    
    btnMode.callback(&HardwareEmulator::btnModeCallback);
    btnLeft.callback(&HardwareEmulator::btnLeftCallback);
    btnCenter.callback(&HardwareEmulator::btnCenterCallback);
    btnRight.callback(&HardwareEmulator::btnRightCallback);
    
    btnClockwise.callback(&HardwareEmulator::rotateCallback);
    btnAnticlockwise.callback(&HardwareEmulator::rotateCallback);
    
    Fl::set_idle(&HardwareEmulator::idleCallback);

    //display.box(FL_UP_BOX);
    //display.labelfont(FL_BOLD + FL_ITALIC);
    //display.labelsize(36);
    //display.labeltype(FL_SHADOW_LABEL);
    std::cout<<"Setup HardwareEmulator"<<std::endl;
}

void HardwareEmulator::setSystem(System* sys)
{
    HardwareEmulator::sys=sys;
}

System* HardwareEmulator::sys=nullptr;
std::chrono::time_point<std::chrono::system_clock> HardwareEmulator::time = std::chrono::system_clock::now();

void HardwareEmulator::btnModeCallback(Fl_Widget* wid, long data)
{
    if(sys!=nullptr)
        sys->buttonModePress();
    else
        std::cout<<"Error: HardwareEmulator:49::btnModeCallback nullptr"<<std::endl;
}

void HardwareEmulator::btnLeftCallback(Fl_Widget* wid, long data)
{
    if(sys!=nullptr)
        sys->buttonLeftPress();
    else
        std::cout<<"Error: HardwareEmulator::btnLeftCallback nullptr"<<std::endl;
}

void HardwareEmulator::btnCenterCallback(Fl_Widget* wid, long data)
{
    if(sys!=nullptr)
        sys->buttonCenterPress();
    else
        std::cout<<"Error: HardwareEmulator::btnCenterCallback nullptr"<<std::endl;
}

void HardwareEmulator::btnRightCallback(Fl_Widget* wid, long data)
{
    if(sys!=nullptr)
        sys->buttonRightPress();
    else
        std::cout<<"Error: HardwareEmulator::btnRightCallback nullptr"<<std::endl;
}

void HardwareEmulator::rotateCallback(Fl_Widget* wid, long data)
{
    if(sys!=nullptr)
    {
        if(wid->label()=="<<<")
            sys->rotate(System::Direction::Clockwise);
        else if(wid->label()==">>>")
            sys->rotate(System::Direction::Counterclockwise);
        else
            std::cout<<"Error"<<std::endl;
    }
    else
        std::cout<<"Error: HardwareEmulator::rotateCallback nullptr"<<std::endl;
}

void HardwareEmulator::idleCallback()
{    
    if(sys!=nullptr)
    {
        auto timeNow = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow-time);
        if(duration.count() > 10)
        {
            time = std::chrono::system_clock::now();
            sys->periodElapsed();
        }
        sys->work();
    }
}

void HardwareEmulator::clearDisplay()
{
    auto img_surf = std::make_unique<Fl_Image_Surface>(W,H);
    Fl_Surface_Device::push_current(img_surf.get());
    fl_color(FL_WHITE);
    fl_rectf(0, 0, W, H);
    Fl_Surface_Device::pop_current();
    display.image(img_surf->image());
    display.redraw();
}

void HardwareEmulator::displayImage(std::vector<DisplayItem> items)
{
    clearDisplay();
    auto img_surf = std::make_unique<Fl_Image_Surface>(W,H);
    Fl_Surface_Device::push_current(img_surf.get());
    fl_color(FL_WHITE);
    fl_rectf(0, 0, W, H);
    for(const DisplayItem& item : items)
    {
        if(item.getType()==DisplayItem::ItemType::Text)
        {
            std::uint8_t intensity = item.intensity;
            int trunc_intensity = intensity/10;
            trunc_intensity = std::clamp(trunc_intensity,0,23);
            fl_color(FL_GRAY0+23-trunc_intensity);
            fl_font(FL_HELVETICA_BOLD, item.size);
            fl_draw(item.characters.c_str(),item.offsetW,item.offsetH);
        }
        else
            std::cout<<"Error"<<std::endl;
    }
    Fl_Surface_Device::pop_current();
    display.image(img_surf->image());
    display.redraw();
}
