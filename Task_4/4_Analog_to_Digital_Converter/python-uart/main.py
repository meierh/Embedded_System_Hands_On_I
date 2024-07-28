import serial
import time
import sys

# Message from M0 which indicates the end of communication. Script will exit when this message is received.
GOODBYE_MSG = ">>> End of measurements."

# Initialise the serial interface
serial_interface = serial.Serial(
    port='/dev/serial0',  # make sure the config of the raspi is correct (see readme)
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=None,
    write_timeout=10
)

num_measurements = 0  # number of measurements to request
additional_delay = 0  # delay between measurements

# Get these values from the argv (if command line args are given) or ask interactively
if len(sys.argv) == 3:
    num_measurements = sys.argv[1]
    additional_delay = sys.argv[2]
else:
    num_measurements = input("Number of measurements?                > ")
    additional_delay = input("Additional delay between measurements? > ")

# Convert read values to ints or fall back to default values
try:
    num_measurements = abs(int(num_measurements))
    additional_delay = abs(int(additional_delay))
except ValueError:
    num_measurements = 10
    additional_delay = 1000
    print(">>> Warning: Invalid input. Falling back to default values.", file=sys.stderr)

# send values to M0
serial_interface.write(f"{num_measurements}\n".encode('ascii'))
while serial_interface.out_waiting != 0:
    pass
serial_interface.write(f"{additional_delay}\n".encode('ascii'))
while serial_interface.out_waiting != 0:
    pass

# get and print data received via UART
while True:
    data = serial_interface.readline().decode('ascii').rstrip()

    if data.startswith(">>>"):
        # print additional outputs to stderr s.t. they do not clutter the CSV file
        print(data, file=sys.stderr)
    else:
        print(data)

    if data == GOODBYE_MSG:
        break

exit(0)