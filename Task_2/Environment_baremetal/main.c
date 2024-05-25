// include CMSIS library for our Cortex-M0 processor
#include "stm32f030xc.h"
#include <stdint.h>

#define LED_PIN    (7)

int main(void) {

	// Enable the GPIOA peripheral in 'RCC_AHBENR'.
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

	// A7 is connected to an LED
	// It should be set to push-pull low-speed output.
	// reset the register with 00
	GPIOA->MODER  &= ~(0x3 << (LED_PIN*2));
	// set the register to output mode
	GPIOA->MODER  |= (0x1 << (LED_PIN*2));
	// set output type register to outout push-pull
	GPIOA->OTYPER &= ~(1 << LED_PIN);
	// enable the LED
	GPIOA->ODR |= (1 << LED_PIN);

	return 0;
}
