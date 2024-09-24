#include "MP3Player.h"
#include "System.h"

MP3Player::MP3Player
(
    System* system
):
Application(system),
periodCounter(0)
{
    system->playerTurnOn(true);
    
    modeStatus.characters = "eiPod";
    leftButtonLabel.characters = "Prev";
    leftButtonLabel.offsetW -= 10 ;
    centerButtonLabel.characters = "Play";
    centerButtonLabel.offsetW += 4 ;
    rightButtonLabel.characters = "Next";
    
    trackLabel = DisplayItem(35,37,DisplayItem::Font16,"Track",255);
    trackIDLabel = DisplayItem(55,20,DisplayItem::Font16,"000",255);
    trackSeparatorLabel = DisplayItem(55,60,DisplayItem::Font16,"/",255);
    trackNumberLabel = DisplayItem(55,80,DisplayItem::Font16,"000",255);
    
    int offsetW = 35;
    int offsetH = 90;
    int barHeight = 1;
    for(int w=0; w<volumeLabels.size(); w++)
    {
        volumeLabels[w] = DisplayItem(offsetH-barHeight,offsetW+w,offsetH,offsetW+w,255);
        if((w+1)%4==0)
            barHeight++;
    }
    volumeLabelsCasingLeft = DisplayItem(offsetH-1,offsetW-1,offsetH,offsetW-1,255);
    volumeLabelsCasingBottom = DisplayItem(offsetH,offsetW-1,offsetH,offsetW+61,255);
    volumeLabelsCasingTop = DisplayItem(offsetH-1,offsetW-1,offsetH-15,offsetW+62,255);
    volumeLabelsCasingRight = DisplayItem(offsetH-15,offsetW+62,offsetH,offsetW+62,255);
    
    readMP3Info();
    displayMP3Info();
    
    std::cout<<"Setup MP3Player"<<std::endl;
}

MP3Player::~MP3Player()
{
    system->playerTurnOff();
}

void MP3Player::work()
{
    if(!inputActions.empty())
    {
        Action action = inputActions.front();
        inputActions.pop();
        switch(action)
        {
            case BtnLeftClick:
            {
                system->playerControl(System::PlayerAction::Previous);
                centerButtonLabel.characters = "Pause";
                playing = true;
                break;
            }
            case BtnCenterClick:
            {
                if(playing)
                {
                    system->playerControl(System::PlayerAction::Pause);
                    centerButtonLabel.characters = "Play";
                    playing = false;
                }
                else
                {
                    system->playerControl(System::PlayerAction::Play);
                    centerButtonLabel.characters = "Pause";
                    playing = true;
                }
                break;
            }
            case BtnRightClick:
            {
                system->playerControl(System::PlayerAction::Next);
                centerButtonLabel.characters = "Pause";
                playing = true;
                break;
            }
            case RotateClock:
            {
                system->playerControl(System::PlayerAction::IncreaseVolume);
                break;
            }
            case RotateAntiClock:
            {
                system->playerControl(System::PlayerAction::DecreaseVolume);
                break;
            }
            case OnePeriod:
            {
                break;
            }
            default:
                break;
        }
        displayCommand();
    }
}

void MP3Player::onPeriod()
{
    inputActions.push(Action::OnePeriod);
}

void MP3Player::displayCommand()
{
    displayMP3Info();
    collectItems();
    Application::displayCommand(displayImage);
}

void MP3Player::readMP3Info()
{
    System::PlayingStatus status;
    uint8_t volume;
    uint16_t numberOfTracks;
    uint16_t currentTrack;
    system->playerGetState(status,volume,numberOfTracks,currentTrack);
    switch(status)
    {
        case System::PlayingStatus::STATUS_PAUSED:
            playing=false;
            break;
        case System::PlayingStatus::STATUS_PLAYING:
            playing=true;
            break;
    }
    this->volume = volume;
    this->trackNumber = numberOfTracks;
    this->trackID = currentTrack;
}

void MP3Player::displayMP3Info()
{
    readMP3Info();
    if(trackID<10)
        trackIDLabel.characters = "00"+std::to_string(trackID);
    else if(trackID<100)
        trackIDLabel.characters = "0"+std::to_string(trackID);
    else
        trackIDLabel.characters = std::to_string(trackID);
    
    if(trackNumber<10)
        trackNumberLabel.characters = "00"+std::to_string(trackNumber);
    else if(trackNumber<100)
        trackNumberLabel.characters = "0"+std::to_string(trackNumber);
    else
        trackNumberLabel.characters = std::to_string(trackNumber);
    
    displayVolume();    
}

void MP3Player::displayVolume()
{
    for(int w=0; w<volumeLabels.size(); w++)
    {
        if(w<volume*2)
            volumeLabels[w].setType(DisplayItem::ItemType::Line);
        else
            volumeLabels[w].setType(DisplayItem::ItemType::Empty);
    }
}

void MP3Player::collectItems()
{
    displayImage.clear();

    Application::collectItems();
    
    displayImage.push_back(trackLabel);
    displayImage.push_back(trackIDLabel);
    displayImage.push_back(trackSeparatorLabel);
    displayImage.push_back(trackNumberLabel);
    
    for(int w=0; w<volumeLabels.size(); w++)
    {
        displayImage.push_back(volumeLabels[w]);
    }
    displayImage.push_back(volumeLabelsCasingLeft);
    displayImage.push_back(volumeLabelsCasingBottom);
    displayImage.push_back(volumeLabelsCasingTop);
    displayImage.push_back(volumeLabelsCasingRight);
}
