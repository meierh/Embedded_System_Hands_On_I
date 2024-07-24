import serial
import time
import random

# Benachmark settings. These need to be the same as the settings in uart_benchmark.h
# Settings for width (number of bytes) and amount (number of reads/writes) of test data
TEST_DATA_WIDTH = 10
TEST_DATA_AMOUNT = 1000
# Baudrates to test
BAUDRATES = [9600, 19200, 57600, 115200]

##########

# Generate random bytes as test data
TEST_DATA = [random.randbytes(TEST_DATA_WIDTH) for _ in range(TEST_DATA_AMOUNT)]

# Initialise the serial interface
serial_interface = serial.Serial(
    port='/dev/serial0',  # make sure the config of the raspi is correct (see readme)
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=10,
    write_timeout=10
)


def uart_benchmark_with_baudrate(baudrate):
    # set baudrate
    serial_interface.close()
    serial_interface.baudrate = baudrate
    serial_interface.open()

    received_data = []

    time_start = time.time()
    for data in TEST_DATA:
        serial_interface.write(data)
        # wait until the send buffer is empty
        while serial_interface.out_waiting != 0:
            pass

        received = serial_interface.read(TEST_DATA_WIDTH)
        # wait until the receive buffer is empty
        while serial_interface.in_waiting != 0:
            pass

        received_data.append(received)
    time_end = time.time()

    # verify that the correct data have been received
    if TEST_DATA != received_data:
        print(f"ERROR: Test failed with baudrate {baudrate}. Received data do not match sent data.")
        if TEST_DATA_AMOUNT < 10:
            print("Expected:", TEST_DATA)
            print("Got:     ", received_data)
        exit(1)
    else:
        print(f"Test succeeded for baudrate {baudrate:6d}. Elapsed time: {time_end-time_start:2.2f} s.")


##########

print(f"UART_Benchmark started with {TEST_DATA_AMOUNT} times {TEST_DATA_WIDTH} bytes.")

for baudrate in BAUDRATES:
    uart_benchmark_with_baudrate(baudrate)

print("UART_Benchmark finished successfully.")
exit(0)