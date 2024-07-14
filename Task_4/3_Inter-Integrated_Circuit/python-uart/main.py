import serial
import time

serial_interface = serial.Serial(
    port='/dev/serial0', # make sure the config of the raspi is correct (see readme)
    baudrate = 115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)

counter = 0

while True:
    received_data = serial_interface.read()              # read serial port
    time.sleep(0.03)
    data_left = serial_interface.inWaiting()             # check for remaining byte
    received_data += serial_interface.read(data_left)
    print(received_data)
