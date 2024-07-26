#ifndef __blink_led_H
#define __blink_led_H

#define DEFAULT_PERIOD_MS      (1000) // all leds start blinking with this period
#define PERIOD_IN_DECREMENT_MS   (50) // when the joystick is moved up/down, the period is changed by this value
#define PERIOD_LOWER_BOUND_MS    (50) // the blinking period can not be changed below this value
#define PERIOD_UPPER_BOUND_MS (10000) // the blinking period can not be changed above this value

void initialise();

static void turn_led_on(uint8_t);
static void turn_led_off(uint8_t);
static void toggle_led(uint8_t);

static void change_led(int8_t);
static void change_period(int16_t);

static void start_timer(uint16_t);
static void stop_timer(void);

#endif /* __blink_led_H */
