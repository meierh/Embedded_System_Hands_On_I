// include CMSIS library for our Cortex-M0 processor
#define STM32F030xC
#include "stm32f0xx.h"

#include <stdbool.h>

#define LED_PIN    (7)

void setup_sys_clk(void);

void setup_timer(void);

void setup_led(void);
void turn_led_on(void);
void turn_led_off(void);
void toggle_led(void);


int main(void) {

    setup_sys_clk();
    setup_timer();
    setup_led();

    turn_led_off();

    // main blinking loop
    while (true) {
        toggle_led();
        while (!(TIM3->SR & TIM_SR_UIF));             // busy-wait until an update interrupt occurs (i.e. the counter overflows)
        TIM3->SR &= ~TIM_SR_UIF;                      // clear the update interrupt flag
    }

    return 0;
}

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

void setup_timer(void) {
    // Clock-enable TIM3
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // Reset TIM3 (optional: no effect in our case)
    TIM3->CR1 &= ~(TIM_CR1_CEN);                      // ensure that the counter is disabled
    RCC->APB1RSTR |= RCC_APB1RSTR_TIM3RST;            // reset the timer
    RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM3RST;

    // Set up the counter
    TIM3->PSC = 480 - 1;                              // set prescaler s.t. timer frequency is 100 kHz  --  48000 kHz : 100 kHz = 480
                                                      //    Note: Subtract one according to datasheet
    TIM3->ARR = 50000 - 1;                            // set reload reg s.t. timer overflows every 500 ms  --  500 ms : (1/100 kHz) = 50000
                                                      //    Note: As the counter starts with 0, we have to subtract 1 here as well
    TIM3->CR1 &= ~TIM_CR1_CMS;                        // set to 'edge-aligned' mode (optional: no effect in our case)
    TIM3->CR1 &= ~TIM_CR1_DIR;                        // set edge-aligned mode to 'upcounting' (optional: no effect in our case)

    TIM3->CR1 |= TIM_CR1_URS;                         // set URS s.t. only counter overflow/underflow generates update interrupt

    TIM3->EGR |= TIM_EGR_UG;                          // generate an update event to apply these settings

    TIM3->CR1 |= TIM_CR1_CEN;                         // enable timer
}

void setup_led(void) {
    // Enable the GPIOA peripheral in 'RCC_AHBENR'.
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    // A7 is connected to an LED
    // It should be set to push-pull low-speed output.
    // reset the register with 00
    GPIOA->MODER  &= ~(0x3 << (LED_PIN*2));
    // set the register to output mode
    GPIOA->MODER  |=  (0x1 << (LED_PIN*2));
    // set output type register to outout open-drain
    GPIOA->OTYPER &= ~(1 << LED_PIN);
    // reset PUDR to no pull-up, pull down (as pin is already permanently connected to ground through the LED)
    GPIOA->PUPDR   &= ~(3 << (LED_PIN*2));
}

void turn_led_on(void) {
    // enable the pin
    GPIOA->ODR |= (1 << LED_PIN);
}

void turn_led_off(void) {
    // disable the pin
    GPIOA->ODR |= (0 << LED_PIN);
}

void toggle_led(void) {
    GPIOA->ODR ^= (1 << LED_PIN);
}