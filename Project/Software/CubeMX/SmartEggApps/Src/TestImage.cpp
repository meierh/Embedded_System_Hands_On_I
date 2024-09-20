#include "TestImage.h"
#include "System.h"

TestImage::TestImage
(
    System* system
):
Application(system),
periodCounter(0)
{
    displayCommand();
}


void TestImage::work()
{
    //displayCommand();
}

void TestImage::onPeriod()
{
}

void TestImage::displayCommand()
{
    collectItems();
    displayCommand(displayImage);
}

void TestImage::displayCommand(std::vector<DisplayItem> items)
{
    if(system!=nullptr)
        system->displayImage(displayImage);
}

void TestImage::speakerCommand()
{
}

void TestImage::collectItems()
{
    displayImage.clear();

    displayImage.push_back(DisplayItem(0,0,0,128,255));
    for(int i=0; i<127; i+=14)
    {
        DisplayItem line(i,0,i,128,255);
        displayImage.push_back(line);
    }
    
    displayImage.push_back(DisplayItem(0,0,128,0,255));
    for(int i=0; i<127; i+=14)
    {
        DisplayItem line(0,i,128,i,255);
        displayImage.push_back(line);
    }
}
