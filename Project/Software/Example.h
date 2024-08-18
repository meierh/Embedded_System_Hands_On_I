#include <FL/Enumerations.H>
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Device.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/fl_draw.H>
#include <cassert>
#include <vector>

Fl_RGB_Image *get_image(int w, int h) {
    // draw image on surface
    auto img_surf = new Fl_Image_Surface(w, h);
    Fl_Surface_Device::push_current(img_surf);
    // We'll use white to mask 255, 255, 255, see the loop
    fl_color(FL_WHITE);
    fl_rectf(0, 0, w, h);
    fl_color(FL_BLACK);
    fl_font(FL_HELVETICA_BOLD, 20);
    fl_draw("Hello World", 50, 50);
    auto image = img_surf->image();
    delete img_surf;
    Fl_Surface_Device::pop_current();
    return image;
}

Fl_RGB_Image *get_transparent_image(const Fl_RGB_Image *image) {
    assert(image);
    // make image transparent
    auto data = (const unsigned char*)(*image->data());
    auto len = image->w() * image->h() * image->d(); // the depth is by default 3
    std::vector<unsigned char> temp;
    for (size_t i = 0; i < len; i++) {
        if (i > 0 && i % 3 == 0) {
            // check if the last 3 vals are the rgb values of white, add a 0 alpha
            if (data[i] == 255 && data[i - 1] == 255 && data[i - 2] == 255)
                temp.push_back(0);
            else
                // add a 255 alpha, making the black opaque
                temp.push_back(255);
            temp.push_back(data[i]);
        } else {
            temp.push_back(data[i]);
        }
    }
    temp.push_back(0);
    assert(temp.size() == image->w() * image->h() * 4);
    auto new_image_data = new unsigned char[image->w() * image->h() * 4];
    memcpy(new_image_data, temp.data(), image->w() * image->h() * 4);
    auto new_image = new Fl_RGB_Image(new_image_data, image->w(), image->h(), 4); // account for alpha
    return new_image;
}

int main() {
    class Window : public Fl_Double_Window
    {
        public:
            Window():
            Fl_Double_Window(400,300),
            box(100, 100, 200, 100),
            btnClockwise(0,0,20,20,"ABC")
            {
                this->end();
                this->show();
                auto image = get_image(box.w(), box.h());
                box.image(image);
                box.redraw();
            }
            Fl_Box box;
            Fl_Button btnClockwise;

    };
    
    //auto win = new Fl_Double_Window(400, 300);
    //auto box = new Fl_Box(0, 0, 400, 300);
    //win->end();
    //win->show();
    Window win;
    
    //auto transparent_image = get_transparent_image(image);
    //delete image;

    return Fl::run();
}
