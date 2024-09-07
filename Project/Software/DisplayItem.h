#ifndef DISPLAYITEM_H
#define DISPLAYITEM_H

#include <string>
#include <cstdint>

class DisplayItem
{
    public:
        enum ItemType{Line,Text,Rectangle,Empty};
        enum Font{Font8=8,Font12=12,Font16=16,Font20=20,Font24=24};
        DisplayItem();
        DisplayItem(ItemType type);
        DisplayItem(int offsetH,int offsetW, Font size, std::string characters, std::uint8_t intensity);
        DisplayItem(int offsetH,int offsetW, int endH, int endW, std::uint8_t intensity);
        DisplayItem(int offsetH,int offsetW,std::pair<int,int> boxSize,std::uint8_t intensity,uint innerMult);
        
        ItemType getType() const {return type;}
        void setType(ItemType type) {this->type=type;}
        
        int offsetH;
        int offsetW;
        Font size;
        std::uint8_t intensity;
        
        //Type: Text
        std::string characters;
        
        //Type: Line
        int endH;
        int endW;
        
        //Type::Rectangle
        uint innerMult;
        
        std::string to_string();
        
    private:
        ItemType type;
};

#endif
