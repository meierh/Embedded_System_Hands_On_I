#include "led_dimmer.h"
#include "cmath"

volatile double current_brightness = 1;

// modes
// 0: manually
// 1: Auto with triangle function
// 2: Heartbeat
uint8_t currentMode = 2;

double brightness_max_value = 100;
double brightness_min_value = 0;
double brightness_step = 1;

int number_of_steps = 100;
int precision_pwm = 665;
double brightness_basis = 3.26;

/**
 * joystick interrupt callback
 * @param GPIO_Pin
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == STICK_DOWN_Pin || GPIO_Pin == STICK_UP_Pin) {
        // change current mode
        currentMode = (currentMode+1) % 3;
    } else if(GPIO_Pin == STICK_LEFT_Pin) {
        Increase_brightness();
        // optional_todo: increase brightness smoothly if button is still pressed
    } else if(GPIO_Pin == STICK_RIGHT_Pin) {
        Decrease_brightness();
        // optional_todo: decrease brightness smoothly if button is still pressed
    } else {
        // reset
        __NOP();
    }
}

/**
 * restart the pwm timers (tim1 and tim3) with new brightness
 * @param new_brightness the new brightness of the led in %
 */
void restart_pwm(double new_brightness_step) {
    // calculate the next brightness step with a logarithmic scale
    double new_brightness = pow(brightness_basis, log2(precision_pwm) * (new_brightness_step + 1) / number_of_steps) - 1;
    sConfigOC.Pulse = (uint32_t) (new_brightness);
    if (HAL_TIM_OC_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_TIM_OC_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) {
        Error_Handler();
    }
}


/**
 * increase the brightness by one brightness step
 */
void Increase_brightness(void) {
    if(current_brightness < brightness_max_value) {
        current_brightness += brightness_step;
    }
    restart_pwm(current_brightness);
}

/**
 * decrease brightness by one brightness step
 */
void Decrease_brightness(void) {
    if(current_brightness > brightness_min_value) {
        current_brightness -= brightness_step;
    }

    restart_pwm(current_brightness);
}

/**
 * increase and decrease brightness with a triangle function
 */
void auto_brightness(void) {
    bool raising_edge = true;

    while (currentMode == 1) {
        // check if we have to switch direction
        if(raising_edge && current_brightness >= brightness_max_value) {
            // we reached the maximum brightness, change to decreasing mode
            raising_edge = false;
        } else if(!raising_edge && current_brightness <= brightness_min_value) {
            // we reached the minimum brightness, change to increasing mode
            raising_edge = true;
        }
        if(raising_edge) {
            Increase_brightness();
        } else {
            Decrease_brightness();
        }
        HAL_Delay(5);
    }
}

/**
 * simulate a heartbeat with the led
 */
void heartbeat(void) {
    uint8_t phase = 0;
    // phase 0: increase brightness to 100%
    // phase 1: decrease brightness to 50%
    // Phase 2: increase to 100%
    // phase 3: decrease to 0%
    // phase 4: wait for 1200 ms

    while (currentMode == 2) {

        if(phase == 0 && current_brightness >= brightness_max_value) {
            phase = 1;
        } else if(phase == 1 && current_brightness <= 0.5*brightness_max_value) {
            phase = 2;
        } else if(phase == 2 && current_brightness >= brightness_max_value) {
            phase = 3;
        } else if(phase== 3 && current_brightness <= brightness_min_value) {
            phase = 4;
        } else if(phase == 4) {
            phase = 0;
        }

        switch (phase) {
            case 0:
                Increase_brightness();
                break;
            case 1:
                Decrease_brightness();
                HAL_Delay(2);
                break;
            case 2:
                Increase_brightness();
                HAL_Delay(2);
                break;
            case 3:
                Decrease_brightness();
                break;
            case 4:
                HAL_Delay(1200);
                break;
        }
    }
}







