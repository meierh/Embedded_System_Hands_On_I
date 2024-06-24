#!/usr/bin/python3

import RPi.GPIO as GPIO
import time

SWCLK_PIN = 5
SWDIO_PIN = 6
PERIOD = 0.001

print("Hello World")

GPIO.setmode(GPIO.BCM)
GPIO.setup(SWCLK_PIN, GPIO.OUT)

def write(x: bool):
    GPIO.output(SWCLK_PIN, GPIO.HIGH)
    time.sleep(PERIOD/2)
    GPIO.output(SWDIO_PIN, GPIO.HIGH if x else GPIO.LOW )
    GPIO.output(SWCLK_PIN, GPIO.LOW)
    time.sleep(PERIOD/2)

def write_seq(x: int):
    binary = bin(x)[2:]
    bit_list = list(map(lambda x : True if x else False, binary) )
    for bit in bit_list:
        write(bit)

GPIO.setup(SWDIO_PIN, GPIO.OUT)

# Do nothing
#for i in range(52):
write(False)

# Line Reset
#for i in range(52):
#    write(True)

# Switch to JTAG
#write_seq(0xe79e)

# Line Reset
for i in range(50):
    write(True)
write(False)

# IDCODE request
#write(False)
#write(False)
write(True) # Start
write(False) # APnDP
write(True) # RnW
write(False) # A[2]
write(False) # A[3]
write(True) # Parity
write(False) # Stop
write(True) # Park

# finish cycle
GPIO.output(SWCLK_PIN, GPIO.HIGH)
time.sleep(PERIOD/2)

# Turnaround
GPIO.output(SWCLK_PIN, GPIO.LOW)
time.sleep(PERIOD/2)

GPIO.output(SWCLK_PIN, GPIO.HIGH)
time.sleep(PERIOD/2)


# ACK
GPIO.setup(SWDIO_PIN, GPIO.IN)

for _ in range(36):
    val = GPIO.input(SWDIO_PIN)
    print(val, end = " ")
    GPIO.output(SWCLK_PIN, GPIO.LOW)
    time.sleep(PERIOD/2)
    GPIO.output(SWCLK_PIN, GPIO.HIGH)
    time.sleep(PERIOD/2)

print()

# Turnaround
GPIO.output(SWCLK_PIN, GPIO.LOW)
time.sleep(PERIOD/2)

# IDCODE request
GPIO.setup(SWDIO_PIN, GPIO.OUT)
write(False)
write(False)
write(True) # Start
write(False) # APnDP
write(True) # RnW
write(False) # A[2]
write(False) # A[3]
write(True) # Parity
write(False) # Stop
write(True) # Park

# finish cycle
GPIO.output(SWCLK_PIN, GPIO.HIGH)
time.sleep(PERIOD/2)

# Turnaround
GPIO.output(SWCLK_PIN, GPIO.LOW)
time.sleep(PERIOD/2)

GPIO.output(SWCLK_PIN, GPIO.HIGH)
time.sleep(PERIOD/2)


# ACK
GPIO.setup(SWDIO_PIN, GPIO.IN)

for _ in range(36):
    val = GPIO.input(SWDIO_PIN)
    print(val, end = " ")
    GPIO.output(SWCLK_PIN, GPIO.LOW)
    time.sleep(PERIOD/2)
    GPIO.output(SWCLK_PIN, GPIO.HIGH)
    time.sleep(PERIOD/2)

print()

# Turnaround
GPIO.output(SWCLK_PIN, GPIO.LOW)
time.sleep(PERIOD/2)

# IDCODE request
GPIO.setup(SWDIO_PIN, GPIO.OUT)
write(False)
write(False)
write(True) # Start
write(False) # APnDP
write(True) # RnW
write(False) # A[2]
write(False) # A[3]
write(True) # Parity
write(False) # Stop
write(True) # Park

# finish cycle
GPIO.output(SWCLK_PIN, GPIO.HIGH)
time.sleep(PERIOD/2)

# Turnaround
GPIO.output(SWCLK_PIN, GPIO.LOW)
time.sleep(PERIOD/2)

GPIO.output(SWCLK_PIN, GPIO.HIGH)
time.sleep(PERIOD/2)


# ACK
GPIO.setup(SWDIO_PIN, GPIO.IN)

for _ in range(36):
    val = GPIO.input(SWDIO_PIN)
    print(val, end = " ")
    GPIO.output(SWCLK_PIN, GPIO.LOW)
    time.sleep(PERIOD/2)
    GPIO.output(SWCLK_PIN, GPIO.HIGH)
    time.sleep(PERIOD/2)

print()

for _ in range(10):
    GPIO.output(SWCLK_PIN, GPIO.LOW)
    time.sleep(PERIOD/2)
    GPIO.output(SWCLK_PIN, GPIO.HIGH)
    time.sleep(PERIOD/2)

GPIO.cleanup()


# 100 1110 1110 0010 1000 1000 1101 1101 0000
#     1110 1110 0010 1000 1000 1101 1101 0000-1

#     1000 0101 1101 1000 1000 1010 0011 1011 