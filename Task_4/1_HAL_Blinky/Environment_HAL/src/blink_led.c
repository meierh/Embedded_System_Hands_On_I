// includes
#include "main.h"
#include "blink_led.h"
#include <stdint.h>


#define DEFAULT_PERIOD_MS      (1000) // all leds start blinking with this period
#define PERIOD_IN_DECREMENT_MS   (50) // when the joystick is moved up/down, the period is changed by this value
#define PERIOD_LOWER_BOUND_MS    (50) // the blinking period can not be changed below this value
#define PERIOD_UPPER_BOUND_MS (10000) // the blinking period can not be changed above this value

/// BlinkingLED_t encapsulates all settings relevant to one particular led
typedef struct {
    const uint8_t pin;           // the pin of the LED at the given port (constant)
    GPIO_TypeDef * const port;   // the GPIO port the LED is connected to (constant pointer to mutable data)
    uint16_t period_ms;          // the blinking period in ms (changed by the joystick)
} BlinkingLED_t;

/// the leds array contains all leds that should be controlled by this program
volatile BlinkingLED_t leds[3] = {
    {LED_0_Pin, LED_0_GPIO_Port, DEFAULT_PERIOD_MS},
    {LED_1_Pin, LED_1_GPIO_Port, DEFAULT_PERIOD_MS},
    {LED_2_Pin, LED_2_GPIO_Port, DEFAULT_PERIOD_MS}
};

/// current_led "points" to the led which is currently blinking. The other leds should be turned off.
/// We did not use a pointer to avoid weird pointer arithmetics on updating the currently blinking led.
volatile uint8_t current_led = 0;

/// start the timer (TIM3) with the given time (in ms) as duration
// requires to setup the timer previously
void start_timer(uint16_t time_ms) {
    // stop timer
    HAL_TIM_Base_Stop_IT(&htim3);
    // set new period
    htim3.Init.Period = time_ms;
    // Initializes the TIM Time base Unit according to the specified parameters
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
    {
        Error_Handler();
    }
    // Starts the TIM Base generation in interrupt mode.
    if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK)
    {
      Error_Handler();
    }
}

/// stop the timer (TIM3)
void stop_timer(void) {
    HAL_TIM_Base_Stop_IT(&htim3);
}

/// HAL interrupts
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
  if (htim == &htim3) {
    toggle_led(current_led);
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == JOYSTICK_DOWN_Pin) {
    change_period(- PERIOD_IN_DECREMENT_MS);

  } else if(GPIO_Pin == JOYSTICK_UP_Pin) {
    change_period(+ PERIOD_IN_DECREMENT_MS);

  } else if(GPIO_Pin == JOYSTICK_LEFT_Pin) {
    change_led(+1);

  } else if(GPIO_Pin == JOYSTICK_RIGHT_Pin) {
    change_led(-1);

  } else {
        start_timer(DEFAULT_PERIOD_MS);    // reset/stop timer
        turn_led_on(0);                    // turn all LEDs on to "report" this error
        turn_led_on(1);
        turn_led_on(2);
      __NOP();
  }
}


/// switch blinking to another led (i.e. switch to the next led for offset=1 / to the previous one for offset=-1)
void change_led(int8_t offset) {
    stop_timer();

    // turn off current led
    turn_led_off(current_led);

    // update the current led while taking care not to go out-of-bounds (+3 to cover negative offsets)
    current_led = (current_led + 3 + offset) % 3;

    // turn on newly current led
    turn_led_on(current_led);

    // reload the stored period for the current led
    start_timer(leds[current_led].period_ms / 2);
}


/// updates the led blinking period by adding offset to the current value and restarts the timer with the new resulting setting
void change_period(int16_t offset) {
    int32_t current_period = leds[current_led].period_ms;
    int32_t new_period = current_period + offset;

    // bounds-check the new period value
    if (new_period < PERIOD_LOWER_BOUND_MS)
        new_period = PERIOD_LOWER_BOUND_MS;
    else if (new_period > PERIOD_UPPER_BOUND_MS)
        new_period = PERIOD_UPPER_BOUND_MS;

    if (current_period != new_period) {
        // update period setting
        leds[current_led].period_ms = new_period;

        // toggle led to let the user know that the period was changed
        toggle_led(current_led);
    }

    // restart timer with new period
    start_timer(new_period / 2);
}




/// turn the led on
void turn_led_on(uint8_t led_idx) {
    HAL_GPIO_WritePin(leds[led_idx].port, leds[led_idx].pin, GPIO_PIN_SET);
}

/// turn the led off
void turn_led_off(uint8_t led_idx) {
     HAL_GPIO_WritePin(leds[led_idx].port, leds[led_idx].pin, GPIO_PIN_RESET);
}

/// toggle the led between on/off
void toggle_led(uint8_t led_idx) {
    HAL_GPIO_TogglePin(leds[led_idx].port, leds[led_idx].pin);
}
