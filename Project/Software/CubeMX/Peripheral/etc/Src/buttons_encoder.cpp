//
// Created by Tobias Henig on 14.09.24.
//

#include "main.h"
#include "buttons_encoder.h"
#include "System_STM32.h"

extern System_STM32* hardware;

static uint32_t lastRotation = 0;
static GPIO_PinState a0 = GPIO_PIN_SET; // both pins set to high s.t. the first rotation is detected
static GPIO_PinState c0 = GPIO_PIN_SET;

void handleEncoder(System::Direction dir);


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (hardware == nullptr)
        return;

    switch (GPIO_Pin)
    {
    case Button_1_Pin:
        // call button left press
        hardware->buttonModePress();
        break;
    case Button_2_Pin:
        // call button center press
        hardware->buttonLeftPress();
        break;
    case Button_3_Pin:
        // call button right press
        hardware->buttonCenterPress();
        break;
    case Button_4_Pin:
        // call mode button press
        hardware->buttonRightPress();
        break;

    case Rotary_Encoder_A_Pin:
        {
            // handle rotation

            // debouncing, source: http://www.technoblogy.com/show?1YHJ
            GPIO_PinState a = HAL_GPIO_ReadPin(Rotary_Encoder_A_GPIO_Port, Rotary_Encoder_A_Pin);
            GPIO_PinState b = HAL_GPIO_ReadPin(Rotary_Encoder_B_GPIO_Port, Rotary_Encoder_B_Pin);
            if (a != a0)
            {
                a0 = a;

                if (b != c0)
                {
                    c0 = b;
                    handleEncoder(a == b ? System::Direction::Counterclockwise : System::Direction::Clockwise);
                }
            }
            break;
        }

    default:
        // do nothing
        break;
    }
}

void handleEncoder(System::Direction dir)
{
    // debouncing: ignore if the last rotation was too recent
    if (HAL_GetTick() - lastRotation < ROTARY_ENCODER_MIN_INTERVAL)
    {
        return;
    }

    lastRotation = HAL_GetTick();

    hardware->rotate(dir);
}
