#include "DisplayItem.h"

DisplayItem::DisplayItem
():
type(ItemType::Empty)
{}

DisplayItem::DisplayItem(ItemType type):type(type)
{}

DisplayItem::DisplayItem
(   
    int offsetH,
    int offsetW,
    Font size,
    std::string characters,
    std::uint8_t intensity
):
type(Text),
offsetH(offsetH),
offsetW(offsetW),
size(size),
characters(characters),
intensity(intensity)
{}

DisplayItem::DisplayItem
(
    int offsetH,
    int offsetW,
    int endH,
    int endW,
    std::uint8_t intensity
):
type(Line),
offsetH(offsetH),
offsetW(offsetW),
endH(endH),
endW(endW),
intensity(intensity)
{}

DisplayItem::DisplayItem
(
    int offsetH,
    int offsetW,
    std::pair<int,int> boxSize,
    std::uint8_t intensity,
    uint innerMult
):
type(Rectangle),
offsetH(offsetH),
offsetW(offsetW),
endH(boxSize.first),
endW(boxSize.second),
intensity(intensity),
innerMult(innerMult)
{}

std::string DisplayItem::to_string()
{
    switch (type)
    {
        case Line:
            return "Line ("+std::to_string(offsetH)+","+std::to_string(offsetW)+")";
        case Text:
            return characters+"("+std::to_string(offsetH)+","+std::to_string(offsetW)+")";
        case Empty:
            return "__";
    }
    return "";
}
