# Task 4

## 4.1 Bettery blinky with HAL

## 4.2 UART

To use UART, we need to enable it first

- open the raspberry pi config with: `sudo raspi-config` 

- select `3. Interace options`

- select `Serial Port`

- It will ask you two questions. 
  
  1. `Would you like a login shell to be accessible over serial?` -> select `No` 
  
  2. `Would you like the serial port hardware to be enabled?` -> select `Yes`

. You should see the following message:

  . <img src="assets/2024-06-26-10-23-43-image.png" title="" alt="" width="352">

- select `OK`

- select `finish and reboot`

The involved GPIO pins on the raspberry pi for the UART communication are

- 15 (rx = receive)

- 14 (tx = transmit)

On our extension board, they are connected to USART 5 on the M0

On the raspberry pi we can now select `/dev/serial0` as UART port. 

**Note:**  `/dev/serial0` is a symbolic link to `/dev/ttyS0`

For more information, see the official documentation ([Configuration - Raspberry Pi Documentation](https://www.raspberrypi.com/documentation/computers/configuration.html#configure-uarts))

### Python Software Preparations on the Raspberry Pi

We use the `pyserial` python libaray, which can be installed via `pip` in the following way:

```bash
# Change to your home folder
cd 

# Install python3 and pip
sudo apt install python3-full pip

# Create and activate a virtual environment
python3 -m venv .venv
source .venv/bin/activate

# Install pyserial
pip3 install pyserial
```

### Execute Benchmark

1. Compile/Run the M0 Code normally

2. Start the Python Code for the Raspberry Pi (`Code-raspi/main.py`). The script `raspi-run.sh` can be used to simplify this when you are on your host machine:  
   ```
   ./raspi-run.sh <Raspberry Pi's Hostname/IP/SSH-Host-Name> <Raspberry Pi's username>
   ```
   Note: Within CLion, the Build Target "Shell Script" can be used to call the script. Be sure to set the working directory to 2_Universal_Asynchronous_Receiver_Transmitter.

Testing UART with 1000x10 Bytes:
```
UART_Benchmark started with 1000 times 10 bytes.
Test succeeded for baudrate   9600. Elapsed time: 20.95 s.
Test succeeded for baudrate  19200. Elapsed time: 10.53 s.
Test succeeded for baudrate  57600. Elapsed time: 3.56 s.
Test succeeded for baudrate 115200. Elapsed time: 1.82 s.
UART_Benchmark finished successfully.
```

We can observe that the time required for the communication approximately decreases linearly to the baudrate.

### Side note about USART vs UART

**UART** stands for **U**niversal **A**synchronous **R**eceiver **T**ransmitter whereas the **USART** stands for **U**niversal **S**ynchronous **A**synchronous **R**eceiver **T**ransmitter.
 The term Synchronous enables the USART to send an additional clock 
signal to the receiving device. The data is then sampled at a predefined
 edge (Rising or Falling) of the clock. The USART mode uses **3 pins** (clock, Tx and Rx) compared to the **2 pins** (Tx and Rx) used in the UART. [Source 26.6.24](https://controllerstech.com/stm32-uart-1-configure-uart-transmit-data/)

## Task 3

### Addresses and datasheets 

**v3 Sensor Board**:

| Sensor                                   | Datasheet                                             | Slave Address (default) (7bit) | read address (8bit) | write address (8bit) |
|------------------------------------------|-------------------------------------------------------|--------------------------------|---------------------|----------------------|
| LPS331AP (barometer)                     | [URL](https://www.pololu.com/file/0J622/LPS331AP.pdf) | 1011101b                       | 10111011b           | 10111010b            |
| LSM404D (accelerometer and magnetometer) | [URL](https://www.pololu.com/file/0J703/LSM303D.pdf)  | 0011101b                       | 00111011b           | 00111010b            |
| L3GD20H (gyro)                           | [URL](https://www.pololu.com/file/0J731/L3GD20H.pdf)  | 1101011b                       | 11010111b           | 11010110b            |

Note: The L3GD20H sensor will not be used in this task, as reading gyroscope measurements is not required.

**v5 Sensor Board**:

| Sensor                            | Datasheet                                                       | Slave Address (default) (7bit)            | read address (8bit)                       | write address (8Bit) |
|-----------------------------------|-----------------------------------------------------------------|-------------------------------------------|-------------------------------------------|----------------------|
| LSM6DS33 (gyro and accelerometer) | [URL](https://www.pololu.com/file/0J1087/LSM6DS33.pdf)          | 1101011b                                  | 11010111 (D7h)                            | 11010110 (D6h)       |
| LIS3MDL (magnetometer)            | [URL](https://www.pololu.com/file/0J1089/LIS3MDL.pdf)           | 0011110b                                  | 00111101 (3Dh)                            | 00111100 (3Ch)       |
| LPS25H (barometer)                | [URL](https://www.pololu.com/file/0J761/LPS25H.pdf)             | 1011101b                                  | 10111011 (BBh)                            | 10111010 (BAh)       |

*The source code was partly inspired by the official Arduino libraries for the sensors.*

- [GitHub - pololu/lsm6-arduino: Arduino library for Pololu LSM6DS33 and LSM6DSO boards](https://github.com/pololu/lsm6-arduino/tree/master) (under MIT License)
- [GitHub - pololu/lps-arduino: Arduino library for Pololu LPS25H and LPS331AP boards](https://github.com/pololu/lps-arduino) (under MIT License)
- [GitHub - pololu/lis3mdl-arduino: Pololu Arduino library for LIS3MDL magnetometer](https://github.com/pololu/lis3mdl-arduino)  (under MIT License)
- [GitHub - pololu/lsm303-arduino](https://github.com/pololu/lsm303-arduino) (Under MIT License)