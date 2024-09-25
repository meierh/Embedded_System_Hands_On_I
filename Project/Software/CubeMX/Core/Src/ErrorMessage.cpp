//
// Created by Tobias Henig on 25.09.24.
//

#include "ErrorMessage.h"
#include "System_STM32.h"

extern System_STM32 *hardware;

void displayError(const char* errorMessage)
{
    std::vector<DisplayItem> displayItems;

    std::string errorDescription = errorMessage;
    if (errorDescription.empty())
        errorDescription = "Unknown error";

    displayItems.push_back(DisplayItem(7, 0, DisplayItem::Font24, ":", 255));
    displayItems.push_back(DisplayItem(7, 9, DisplayItem::Font24, "(", 255));

    displayItems.push_back(DisplayItem(35, 0, DisplayItem::Font12, "SmartEggTimer ran", 255));
    displayItems.push_back(DisplayItem(47, 0, DisplayItem::Font12, "into a problem", 255));
    displayItems.push_back(DisplayItem(59, 0, DisplayItem::Font12, "that it couldn't", 255));
    displayItems.push_back(DisplayItem(71, 0, DisplayItem::Font12, "handle:", 255));

    displayItems.push_back(DisplayItem(91, 0, DisplayItem::Font12, errorDescription, 255));

    displayItems.push_back(DisplayItem(124, 0, DisplayItem::Font8, "Please restart the device", 255));

    hardware->displayImage(displayItems);
}
