#ifndef DISPLAYITEM_H
#define DISPLAYITEM_H

#include <string>

class DisplayItem
{
    public:
        enum ItemType{Line,Text,Rectangle,Empty};
        DisplayItem();
        DisplayItem(ItemType type);
        DisplayItem(int offsetH,int offsetW, int size, std::string characters, std::uint8_t intensity);
        DisplayItem(int offsetH,int offsetW, int endH, int endW, std::uint8_t intensity);
        DisplayItem(int offsetH,int offsetW,std::pair<uint,uint> boxSize,std::uint8_t intensity);
        
        ItemType getType() const {return type;}
        void setType(ItemType type) {this->type=type;}
        
        int offsetH;
        int offsetW;
        int size;
        std::uint8_t intensity;
        
        //Type: Text
        std::string characters;
        
        //Type: Line
        int endH;
        int endW;
        
        std::string to_string();
        
    private:
        ItemType type;
};

#endif
