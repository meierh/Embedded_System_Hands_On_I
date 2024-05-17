/*
 * Dummy main source file.
 *
 * Please add apropriate include file
 *
 */

// include CMSIS library for our Cortex-M0 processor
#include "stm32f030xc.h"

int main(void) {
	/*
	unsigned int i = 0;
	while (1) {
		i ++;
	}
	*/

	// Set mode register of port A pin 7 to 0b10 => General purpose output
	GPIOA->MODER = 0;
	GPIOA->MODER |= GPIO_MODER_MODER7_0;

	// Set output register of port A pin 7 to 1
	GPIOA->ODR = 0;
	GPIOA->ODR |= GPIO_ODR_7; 

	return 0;
}
