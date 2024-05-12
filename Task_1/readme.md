# Documentation of Task 01

## Task 1.1 Raspberry Pi

### Hardware preparations:

- Install Raspherry pi imager using sudo apt install rpi-imager

- Take sd card with reader and insert in pc

- Open imager and select sd card and Raspherry pi os lite 32 bit

- Press Str-Shift-X:

- Activate Hostname

- Activate SSH - Password

- Username: pi

- Password: RasPi

- Press Write

- insert the SD card in the Raspherry pi and connect power and ethernet 

- we can now run `ssh pi@raspberrypi` to connect to the raspberry pi

### software preparations

- update the software with:

```bash
sudo apt update
sudo apt upgrade -y
```

## Task 1.2 OpenOCD

- We follow the instructions on the [Repository]([GitHub - openocd-org/openocd: Official OpenOCD Read-Only Mirror (no pull requests)](https://github.com/openocd-org/openocd/))
- to build openOCD from source we first have to install some dependencies

```bash
sudo apt update
sudo apt upgrade
sudo apt install git build-essential libtool-bin pkg-config autoconf automake texinfo
```

- after that we can clone the repository, configure openocd to enable the `bcm2835gpio` interface, build and install openocd

```bash
# clone the openocd repository
git clone https://github.com/openocd-org/openocd.git
# navigate into the folder
cd openocd
# checkout the latest release v0.12.0
git checkout 9ea7f3d647c8ecf6b0f1424002dfc3f4504a162
# needed when building from the git repository
./bootstrap
# generates the Makefiles required to build. We enable the bcm2835gpio interface
./configure --enable-bcm2835gpio
# build openocd
make
# install it
sudo make install
```

- with openocd installed, we can run it with the config file:

- FILE:stm32f0raspberry.cfg:

```stm32f0raspberry.cfg:
adapter driver bcm2835gpio

adapter gpio swdio 6
adapter gpio swclk 5

transport select swd

source [find target/stm32f0x.cfg]

bindto 0.0.0.0

init
targets
```

- The following Pins are involved in the SWD communication:
- @todo check the pins on the Cortex-A53, the Raspberry [Pi], the extension board, and the Cortex-M0 involved in the SWD communication

| Name                  | Pin  | Source                                                                                                                                                    |
| --------------------- | ---- | --------------------------------------------------------------------------------------------------------------------------------------------------------- |
| SWD  on Raspberry pi  | 6    | [Raspberry pi documentation]([Raspberry Pi hardware - Raspberry Pi Documentation](https://www.raspberrypi.com/documentation/computers/raspberry-pi.html)) |
| SWCLK on Raspberry pi | 5    |                                                                                                                                                           |
| SWCLK on STM32f0      | PA14 | [ STM32f0 Manual](http://www.st.com/resource/en/reference_manual/dm00031936.pdf) page 151                                                                 |
| SWDIO on STM32f       | PA13 |                                                                                                                                                           |

- outout after running `openocd -f stm32f0raspberry.cfg`

```bash
Open On-Chip Debugger 0.12.0+dev-01573-gbc9ca5f4a (2024-05-08-12:11)
Licensed under GNU GPL v2
For bug reports, read
        http://openocd.org/doc/doxygen/bugs.html
Info : BCM2835 GPIO JTAG/SWD bitbang driver
Info : clock speed 997 kHz
Info : SWD DPIDR 0x0bb11477
Info : [stm32f0x.cpu] Cortex-M0 r0p0 processor detected
Info : [stm32f0x.cpu] target has 4 breakpoints, 2 watchpoints
Info : [stm32f0x.cpu] Examination succeed
Info : starting gdb server for stm32f0x.cpu on 3333
Info : Listening on port 3333 for gdb connections
    TargetName         Type       Endian TapName            State       
--  ------------------ ---------- ------ ------------------ ------------
 0* stm32f0x.cpu       cortex_m   little stm32f0x.cpu       unknown
Info : Listening on port 6666 for tcl connections
Info : Listening on port 4444 for telnet connections
```

## Task 1.3 Compile a Linux Kernel

#### Cross Compile on a x86 machine

- To cross-compile the Linux kernel on a x86 machine, we use the `gcc-arm-linux-gnu` cross-compiler. 

- First, we clone the Raspberry Pi Linux kernel repository with 
  
  ```bash
  git clone https://github.com/raspberrypi/linux.git
  ```

- In the meantime, we install the cross-compiler package (on a fedora Linux):
  
  ```bash
  sudo dnf install gcc-arm-linux-gnu
  ```

- To compile the kernel: 
  
  ```bash
  # set environment variable with the target kernel version
  KERNEL=kernel7
  # create config
  make ARCH=arm CROSS_COMPILE=arm-linux-gnu- bcm2709_defconfig
  # show config and close it afterwards
  make ARCH=arm CROSS_COMPILE=arm-linux-gnu- menuconfig
  # compile the kernel
  make ARCH=arm CROSS_COMPILE=arm-linux-gnu- -j12 zImage modules
  ```

- The config file is attached and can be found inside the file `raspi-linux-kernel-config`

- the compiling with 12 threads on a Laptop  takes about 28 min and 12 sec

- the final kernel image is then located at `[...]/linux/arch/arm/boot/Image`

### compile on the Raspberry pi

- To compile the kernel
  
  ```bash
  # set environment variable with the target kernel version
  KERNEL=kernel7
  #create config
  make ARCH=arm bcm2709_defconfig
  #show config and close it afterwards
  make ARCH=arm menuconfig
  # compile the kernel
  make ARCH=arm -j2 zImage modules
  ```

- 
