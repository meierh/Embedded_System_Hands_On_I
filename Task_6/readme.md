# Documentation of Task 06

## 5.1 Dimming a LED by PWM


### Short explanation of the schematic
Circuit with 5V voltage source V2, the LED, a resistor R2 and a transistor. When the voltage source V3 activates the circuit the LED lights up.

### Measurements in Lab

Within `1_Dimming_a_LED_by_PWM/Lab_Code/` you'll find the code we used to perform the measurements in the Lab:

- `Onboard_LED_PWM`: This dims the LED1 on the extension board. We used it to get in touch with PWM on the M0.

- `M4_PWM`: This generates a PWM signal for the `U6` BJT on the extension board (M4). The `U6` Base is connected to the pin PA9 of the M0.

Both programs use a PWM frequency of 24 kHz. The duty cycle is initially set to 100 % and can be adjusted via the debugger, as described in both main.c files.

### KiCad project and screenshots of the simulation results
See SimulationPWM_*_100Ohm_VoltageLED_CurrentR2.png
See SimulationPWM_*_1000Ohm_VoltageLED_CurrentR2.png

### Screenshots of measurements and comparison to simulated results
See VoltageLED_100Ohm_*.png
See VoltageLED_10Ohm_*.png

The simulated base voltage was made to fit the measured one. Obviously the simulated voltage is cleaner than the measured voltage.

### Explanation or documented implementation of improved brightness step equality
