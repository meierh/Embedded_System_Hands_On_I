#include "DisplayItem.h"

DisplayItem::DisplayItem(ItemType type):type(type)
{}

DisplayItem::DisplayItem
(   
    int offsetH,
    int offsetW,
    int size,
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
