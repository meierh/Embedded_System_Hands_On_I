/*
 * Dummy main source file.
 *
 * Please add apropriate include file
 *
 */

// include CMSIS library for our Cortex-M0 processor
#include "stm32f030xc.h"

int main(void) {

	// Set mode register of port A pin 7 to 0b10 => General purpose output
	uint32_t* GPIOA_MODER_Reg =  GPIOA_BASE + GPIO_TypeDef.MODER;
	*GPIOA_MODER_Reg |=  ((uint32_t)1 << GPIO_MODER_MODER7_0 /*14*/); //set bit 14 to 1
	*GPIOA_MODER_Reg &= ~((uint32_t)1 << GPIO_MODER_MODER7_1 /*15*/); //set bit 15 to 0

	// Set output register of port A pin 7 to 1
	uint32_t* GPIOA_ODR =  GPIOA_BASE + GPIO_TypeDef.ODR;
	*GPIOA_ODR |=  ((uint32_t)1 << GPIO_ODR_7); //set bit 7 to 1
	// why is GPIO_ODR_7 = 8???

	return 0;
}
