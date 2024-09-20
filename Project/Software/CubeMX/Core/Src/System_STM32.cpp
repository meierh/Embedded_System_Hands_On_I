#include "System_STM32.h"
#include "stm32f1xx_it.h"

System_STM32::System_STM32()
{
    //std::cout<<"Setup System"<<std::endl;
    OLED_1in5_Init();
    Driver_Delay_ms(500); //@todo necessary?
    OLED_1in5_Clear();
}

System_STM32::~System_STM32()
{
}

// get the current time
// return Datetime
DateTime System_STM32::getSystemTime() {
    DS3231 clock;
    return clock.getCurrentTime();
}

void System_STM32::setSystemTime(DateTime newTime) {
    DS3231 clock;
    clock.setCurrentTime(newTime);
}

void System_STM32::displayImage(std::vector <DisplayItem> image) {
// check if display is ready
    if(System_Init() != 0) {
        // @todo error handling
        //return -1;
        OLED_1in5_Init();
        OLED_1in5_Clear();
    }

    // 0.Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((OLED_1in5_WIDTH%2==0)? (OLED_1in5_WIDTH/2): (OLED_1in5_WIDTH/2+1)) * OLED_1in5_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        //return -1;
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, OLED_1in5_WIDTH, OLED_1in5_HEIGHT, 180, BLACK); //@todo rotate by 90?
    Paint_SetScale(16);
    printf("Drawing\r\n");
    // 1.Select Image
    Paint_SelectImage(BlackImage);
    //Driver_Delay_ms(500); //@todo remove later? or change prio
    Paint_Clear(BLACK);


    // display every item on the screen
    for(const DisplayItem& displayItem : image)
    {
        switch (displayItem.getType()) {

            case DisplayItem::Line:
                // draw a line on the display
                // we could also use different line styles like
                // - LINE_STYLE_DOTTED
                Paint_DrawLine(displayItem.offsetW+1, displayItem.offsetH+1, displayItem.endW+1, displayItem.endH+1, WHITE, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
                break;
            case DisplayItem::Text:
            {
                // convert 255 colors into 16er grey scale
                uint16_t color = displayItem.intensity/16;
                uint16_t h_offset = 7;
                // draw text on the display
                switch (displayItem.size) {
                    // draw the text with the co
                    case DisplayItem::Font8:
                        Paint_DrawString_EN(displayItem.offsetW, displayItem.offsetH-h_offset, displayItem.characters.c_str(), &Font8, color, 0x0);
                        break;
                    case DisplayItem::Font12:
                        Paint_DrawString_EN(displayItem.offsetW, displayItem.offsetH-h_offset, displayItem.characters.c_str(), &Font12, color, 0x0);
                        break;
                    case DisplayItem::Font16:
                        Paint_DrawString_EN(displayItem.offsetW, displayItem.offsetH-h_offset, displayItem.characters.c_str(), &Font16, color, 0x0);
                        break;
                    case DisplayItem::Font20:
                        Paint_DrawString_EN(displayItem.offsetW, displayItem.offsetH-h_offset, displayItem.characters.c_str(), &Font20, color, 0x0);
                        break;
                    case DisplayItem::Font24:
                        Paint_DrawString_EN(displayItem.offsetW, displayItem.offsetH-h_offset, displayItem.characters.c_str(), &Font24, color, 0x0);
                        break;
                    default:
                        Paint_DrawString_EN(displayItem.offsetW, displayItem.offsetH-h_offset, displayItem.characters.c_str(), &Font12, color, 0x0);
                        break;
                }
                break;
            }
            case DisplayItem::Rectangle:
                // draw a rectangle on the display
                // we can choose between
                // - filled rectangle with DRAW_FILL_FULL
                // - empty rectangle with DRAW_FILL_EMPTY
                Paint_DrawRectangle(displayItem.offsetH, displayItem.offsetH, displayItem.endW, displayItem.endH, WHITE, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
                break;
            case DisplayItem::Empty:
                // to hide a part of the image;
                break;

                // we could also draw
                // - points with Paint_DrawPoint(20, 10, WHITE, DOT_PIXEL_1X1, DOT_STYLE_DFT);
                // - circle Paint_DrawCircle(60, 30, 15, WHITE, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
                //          Paint_DrawCircle(100, 40, 20, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
        }
    }
    OLED_1in5_Display(BlackImage);
    free(BlackImage);
}


int8_t System_STM32::getSeconds() {
    DS3231 time;
    return time.getSecond();
}

void System_STM32::buttonModePress() {
    // disable all interrupts
    //__disable_irq();
    HAL_NVIC_DisableIRQ(TIM3_IRQn); //@todo add button interrupt
    System::buttonModePress();
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
    // enable all interrupts again
    //__enable_irq();
}

