// include CMSIS library for our Cortex-M0 processor
#define STM32F030xC
#include "stm32f0xx.h"

#include <stdint.h>

#define LED_0_PIN                 (7) // PA7 (green)
#define LED_1_PIN                 (0) // PB0 (red)
#define LED_2_PIN                 (1) // PB1 (green)

// orientation of the joystick pins in this case: J2 facing upwards
#define JOYSTICK_RIGHT_PIN       (13) // PB13
#define JOYSTICK_UP_PIN          (10) // PB10
#define JOYSTICK_LEFT_PIN        (11) // PB11
#define JOYSTICK_DOWN_PIN        (12) // PB12

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
    {LED_0_PIN, GPIOA, DEFAULT_PERIOD_MS},
    {LED_1_PIN, GPIOB, DEFAULT_PERIOD_MS},
    {LED_2_PIN, GPIOB, DEFAULT_PERIOD_MS}
};

/// current_led "points" to the led which is currently blinking. The other leds should be turned off.
/// We did not use a pointer to avoid weird pointer arithmetics on updating the currently blinking led.
volatile uint8_t current_led = 0;


void turn_led_on(uint8_t);
void turn_led_off(uint8_t);
void toggle_led(uint8_t);

void change_led(int8_t);
void change_period(int16_t);

void setup_sys_clk(void);
void setup_timer(void);
void start_timer(uint16_t);
void stop_timer(void);
void setup_led(void);
void setup_one_led(uint8_t);
void setup_joystick(void);
void setup_one_direction_of_joystick(uint32_t);

// overrides the interrupt handler. The methode name is determined by the vector table, see ./startup_stm32f030xc.s
void TIM3_IRQHandler(void) {
    if(TIM3->SR & TIM_SR_UIF) {
        TIM3->SR &= ~(TIM_SR_UIF);                    // clear the update interrupt flag
        toggle_led(current_led);                      // toggle the current LED
    }
}

// interrupt handler for the joystick
void EXTI4_15_IRQHandler(void) {
    // detect which EXTI line caused the interrupt
    if (EXTI->PR & (1 << JOYSTICK_LEFT_PIN)) {
        // Clear the EXTI status flag. 1=clear
        EXTI->PR |= (1 << JOYSTICK_LEFT_PIN);
        // joystick left press action
        change_led(+1);

    } else if (EXTI->PR & (1 << JOYSTICK_RIGHT_PIN)) {
        // Clear the EXTI status flag. 1=clear
        EXTI->PR |= (1 << JOYSTICK_RIGHT_PIN);
        // joystick right press action
        change_led(-1);
        

    } else if (EXTI->PR & (1 << JOYSTICK_UP_PIN)) {
        // Clear the EXTI status flag. 1=clear
        EXTI->PR |= (1 << JOYSTICK_UP_PIN);
        // joystick up press action
        change_period(+ PERIOD_IN_DECREMENT_MS);

    } else if (EXTI->PR & (1 << JOYSTICK_DOWN_PIN)) {
        // Clear the EXTI status flag. 1=clear
        EXTI->PR |= (1 << JOYSTICK_DOWN_PIN);
        // joystick down press action
        change_period(- PERIOD_IN_DECREMENT_MS);

    } else {
        // None of the JOYSTICK_*_PINs caused the interrupt. This is an error.
        setup_timer();                                // reset/stop timer
        turn_led_on(0);                               // turn all LEDs on to "report" this error
        turn_led_on(1);
        turn_led_on(2);
        EXTI->PR |= 0x807BFFFF;                       // clear all EXTI status flags
    }
}

int main(void) {

    setup_led();

    setup_joystick();

    setup_sys_clk();
    setup_timer();

    // initialisation: turn on default current led and set timer to default period
    turn_led_on(current_led);
    start_timer(DEFAULT_PERIOD_MS / 2);

    while (1) {

    }

    return 0;
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

/// start the timer (TIM3) with the given time (in ms) as duration
// requires to setup the timer previously
void start_timer(uint16_t time_ms) {
    TIM3->CR1 &= ~(TIM_CR1_CEN);                      // stop the counter
    
    TIM3->ARR = time_ms - 1;                          // set reload register s.t. the timer overflows after time_ms
    TIM3->EGR |= TIM_EGR_UG;                          // generate an update event to apply this setting

    TIM3->CR1 |= TIM_CR1_CEN;                         // re-enable the counter
}

/// stop the timer (TIM3)
void stop_timer(void) {
    TIM3->CR1 &= ~(TIM_CR1_CEN);
}

/// turn the led on
void turn_led_on(uint8_t led_idx) {
    GPIO_TypeDef *GPIOx = leds[led_idx].port;
    uint8_t led_pin = leds[led_idx].pin;

    // enable the pin
    // ODR = output data register
    GPIOx->ODR |= (1 << led_pin);
}

/// turn the led off
void turn_led_off(uint8_t led_idx) {
    GPIO_TypeDef *GPIOx = leds[led_idx].port;
    uint8_t led_pin = leds[led_idx].pin;

    GPIOx->ODR &= ~(1 << led_pin);
}

/// toggle the led between on/off
void toggle_led(uint8_t led_idx) {
    GPIO_TypeDef *GPIOx = leds[led_idx].port;
    uint8_t led_pin = leds[led_idx].pin;

    GPIOx->ODR ^= (1 << led_pin);
}


/* ----------------------- SETUP FUNCTIONS ----------------------- */

/// setup the system clock to 48 MHz
void setup_sys_clk(void) {
    
    // Increase flash access latency and enable prefetch buffer
    // This is necessary as the flash memory apparently has a maximum speed of 24 MHz. The prefetch buffer might slightly enhance performance.
    FLASH->ACR &= ~0x17;                               // Reset the register
    FLASH->ACR |= FLASH_ACR_LATENCY | FLASH_ACR_PRFTBE;

    // Set the PLL to 48 MHz
    RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL); // Reset PLL source and multiplicator
    RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_DIV2;             // Select HSI/2 (4 MHz) as PLL input
    RCC->CFGR |= RCC_CFGR_PLLMUL12;                    // Multiply PLL input (4 MHz) with 12 => 48 MHz

    // Enable the PLL and wait until it is ready
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY));

    // Select PLL as system clock source and wait until switched successfully
    RCC->CFGR &= ~RCC_CFGR_SW; RCC->CFGR |= RCC_CFGR_SW_PLL;
    while (!(RCC->CFGR & RCC_CFGR_SWS_PLL));
}

/// setup the timer (TIM3) and enable interrupt
void setup_timer(void) {
    // enable the TIM3 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // enable the NVIC interrupt for the TIM3, set priority higher than EXTI (i.e. joystick) interrupts
    NVIC_SetPriority(TIM3_IRQn, 0x02);
    NVIC_EnableIRQ(TIM3_IRQn);

    // Reset TIM3
    TIM3->CR1 &= ~(TIM_CR1_CEN);                      // ensure that the counter is disabled
    RCC->APB1RSTR |= RCC_APB1RSTR_TIM3RST;            // reset the timer
    RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM3RST;

    // Set up the counter
    TIM3->PSC = 48000 - 1;                            // set prescaler s.t. timer frequency is 1 kHz  --  48000 kHz : 1 kHz = 48000   ==>  counts up by 1 every ms
    
    TIM3->CR1 &= ~TIM_CR1_CMS;                        // set to 'edge-aligned' mode (optional: no effect in our case)
    TIM3->CR1 &= ~TIM_CR1_DIR;                        // set edge-aligned mode to 'upcounting' (optional: no effect in our case)

    TIM3->CR1 |= TIM_CR1_URS;                         // set URS s.t. only counter overflow/underflow generates update interrupt
    TIM3->DIER |= TIM_DIER_UIE;                       // enable hardware interrupts

    TIM3->EGR |= TIM_EGR_UG;                          // generate an update event to apply these settings
}

/// setup the LED
void setup_one_led(uint8_t led_idx) {
    uint8_t led_pin = leds[led_idx].pin;
    GPIO_TypeDef *GPIOx = leds[led_idx].port;

    // reset the register with 00
    GPIOx->MODER  &= ~(0x3 << (led_pin*2));
    // set the register to output mode
    GPIOx->MODER  |=  (0x1 << (led_pin*2));
    // set output type register to outout open-drain
    GPIOx->OTYPER &= ~(1 << led_pin);
    // reset PUDR to no pull-up, pull down (as pin is already permanently connected to ground through the LED)
    GPIOx->PUPDR &= ~(3 << (led_pin*2));
}

void setup_led(void) {
    // Enable the GPIOA peripheral in 'RCC_AHBENR'. for led 0
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    // Enable the GPIOB peripheral in 'RCC_AHBENR'. for led 1 and led 2
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    setup_one_led(0);
    setup_one_led(1);
    setup_one_led(2);
}

void setup_one_direction_of_joystick(uint32_t joystick_pin) {
    // reset port mode register to input mode (00)
    GPIOB->MODER &= ~(0x3 << (joystick_pin*2));
    // reset port pull-up/pull-down register (00)
    GPIOB->PUPDR &= ~(0x3 << (joystick_pin*2));
    
    //GPIOB->PUPDR |= (0x1 << (joystick_pin*2));    // set PUPDR register to pull up mode (01) so that the pin gets pulled down when the stick is moved
    // pull up is not necessary as pull-up resistors already are on the extension board (cf. SKQUCAA010 at the right of the schematic)

    // Set external interrupt configuration register 1
    // to connect the button EXTI line to GPIOB.
    SYSCFG->EXTICR[(joystick_pin/4)] &= ~(0xF << ((joystick_pin % 4) * 4));
    SYSCFG->EXTICR[(joystick_pin/4)] |=  (0x1 << ((joystick_pin % 4) * 4));

    // Setup the button's EXTI line as an interrupt.
    // IMR = Interrupt mask register
    EXTI->IMR  |=  (1 << joystick_pin);
    // Disable the 'rising edge' trigger (button release).
    // RTSR = Rising trigger selection register
    EXTI->RTSR &= ~(1 << joystick_pin);
    // Enable the 'falling edge' trigger (button press).
    // FTSR = Falling trigger selection register
    EXTI->FTSR |=  (1 << joystick_pin);
}


void setup_joystick(void) {
    // enable GPIOB
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    // setup interrupts
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;                 // Clock-enable the system configuration controller which is responsible for the connection of the EXTI lines to the GPIOs
    setup_one_direction_of_joystick(JOYSTICK_DOWN_PIN);
    setup_one_direction_of_joystick(JOYSTICK_UP_PIN);
    setup_one_direction_of_joystick(JOYSTICK_LEFT_PIN);
    setup_one_direction_of_joystick(JOYSTICK_RIGHT_PIN);

    // Enable the NVIC interrupt for EXTI4-15 at minimum priority.
    // there are four priority levels 0 = highest, 4 = lowest
    NVIC_SetPriority(EXTI4_15_IRQn, 0x03);
    NVIC_EnableIRQ(EXTI4_15_IRQn);
}

