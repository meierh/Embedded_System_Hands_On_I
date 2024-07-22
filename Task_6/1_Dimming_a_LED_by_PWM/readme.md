# Documentation of Task 06

## 5.1 Dimming a LED by PWM

### Simulation

### Measurements in Lab

Within `1_Dimming_a_LED_by_PWM/Lab_Code/` you'll find the code we used to perform the measurements in the Lab:

- `Onboard_LED_PWM`: This dims the LED1 on the extension board. We used it to get in touch with PWM on the M0.

- `M4_PWM`: This generates a PWM signal for the `U6` BJT on the extension board (M4).

Both programs use a PWM frequency of 24 kHz. The duty cycle is initially set to 100 % and can be adjusted via the debugger, as described in both main.c files.