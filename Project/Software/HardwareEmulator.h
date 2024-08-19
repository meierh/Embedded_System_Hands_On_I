#ifndef HARDWAREEMULATOR_H
#define HARDWAREEMULATOR_H

#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Device.H>
#include <FL/Fl_Bitmap.H>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <chrono>
#include "DisplayItem.h"

constexpr int H = 128;
constexpr int W = 128;

class System;

class HardwareEmulator : public Fl_Double_Window
{
    public:
        HardwareEmulator();
        void setSystem(System* sys);
        void displayImage(std::vector<DisplayItem> items);
        
    private:
        void clearDisplay();
        Fl_Box display;
        
        Fl_Button btnMode;
        Fl_Button btnLeft;
        Fl_Button btnCenter;
        Fl_Button btnRight;
        
        Fl_Button btnClockwise;
        Fl_Button btnAnticlockwise;
        
        static void btnModeCallback(Fl_Widget* wid, long data);
        static void btnLeftCallback(Fl_Widget* wid, long data);
        static void btnCenterCallback(Fl_Widget* wid, long data);
        static void btnRightCallback(Fl_Widget* wid, long data);
        static void rotateCallback(Fl_Widget* wid, long data);
        
        static void idleCallback();
        
        static std::chrono::time_point<std::chrono::system_clock> time;
        static System* sys;        
};

#endif
