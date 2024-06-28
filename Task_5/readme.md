# Documentation of Task 05

## 5.1 Simulation of Analog Filters

**Simulation schematic**:
Open 1_Simulation_of_Analog_Filters/Filters/Filters.kicad_sch with KiCad

**Resistor and Capacitor values**:
Given f_LP = 1500Hz
      f_HP = 4500Hz
      L = 0.1
Calculated
      R_HP = 2*pi*L*f_HP = 900pi Ohm = 2827.43 Ohm
      C_HP = 1/(2*pi*R_HP*f_HP) = 1/(8100000pi²) Farad = 1.25088e-8 Farad
      R_LP = 2*pi*L*f_LP = 300pi Ohm = 942.48 Ohm
      C_LP = 1/(2*pi*R_LP*f_LP) = 1/(900000pi²) Farad  = 1.12579e-7 Farad
      
**Transient simulation**:
View 1_Simulation_of_Analog_Filters/LinearTransient*
General: Input signal in red, Output signal in blue

The input signal is the combination of a 1kHz sinus wave and a 5kHz sinus wave.

As expected the high pass filter both with capacitor and with coil lead to an identical output signal using the computed values. The same holds true for the low pass filter.

The low pass filter (LinearTransient1Khz5KHz_RCLP.png / LinearTransient1Khz5KHz_RLLP.png) dampens the 5kHz signal and lets the 1kHz signal through.

The high pass filter (LinearTransient1Khz5KHz_RCHP.png / LinearTransient1Khz5KHz_RLHP.png) dampens the 1kHz signal and lets the 5kHz signal through.
      
In both cases the dampening is not complete and remains of the dampened sinus wave are visible.

**Frequency response simulation**:
Parameterstudy: f_span = {500Hz, 1500Hz, 2500Hz, 3500Hz, 4500Hz, 6500Hz, 8500Hz}
                LP x HP = f_span x f_span
View LinearAC_fLP${f_span}_fHP${f_span}.png. The frequency response diagram shows the effect of the high and low pass filter. It varies according to the cut-of frequencies.

**Damping of all filter at 1kHz and 5kHz**:
High-Pass Filter
High-pass gain formula: 1/(sqrt(1+(1/(2*pi*C*R)²)))
Frequency: 1000   Cut-off Frequency:  500   Damping: -0.97 dB
Frequency: 1000   Cut-off Frequency: 1500   Damping: -5.12 dB
Frequency: 1000   Cut-off Frequency: 2500   Damping: -8.60 dB
Frequency: 1000   Cut-off Frequency: 3500   Damping: -11.22 dB
Frequency: 1000   Cut-off Frequency: 4500   Damping: -13.27 dB
Frequency: 1000   Cut-off Frequency: 6500   Damping: -16.36 dB
Frequency: 1000   Cut-off Frequency: 8500   Damping: -18.65 dB
Frequency: 5000   Cut-off Frequency:  500   Damping: -0.04 dB
Frequency: 5000   Cut-off Frequency: 1500   Damping: -0.37 dB
Frequency: 5000   Cut-off Frequency: 2500   Damping: -0.97 dB
Frequency: 5000   Cut-off Frequency: 3500   Damping: -1.73 dB
Frequency: 5000   Cut-off Frequency: 4500   Damping: -2.58 dB
Frequency: 5000   Cut-off Frequency: 6500   Damping: -4.30 dB
Frequency: 5000   Cut-off Frequency: 8500   Damping: -5.90 dB


Low-Pass Filter
Low-pass gain formula: 1/(sqrt(1+(2*pi*C*R)²))
Frequency: 1000   Cut-off Frequency:  500   Damping: -6.99 dB
Frequency: 1000   Cut-off Frequency: 1500   Damping: -1.60 dB
Frequency: 1000   Cut-off Frequency: 2500   Damping: -0.64 dB
Frequency: 1000   Cut-off Frequency: 3500   Damping: -0.34 dB
Frequency: 1000   Cut-off Frequency: 4500   Damping: -0.21 dB
Frequency: 1000   Cut-off Frequency: 6500   Damping: -0.10 dB
Frequency: 1000   Cut-off Frequency: 8500   Damping: -0.06 dB
Frequency: 5000   Cut-off Frequency:  500   Damping: -20.04 dB
Frequency: 5000   Cut-off Frequency: 1500   Damping: -10.83 dB
Frequency: 5000   Cut-off Frequency: 2500   Damping: -6.99 dB
Frequency: 5000   Cut-off Frequency: 3500   Damping: -4.83 dB
Frequency: 5000   Cut-off Frequency: 4500   Damping: -3.49 dB
Frequency: 5000   Cut-off Frequency: 6500   Damping: -2.02 dB
Frequency: 5000   Cut-off Frequency: 8500   Damping: -1.29 dB


## 5.2 Measurement of Analog Filters

## 5.3 Simulation of Digital Filters

**Frequency response of FIR filters**:
View 3_Simulation_of_Digital_Filters/*.png for Frequency response and the Matlab tool settings

**Filter coefficients of Nuttal-FIR filters**:
View 3_Simulation_of_Digital_Filters/*.fcf for filter coefficients of the different filters

**Filter coefficients of KaiserWindow-FIR filters**:
View 3_Simulation_of_Digital_Filters/KaiserWindow_Optim.fcf for filter coefficients and the order of the filter
View 3_Simulation_of_Digital_Filters/KaiserWindow_Optim.png for Frequency response
View 3_Simulation_of_Digital_Filters/KaiserWindow_Optim_Settings.png for Matlab tool settings

**Nuttal-FIR filter vs Task 5.1 Lowpass@1.5kHz**:
For N=5 and N=10 the analouge filter is the steeper one. Only N=20 leads to a digital Nuttal filter with a steeper amplitude response. This can be seen by comparing the values at f=2kHz. The analoge filter has a -12dB damping at that point while the Nuttal filter for N=5 and N=10 has a damping of > -10dB.

**Nuttal-FIR filter N=10 vs Rectangular N=10**:
Rectangular is steeper between 1.5kHz and 2kHz but has a lower damping above 2kHz, staying above -30dB for large frequency bands.

**Nuttal-FIR filter N=20 vs Kaiser Window**:
The Nuttal window N=20 filter drops to a much higher damping at high frequencies. Between 1.5kHz and 3kHz the Nuttal window filter is steeper.

## 5.4 Realization of Digital Filters

## 5.5 Choice of Filter Implementation

Advantage analoge filter:
- Can be implemented cheaply with electric components without the need for computation power
- No compilcation by the necessity to pay attention to the digital frequency
- No additional components necessary to bridge the analogue to digital gap

Advantage digital filter:
- Can be implemented by software and therefore easily changed
- Easy implementeation of high order filters only limited by memory consumption
- Components can not change their values due to age
