#include "SmartEgg.h"
#include "System.h"

SmartEgg::SmartEgg
(
    System* system
):
EggTimerBase(system)
{
    modeStatus.characters = "SmartEgg";
    
    minText = DisplayItem(40,15,DisplayItem::Font16,"Min",255);
    secText = DisplayItem(40,70,DisplayItem::Font16,"Sec",255);
    timeMin = DisplayItem(70,20,DisplayItem::Font24,"00",255);
    timeSeparator = DisplayItem(70,57,DisplayItem::Font24,":",255);
    timeSec = DisplayItem(70,75,DisplayItem::Font24,"00",255);
    
    eggText =       DisplayItem(100, 2,DisplayItem::Font16,"Egg"  ,255);
    
    eggSize =       DisplayItem(115, 2,DisplayItem::Font8,"XX"   ,30);
    eggIniTemp =    DisplayItem(115,10,DisplayItem::Font8,"XXXX" ,30);
    eggPressure =   DisplayItem(115,32,DisplayItem::Font8,"XXXXX",30);
    eggEndTemp =    DisplayItem(115,63,DisplayItem::Font8,"XXXX" ,30);
    
    writeSize(sizes[sizeInd]);
    writeIniTemp(setIniTemp);
    writePressure(setPressure);
    writeEndTemp(setEndTemp);
    
    leftButtonLabel.characters = "Left";
    leftButtonLabel.offsetW -= 5 ;
    centerButtonLabel.characters = "Enter";
    rightButtonLabel.characters = "Right";

    std::cout<<"Setup SmartEgg"<<std::endl;
}

void SmartEgg::work()
{
    if(!inputActions.empty())
    {
        Action action = inputActions.front();
        inputActions.pop();
        switch(action)
        {
            case BtnLeftClick:
            {
                if(status==Run || status==End)
                {
                    status = SetSize;
                    eggSize.intensity = 255;
                }
                else
                {
                    switch(status)
                    {
                        case SetSize:
                            eggSize.intensity = 30;
                            eggIniTemp.intensity = 255;
                            status = SetIniTemp;
                            break;
                        case SetIniTemp:
                            eggIniTemp.intensity = 30;
                            eggPressure.intensity = 255;
                            status = SetPressure;
                            break;
                        case SetPressure:
                            eggPressure.intensity = 30;
                            eggEndTemp.intensity = 255;
                            status = SetEndTemp;
                            break;
                        case SetEndTemp:
                            eggEndTemp.intensity = 30;
                            eggSize.intensity = 255;
                            status = SetSize;
                            break;
                    }
                }
                break;
            }
            case BtnCenterClick:
            {
                std::cout<<"Enter"<<std::endl;
                if(status!=Run && status!=End)
                {
                    std::cout<<"Reset"<<std::endl;
                    eggSize.intensity = 30;
                    eggIniTemp.intensity = 30;
                    eggPressure.intensity = 30;
                    eggEndTemp.intensity = 30;
                    remainingSeconds = computePerfectEggTime();
                    std::pair<uint,uint> minSecs = secondsToMinSecs(remainingSeconds);
                    setMinutes = minSecs.first;
                    setSeconds = minSecs.second;
                    status = Run;             
                }
                std::cout<<"Stat:"<<std::to_string(status)<<std::endl;
                break;
            }
            case BtnRightClick:
            {
                if(status==Run || status==End)
                {
                    status = SetSize;
                    eggSize.intensity = 255;
                }
                else
                {
                    switch(status)
                    {
                        case SetSize:
                            eggSize.intensity = 30;
                            eggIniTemp.intensity = 255;
                            status = SetIniTemp;
                            break;
                        case SetIniTemp:
                            eggIniTemp.intensity = 30;
                            eggPressure.intensity = 255;
                            status = SetPressure;
                            break;
                        case SetPressure:
                            eggPressure.intensity = 30;
                            eggEndTemp.intensity = 255;
                            status = SetEndTemp;
                            break;
                        case SetEndTemp:
                            eggEndTemp.intensity = 30;
                            eggSize.intensity = 255;
                            status = SetSize;
                            break;
                    }
                }
                break;
            }
            case RotateClock:
            {
                switch(status)
                {
                    case SetSize:
                        sizeInd++;
                        if(sizeInd>=sizes.size())
                            sizeInd=0;
                        break;
                    case SetIniTemp:
                        setIniTemp++;
                        if(setIniTemp>upperBoundIniTemp)
                            setIniTemp=lowerBoundIniTemp;
                        break;
                    case SetPressure:
                        setPressure++;
                        if(setPressure>upperBoundPressure)
                            setPressure=lowerBoundPressure;
                        break;
                    case SetEndTemp:
                        setEndTemp++;
                        if(setEndTemp>upperBoundEndTemp)
                            setEndTemp=lowerBoundEndTemp;
                        break;
                }
                break;
            }
            case RotateAntiClock:
            {
                switch(status)
                {
                    case SetSize:
                        sizeInd--;
                        if(sizeInd<0)
                            sizeInd=3;
                        break;
                    case SetIniTemp:
                        setIniTemp--;
                        if(setIniTemp<lowerBoundIniTemp)
                            setIniTemp=upperBoundIniTemp;
                        break;
                    case SetPressure:
                        setPressure--;
                        if(setPressure<lowerBoundPressure)
                            setPressure=upperBoundPressure;
                        break;
                    case SetEndTemp:
                        setEndTemp--;
                        if(setEndTemp<lowerBoundEndTemp)
                            setEndTemp=upperBoundEndTemp;
                        break;
                }
                break;
            }
            case OnePeriod:
            {
                std::cout<<sizes[sizeInd]<<" "<<std::to_string(setIniTemp)<<" "<<std::to_string(setPressure)<<" "<<std::to_string(setEndTemp)<<std::endl;
                if(status==Run)
                {
                    std::cout<<"Run"<<std::endl;
                    if(remainingSeconds>0)
                    {
                        remainingSeconds--;
                        std::pair<uint,uint> secMins = secondsToMinSecs(remainingSeconds);
                        writeMinutes(secMins.first);
                        writeSeconds(secMins.second);
                        std::cout<<"Rem:"<<remainingSeconds<<std::endl;
                        displayCommand();
                    }
                    else
                        status = End;
                }
                else if(status!=End)
                {
                    displayCommand();
                }
                break;
            }
            default:
                break;
        }
        displayCommand();
    }
}

void SmartEgg::onPeriod()
{
    inputActions.push(Action::OnePeriod);
    /*peroidCounter++;
    if(peroidCounter>=100)
    {
        peroidCounter=0;
        inputActions.push(Action::OnePeriod);
    }*/
}

void SmartEgg::displayCommand()
{
    std::cout<<"status:"<<std::to_string(status)<<std::endl;
    updateClock();
    writeSize(sizes[sizeInd]);
    writeIniTemp(setIniTemp);
    writePressure(setPressure);
    writeEndTemp(setEndTemp);
    
    collectItems();
    displayCommand(displayImage);
}

void SmartEgg::displayCommand(std::vector<DisplayItem> items)
{
    if(system!=nullptr)
        system->displayImage(displayImage);
}

void SmartEgg::speakerCommand()
{
}

void SmartEgg::collectItems()
{
    displayImage.clear();
    
    Application::collectItems();
        
    displayImage.push_back(minText);
    displayImage.push_back(secText);
    
    displayImage.push_back(timeMin);
    displayImage.push_back(timeSeparator);
    displayImage.push_back(timeSec);
    
    displayImage.push_back(eggText);
    displayImage.push_back(eggSize);
    displayImage.push_back(eggIniTemp);
    displayImage.push_back(eggPressure);
    displayImage.push_back(eggEndTemp);
}

uint SmartEgg::computePerfectEggTime()
{
    float M = sizesWeight[sizeInd];
    float c = 3.7; // J g^-1 K^-1
    float rho = 1.038; // g cm^-3
    float K = 0.0054; // W cm^-1 K^-1
    float T_egg = setIniTemp;
    float T_water = computeBoilingTemperature();
    std::cout<<"T_water:"<<T_water<<std::endl;
    float T_yolk = setEndTemp;
    std::cout<<"T_yolk:"<<T_yolk<<std::endl;
    float factor1 = c/(K*(pi)*(pi));
    float factor2 = 9*M*M*rho/(16*pi*pi);
    float factor3 = 0.76 * (T_egg-T_water)/(T_yolk-T_water);
    return factor1*std::cbrt(factor2)*log(factor3);
}

float SmartEgg::computeBoilingTemperature()
{
    float T_0 = 99.97; //°C
    float P_0 = 1013.25; //mBar
    float P = setPressure;
    float H_vap = 40660; //J mol^-1
    float R =  8.31446261815324; // J K^−1 mol^−1
    float inv_TB = (1/(T_0+273.15) - (R*std::log(P/P_0))/H_vap);
    return (1/inv_TB)-273.15;;
}

void SmartEgg::writeSize(std::string size)
{
    eggSize.characters = size;
}

void SmartEgg::writeIniTemp(int temp)
{
    if(temp<0)
        eggIniTemp.characters = " "+std::to_string(temp);
    else if(temp<10)
        eggIniTemp.characters = "  "+std::to_string(temp);
    else
        eggIniTemp.characters = std::to_string(temp);
    
    eggIniTemp.characters+="C";
}

void SmartEgg::writePressure(uint pressure)
{
    if(pressure>=1000)
        eggPressure.characters = std::to_string(pressure);
    else
        eggPressure.characters = " "+std::to_string(pressure);
    
    eggPressure.characters+="m";
}

void SmartEgg::writeEndTemp(int temp)
{
    eggEndTemp.characters = std::to_string(temp);
    eggEndTemp.characters+="C";
}
