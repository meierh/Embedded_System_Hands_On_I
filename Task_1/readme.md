# Documentation of Task 01

## Task 1.1 Raspberry Pi

### Hardware preparations:

- Install Raspherry Pi Imager using `sudo apt install rpi-imager` (on Debian-based distros)

- Take sd card with reader and insert in pc

- Open "Imager" and select sd card and Raspherry Pi OS Lite 32 bit

- Press `Ctrl`+`Shift`+`X`:

- Activate Hostname

- Activate SSH - Password

- Username: pi-esho

- Password: RasPi

- Press Write

- insert the SD card in the Raspberry Pi and connect power and ethernet 

- we can now run `ssh pi-esho@raspberrypi` to connect to the Raspberry Pi

### Software preparations

- update the software with:

```bash
sudo apt update
sudo apt upgrade -y
```

## Task 1.2 OpenOCD

- We follow the instructions on the [OpenOCD GitHub Repository](https://github.com/openocd-org/openocd/)

- To build OpenOCD from source we first have to install some dependencies

```bash
sudo apt update
sudo apt upgrade
sudo apt install git build-essential libtool libtool-bin pkg-config autoconf automake texinfo
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

- Compilation succeded without any errors noticed.

- The following pins are involved in the SWD communication:

| | SWDIO | SWCLK | (Source) |
| --- | --- | --- | --- |
| Pins on the Cortex-M0 | PA13 | PA14 | [STM32F0 Reference Manual](http://www.st.com/resource/en/reference_manual/dm00031936.pdf), page 922 |
| Pin numbers of the STM32F0 on the extension board | 34 | 37 | ESA Board Documentation, PCB Schematic (U11) |
| Wires on the extension board | JTAG_SWD | JTAG_SWCLK | ESA Board Documentation, PCB Schematic (U11) |
| Pin numbers on the J1 header on the extension board / on the 40-pin header on the Raspberry Pi | 31 | 29 | ESA Board Documentation, PCB Schematic (J1) |
| GPIO pins on the Cortex-A53 | GPIO 6 | GPIO 5 | [Raspberry Pi hardware - Raspberry Pi Documentation](https://www.raspberrypi.com/documentation/computers/raspberry-pi.html) |

- Thus, GPIO 5 will be used for SWDLK and GPIO 6 for SWDIO. Our configuration file `stm32f0raspberry.cfg` looks like this:

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

- With openocd installed, we can run it with our config file. Output after running `openocd -f stm32f0raspberry.cfg`:

```
Open On-Chip Debugger 0.12.0-01004-g9ea7f3d64 (2024-05-16-10:11)
Licensed under GNU GPL v2
For bug reports, read
	http://openocd.org/doc/doxygen/bugs.html
Info : BCM2835 GPIO JTAG/SWD bitbang driver
Info : clock speed 1006 kHz
Info : SWD DPIDR 0x0bb11477
Info : [stm32f0x.cpu] Cortex-M0 r0p0 processor detected
Info : [stm32f0x.cpu] target has 4 breakpoints, 2 watchpoints
Info : starting gdb server for stm32f0x.cpu on 3333
Info : Listening on port 3333 for gdb connections
    TargetName         Type       Endian TapName            State       
--  ------------------ ---------- ------ ------------------ ------------
 0* stm32f0x.cpu       cortex_m   little stm32f0x.cpu       running

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
