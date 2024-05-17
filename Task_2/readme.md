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