#include "stm32f030xc.h"
#include <stdint.h>

#define LED_PIN    (7)

// overrides the interrupt handler. The methode name is determined by CMSIS.
// see [...]/STM32CubeF0/Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/gcc/startup_stm32f030xc.s
void TIM3_IRQHandler(void) {
	if(TIM3->SR & TIM_SR_UIF) {
		TIM3->SR &= ~(TIM_SR_UIF);
		// toggle the led
		GPIOA->ODR ^= (1 << LED_PIN);
	}
}


void start_timer(TIM_TypeDef *TIMx, uint16_t ms, uint32_t core_clock_hz){
	// make sure that the timer's counter is off
	TIMx->CR1 &= ~(TIM_CR1_CEN);
	// reset the peripheral
	if(TIMx == TIM3) {
		RCC->APB1RSTR |= (RCC_APB1RSTR_TIM3RST);
		RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM3RST);
	} else if (TIMx == TIM14) {
    	RCC->APB1RSTR |= (RCC_APB1RSTR_TIM14RST);
    	RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM14RST);
	} else if (TIMx == TIM16) {
		RCC->APB2RSTR |=  (RCC_APB2RSTR_TIM16RST);
		RCC->APB2RSTR &= ~(RCC_APB2RSTR_TIM16RST);
	} else if (TIMx == TIM17) {
		RCC->APB2RSTR |=  (RCC_APB2RSTR_TIM17RST);
		RCC->APB2RSTR &= ~(RCC_APB2RSTR_TIM17RST);
	}

	// set the timer prescaler register to count up every millisecond
	TIMx->PSC = core_clock_hz / 100;
	// set the timer auto-reload register. This is the time between the interrupt triggers
	TIMx->ARR = ms;
	// send an update event to reset the timer and apply settings
	TIMx->EGR |= TIM_EGR_UG;
	// enable the hardware interrupt
	TIMx->DIER |=TIM_DIER_UIE;
	// enable the timer
	TIMx->CR1 |= TIM_CR1_CEN;
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
	GPIOA->PUPDR   &= ~(3 << (LED_PIN*2));
}

void turn_led_on(void) {
	// enable the pin
	GPIOA->ODR |= (1 << LED_PIN);
}

void turn_led_off(void) {
	// enable the pin
	GPIOA->ODR |= (0 << LED_PIN);
}

void toggle_led(void) {
	GPIOA->ODR ^= (1 << LED_PIN);
}

int main(void) {

	setup_led();
	turn_led_off();
	//toggle_led();
	//turn_led_on();

	// reset register
	FLASH->ACR &= ~(0x00000017);
	// set one wait state and enable prefetch buffer
	// the header file only shows one bit, the reference manual three
	FLASH->ACR |= (FLASH_ACR_LATENCY | FLASH_ACR_PRFTBE );

	RCC->CFGR &= ~(RCC_CFGR_PLLMUL | RCC_CFGR_PLLSRC);

	RCC->CFGR |= (RCC_CFGR_PLLSRC_HSI_DIV2 | RCC_CFGR_PLLMUL12);

	// turn PLL on
	RCC->CR |= (RCC_CR_PLLON);
	// wait for it to be ready
	while (!(RCC->CR & RCC_CR_PLLRDY)) {};

	// selct the PLL as a system clock source
	RCC->CFGR &= ~(RCC_CFGR_SW);
	RCC->CFGR |= (RCC_CFGR_SW_PLL);
	while(!(RCC->CFGR & RCC_CFGR_SWS_PLL)) {};
	// store clockspeed in var
	uint32_t core_clock_hz = 48000000;

	// enable the TIM3 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	// enable the NVIC inteerrupt for the TIM3
	NVIC_SetPriority(TIM3_IRQn, 0x03);
	NVIC_EnableIRQ(TIM3_IRQn);

	start_timer(TIM3, 10000, core_clock_hz);

	while(1) {

	}

	return 0;
}
