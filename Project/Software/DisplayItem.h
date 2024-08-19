#ifndef DISPLAYITEM_H
#define DISPLAYITEM_H

#include <string>

class DisplayItem
{
    public:
        enum ItemType{Text,Empty};
        DisplayItem();
        DisplayItem(ItemType type);
        DisplayItem(int offsetH,int offsetW, int size, std::string characters, std::uint8_t intensity);

        ItemType getType() const {return type;}
        
        int offsetH;
        int offsetW;
        int size;
        
        //Type: Text
        std::string characters;
        std::uint8_t intensity;
        
        
    private:
        ItemType type;
};

#endif
