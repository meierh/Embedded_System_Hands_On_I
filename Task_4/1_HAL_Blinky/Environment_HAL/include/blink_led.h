#ifndef __blink_led_H
#define __blink_led_H

void turn_led_on(uint8_t);
void turn_led_off(uint8_t);
void toggle_led(uint8_t);

void change_led(int8_t);
void change_period(int16_t);

void start_timer(uint16_t);
void stop_timer(void);

#endif /* __blink_led_H */
