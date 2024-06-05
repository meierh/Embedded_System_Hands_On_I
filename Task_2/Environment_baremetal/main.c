// include CMSIS library for our Cortex-M0 processor
//#include "stm32f030xc.h"
#define STM32F030xC
#include "stm32f0xx.h"
#include <stdint.h>

#define LED0_PIN    (7)
#define LED1_PIN	(0)
#define LED2_PIN	(1)

int main(void) {

	// Enable the GPIOA and GPIOB peripherals in 'RCC_AHBENR'.
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;

	// A7 is connected to an LED
	// It should be set to push-pull low-speed output.
	// reset the register with 00
	GPIOA->MODER  &= ~(0x3 << (LED0_PIN*2));
	// set the register to output mode
	GPIOA->MODER  |= (0x1 << (LED0_PIN*2));
	// set output type register to outout push-pull
	GPIOA->OTYPER &= ~(1 << LED0_PIN);
	// enable the LED
	GPIOA->ODR |= (1 << LED0_PIN);

	// Turn on the other two LEDs the same way
	GPIOB->MODER &= ~(0x3 << (LED1_PIN*2) | 0x3 << (LED2_PIN*2));
	GPIOB->MODER |= (0x1 << (LED1_PIN*2) | 0x1 << (LED2_PIN*2));
	GPIOB->OTYPER &= ~(1 << LED1_PIN | 1 << LED2_PIN);
	GPIOB->ODR |= (1 << LED1_PIN) | (1 << LED2_PIN);

	return 0;
}
