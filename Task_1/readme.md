# Documentation of Task 01

## Task 1.1 Raspberry Pi

## Task 1.2 OpenOCD

## Task 1.3 Compile a Linux Kernel

#### Cross Compile on a x86 maschine

- To cross-compile the Linux kernel on a x86 maschine, we use the `gcc-arm-linux-gnu` cross-compiler. 

- First, we clone the Raspberry Pi Linux kernel repository with 
  
  - `git clone https://github.com/raspberrypi/linux.git`

- In the meantime, we install the cross-compiler package (on a fedora Linux):
  
  - `sudo dnf install gcc-arm-linux-gnu`

- To compile the kernel: 
  
  ```
  // set environment variable with the target kernel version
  KERNEL=kernel7
  // create config
  make ARCH=arm CROSS_COMPILE=arm-linux-gnu- bcm2709_defconfig
  // show config and close it afterwards
  make ARCH=arm CROSS_COMPILE=arm-linux-gnu- menuconfig
  // compile the kernel
  make ARCH=arm CROSS_COMPILE=arm-linux-gnu- -j12 zImage modules
  ```

- The config file is attached and can be found inside the file `raspi-linux-kernel-config`

- the compiling with 12 threads on a Laptop  takes about 28 min and 12 sec

- the final kernel image is then located at `[...]/linux/arch/arm/boot/Image`

### compile on the Raspberry pi
