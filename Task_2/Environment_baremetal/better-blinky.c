#include "stm32f030xc.h"
#include <stdint.h>

#define LED_0_PIN     		 (7) // PA7 (green)
#define LED_1_PIN			 (0) // PB0 (red)
#define LED_2_PIN			 (1) // PB1 (green)
#define JOYSTICK_LEFT_PIN   (11) // PB11
#define JOYSTICK_RIGHT_PIN 	 (9) // PB9
#define JOYSTICK_UP_PIN  	(10) // PB10
#define JOYSTICK_DOWN_PIN 	(13) // PB13
volatile uint32_t core_clock_hz;

// overrides the interrupt handler. The methode name is determine by CMSIS.
// see [...]/STM32CubeF0/Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/gcc/startup_stm32f030xc.s
void TIM3_IRQHandler(void) {
	if(TIM3->SR & TIM_SR_UIF) {
		TIM3->SR &= ~(TIM_SR_UIF);
		// toggle the led
		GPIOA->ODR ^= (1 << LED_0_PIN);
	}
}

// interrupt handler for the joystick
void EXTI4_15_IRQHandler(void) {
	// after detecting an interrupt we have to reset the EXTI status flag to 1
	// 1 = clear, 0 = interrupt happend
	if (EXTI->PR & (1 << JOYSTICK_LEFT_PIN)) {
		// Clear the EXTI status flag. 1=clear
		EXTI->PR |= (1 << JOYSTICK_LEFT_PIN);
		// joystick left press action
		// @todo

	} else if (EXTI->PR & (1 << JOYSTICK_RIGHT_PIN)) {
		// Clear the EXTI status flag. 1=clear
		EXTI->PR |= (1 << JOYSTICK_RIGHT_PIN);
		// joystick right press action
		// @todo

	} else if (EXTI->PR & (1 << JOYSTICK_UP_PIN)) {
		// Clear the EXTI status flag. 1=clear
		EXTI->PR |= (1 << JOYSTICK_UP_PIN);
		// joystick up press action
		// @todo

	} else if (EXTI->PR & (1 << JOYSTICK_DOWN_PIN)) {
		// Clear the EXTI status flag. 1=clear
		EXTI->PR |= (1 << JOYSTICK_DOWN_PIN);
		// joystick down press action
		// @todo

	}
}

/// setup the timer (TIM3) and enable interrupt
void setup_timer(void) {
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
	core_clock_hz = 48000000;

	// enable the TIM3 clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	// enable the NVIC inteerrupt for the TIM3
	NVIC_SetPriority(TIM3_IRQn, 0x03);
	NVIC_EnableIRQ(TIM3_IRQn);
}

/// start the given timer with the given ms as duration
// requires to setup the timer previously
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
	TIMx->DIER |= TIM_DIER_UIE;
	// enable the timer
	TIMx->CR1 |= TIM_CR1_CEN;
}

/// setup the LED
void setup_one_led(uint32_t led_pin, GPIO_TypeDef *GPIOx) {
	// A7 is connected to an LED
	// It should be set to push-pull low-speed output.
	// reset the register with 00
	GPIOx->MODER  &= ~(0x3 << (led_pin*2));
	// set the register to output mode
	GPIOx->MODER  |=  (0x1 << (led_pin*2));
	// set output type register to outout open-drain
	GPIOx->OTYPER &= ~(1 << led_pin);
	// reset PUDR to no pull-up, pull down
	//@todo why?
	GPIOx->PUPDR &= ~(3 << (led_pin*2));
}

void setup_led(void) {
	// Enable the GPIOA peripheral in 'RCC_AHBENR'. for led 0
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	// Enable the GPIOB peripheral in 'RCC_AHBENR'. for led 1 and led 2
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

	setup_one_led(LED_0_PIN, GPIOA);
	setup_one_led(LED_1_PIN, GPIOB);
	setup_one_led(LED_2_PIN, GPIOB);
}

/// turn the led on
void turn_led_on(void) {
	// enable the pin
	// ODR = output data register
	GPIOA->ODR |= (1 << LED_0_PIN);
}

/// turn the led off
void turn_led_off(void) {
	GPIOA->ODR |= (0 << LED_0_PIN);
}

/// toggle the led between on/off
void toggle_led(void) {
	// toogle between on and off
	GPIOA->ODR ^= (1 << LED_0_PIN);
}

void setup_one_direction_of_joystick(uint32_t joystick_pin) {
	// reset port mode register to input mode (00)
	GPIOB->MODER &= ~(0x3 << (joystick_pin*2));
	// reset port pull-up/pull-down register (00)
	GPIOB->PUPDR &= ~(0x3 << (joystick_pin*2));
	// set PUPDR register to pull up mode (01)
	GPIOB->PUPDR |= (0x1 << (joystick_pin*2));

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
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	setup_one_direction_of_joystick(JOYSTICK_DOWN_PIN);
	setup_one_direction_of_joystick(JOYSTICK_UP_PIN);
	setup_one_direction_of_joystick(JOYSTICK_LEFT_PIN);
	setup_one_direction_of_joystick(JOYSTICK_RIGHT_PIN);

	// Enable the NVIC interrupt for EXTI4-15 at minimum priority.
	// there are four priority levels 0 = highest, 4 = lowest
    NVIC_SetPriority(EXTI4_15_IRQn, 0x03);
    NVIC_EnableIRQ(EXTI4_15_IRQn);
}

int main(void) {

	setup_led();

	setup_joystick();

	setup_timer();

	start_timer(TIM3, 10000, core_clock_hz);

	// turn_led_off();
	// toggle_led();
	// turn_led_on();

	while(1) {

	}


	return 0;
}
