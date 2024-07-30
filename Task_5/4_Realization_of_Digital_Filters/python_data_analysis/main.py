import pandas as pd
import matplotlib.pyplot as plt

# read CSV-file
data = pd.read_csv('../measurements.csv')

# select rows
a_x_raw = data['a_x_raw [m/s^2]']
a_y_raw = data['a_y_raw [m/s^2]']
a_z_raw = data['a_z_raw [m/s^2]']

a_x_filtered = data['a_x_filtered [m/s^2]']
a_y_filtered = data['a_y_filtered [m/s^2]']
a_z_filtered = data['a_z_filtered [m/s^2]']

# create plot
plt.plot(a_x_raw, label='a_x_raw [m/s^2]')
plt.plot(a_y_raw, label='a_y_raw [m/s^2]')
plt.plot(a_z_raw, label='a_z_raw [m/s^2]')

plt.plot(a_x_filtered, label='a_x_filtered [m/s^2]')
plt.plot(a_y_filtered, label='a_y_filtered [m/s^2]')
plt.plot(a_z_filtered, label='a_z_filtered [m/s^2]')

# add labels
plt.xlabel('Zeit')
plt.ylabel('m/s^2')

# show legend
plt.legend()

# show plot
plt.show()