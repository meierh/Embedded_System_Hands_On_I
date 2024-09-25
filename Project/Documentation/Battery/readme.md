# Measured Battery Values

## Measurement Setup
See `V_BAT-measurement-setup.jpeg`. Eight 100 ohm resistors between 3.3V and GND connected in parallel discharge the battery. This results in a total resistance of 12.5 ohm and a charge current of about 0.26 A. The voltage of the battery is measured with a multimeter.

To measure the voltage drop under load, we disconnected the resistors and measured the battery voltage again.

## Measured Data
See `measurements.csv` (exported based on the data in `measurements.xlsx`).

## Diagrams
See `U_BAT.png` and `U_drop.png`.

## Conversion between Voltage and ADC-Value

- V_BAT(ADC) = (2 * ADC * 3.3) / 4095 
- ADC(V_BAT) = (4095 * VBAT) / 6.6

## Charge level without any additional load

While discharging:

| Charge Level | Voltage [V] | ADC-Value |
|--------------|-------------|-----------|
| 3/3 bars     | ≥ 3.91      | 2425      |
| 2/3 bars     | ≥ 3.68      | 2283      |
| 1/3 bars     | ≥ 3.51      | 2178      |
| 0/3 bars     | < 3.51      | 2178      |


Due to fluctuations in the voltage and possible drop of the ADC reference voltage, no state transistion to a higher charge level can be made while the system is powered on.

## Voltage drop under load

Measured by connecting/disconnecting peripheral devices and measuring the voltage difference.

| Load                                     | Voltage Drop [V] |
|------------------------------------------|------------------|
| RTC + Display (constant load)            | 0.015            |
| DFPlayer (constant load when turned off) | 0.001            |
| DFPlayer (turned on)                     | 0.150            | 

**Result**:
- Constant voltage drop:   
  0.016 V → 10
- Additional voltage drop when DFPlayer is turned on:
  0.150 V → 93