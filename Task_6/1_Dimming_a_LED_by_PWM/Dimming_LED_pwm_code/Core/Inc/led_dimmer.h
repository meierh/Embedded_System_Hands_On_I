#ifndef INC_6_PWM_LED_LED_DIMMER_H
#define INC_6_PWM_LED_LED_DIMMER_H


#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

void Decrease_brightness(void);

void Increase_brightness(void);

void restart_pwm(double new_brightness);
void auto_brightness(void);
void heartbeat(void);

extern uint8_t currentMode;

#ifdef __cplusplus
}
#endif

#endif //INC_6_PWM_LED_LED_DIMMER_H
