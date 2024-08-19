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
