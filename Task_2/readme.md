# Documentation of Task 02

Prerequisites:

- Install `gcc-arm-none-eabi` on the Raspberry Pi so that we are able to cross-compile to the Cortex-M0 from the Pi itself:
  
  ```
  sudo apt install gcc-arm-none-eabi
  ```

## Makefile

The following additions were made:

- Line 6: the path to the CMSIS library. In our repository, STM32CubeF0 is located within /utils.
- Line 44: the full command to download the compiled ELF file to the Cortex-M0. Therefore, the `stm32f0raspberry.cfg` file is needed, which we created in Task 1. Information on how the program command works can be found in the [OpenOCD Documentation](https://openocd.org/doc-release/pdf/openocd.pdf) at page 126.



## Important registers

- `MODER` mode register (p. 134)

- `PUPDR` pull-up/pull-down register (p. 135)

- `SYSCFG->EXTICR` external interrupt configuration register 1 (p. 144)

- `IMR` Interrupt mask register (p. 177)

- `RTSR` Rising trigger selection register (p. 177)

- `FTSR` Falling trigger selection register (p. 178)

- `ODR` output data register (p. 136)

- `CR1`: ‘Control Register 1’ – this register is used to enable and disable the peripheral.

- `CNT`: ‘Counter Register’ – this register hold’s the timer’s current counter value. It counts up from 0 once the timer is started.

- `PSC`: ‘Prescaler Register’ – this register will hold the timer’s prescaler. A prescaler value of `N` will tick the timer’s counter register up by one every `N+1` clock cycles.

- `ARR`: ‘Autoreload Register’ – the autoreload value is the ‘period’ of the timer. An autoreload value of `N` will cause the timer to trigger an update event every time the `CNT` register counts up to `N`.

- `EGR`: ‘Event Generation Register’ – Setting the `UG` bit in this registers resets all of the timer’s counters and tells it to use the currently-set prescaler/autoreload values.

- `DIER`: ‘DMA/Interrupt Enable Register’ – Setting the `UIE` bit
   in this register sets the timer to trigger a hardware interrupt when an
   update event occurs. Usually, that happens when the ‘Counter’ register 
  matches the ‘Autoreload’ value.

- `SR`: ‘Status Register’ – The `UIF` flag is set in this register when a timer’s hardware interrupt triggers, and must be cleared before another one can occur.

source: Reference manual and [“Bare Metal” STM32 Programming (Part 5): Timer Peripherals and the System Clock &#8211; Vivonomicon&#039;s Blog](https://vivonomicon.com/2018/05/20/bare-metal-stm32-programming-part-5-timer-peripherals-and-the-system-clock/) (23.5.24)