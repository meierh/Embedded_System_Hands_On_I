#ifndef HARDWAREEMULATOR_H
#define HARDWAREEMULATOR_H

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Bitmap.H>

#include <iostream>
#include "System.h"

constexpr int H = 128;
constexpr int W = 128;

class HardwareEmulator : public Fl_Window
{
    public:
        HardwareEmulator();
        static System* sys;
    private:
        
        Fl_Box display;
        
        Fl_Button btn1;
        Fl_Button btn2;
        Fl_Button btn3;
        Fl_Button btn4;
        
        Fl_Button btnClockwise;
        Fl_Button btnAnticlockwise;
        
        static void btn1Callback(Fl_Widget* wid, long data);
        static void btn2Callback(Fl_Widget* wid, long data);
        static void btn3Callback(Fl_Widget* wid, long data);
        static void btn4Callback(Fl_Widget* wid, long data);
        static void rotateCallback(Fl_Widget* wid, long data);
        
        static void idleCallback();
};

#endif
