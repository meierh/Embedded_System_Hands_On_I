# Documentation of Task 03

## Task 3.2 

### Software preparations
- Install on host
  ```bash
  sudo apt-get install gcc-arm-none-eabi
  sudo apt-get install gdb-multiarch
  ```
- Download gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2 from https://developer.arm.com/downloads/-/gnu-rm
- Extract via sudo tar xjf gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2
- Execute ./arm-none-eabi-gdb

- Start openocd on Raspberry pi by running:
  ```bash
  openocd -f stm32f0raspberry.cfg
  
  Result:
  Open On-Chip Debugger 0.12.0-01004-g9ea7f3d64 (2024-05-08-19:50)
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
- Connect using target remote tcp:hostIP:port (port given on Raspberry pi openocd
  ```bash
  (gdb) target remote tcp:192.168.178.48:3333

  Result:
  Remote debugging using tcp:192.168.178.48:3333
  warning: No executable has been specified and target does not support
  determining executable automatically.  Try using the "file" command.
  0x080092a0 in ?? ()
  ```
- At host: 
  ```bash
  Info : accepting 'gdb' connection on tcp/3333
  [stm32f0x.cpu] halted due to debug-request, current mode: Thread 
  xPSR: 0x41000000 pc: 0x080092a0 psp: 0x20001cc0
  Info : device id = 0x10006442
  Info : flash size = 256 KiB
  Warn : Prefer GDB command "target extended-remote :3333" instead of "target remote :3333"
  ```
  
- Test
  ```bash
  (gdb) monitor reset halt
  [stm32f0x.cpu] halted due to debug-request, current mode: Thread 
  xPSR: 0xc1000000 pc: 0x080000d0 msp: 0x20000400
  ```
- At host
  ```bash
  [stm32f0x.cpu] halted due to debug-request, current mode: Thread 
  xPSR: 0xc1000000 pc: 0x080000d0 msp: 0x20000400
  ```

  
## Task 3.3

### Software preparations
- Install eclipse: 
- Download eclipse installer from https://www.eclipse.org/downloads/packages/installer
- Extract installer 
  ```bash
  tar -xf eclipse-inst-jre-linux64.tar.gz
  ```
- Run installer 
  ```bash
  ./eclipse-inst
  ```
- Select: Eclipse IDE for Embedded C/C++ Developers

-Connect according to https://eclipse-embed-cdt.github.io/debug/openocd/
