import scipy.fftpack
import numpy as np
import matplotlib.pyplot as plt
import csv
from scipy.signal import savgol_filter
import FIRFilterWrapper
Nut_N20_x = FIRFilterWrapper.FIRFilterWrapper()
Nut_N20_y = FIRFilterWrapper.FIRFilterWrapper()
Nut_N20_z = FIRFilterWrapper.FIRFilterWrapper()


file = "MeasuredData2.csv"

T = []
accX = []
accY = []
accZ = []
filteredaccX = []
filteredaccY = []
filteredaccZ = []
postFilteredaccX = []
postFilteredaccY = []
postFilteredaccZ = []
sum_T = 0

with open(file, newline='') as csvfile:
    spanreader = csv.reader(csvfile,skipinitialspace=True, delimiter=',', quotechar='|')
    rowInd=0
    for row in spanreader:
        if rowInd==0:
            print(row)
            #print(', '.join(row))
        else:
            data = []
            for word in row:
                data.append(float(word))
            if len(T)>0 :
                sum_T += data[0]-T[-1]
            T.append(data[0])
            accX.append(data[1])
            accY.append(data[2])
            accZ.append(data[3])
            filteredaccX.append(data[4])
            filteredaccY.append(data[5])
            filteredaccZ.append(data[6])
            postFilteredaccX.append(Nut_N20_x(data[1]))
            postFilteredaccY.append(Nut_N20_y(data[2]))
            postFilteredaccZ.append(Nut_N20_z(data[3]))
        rowInd+=1

N = len(T)
deltaT = sum_T/(N-1)
print("N:",N)
print("sum_T:",sum_T)
print("deltaT:",deltaT)

fig, ax = plt.subplots()
ax.plot(T,accX,label="accX (Unfiltered)",color='r',linestyle='-')
ax.plot(T,accY,label="accY (Unfiltered)",color='b',linestyle='-')
ax.plot(T,accZ,label="accZ (Unfiltered)",color='g',linestyle='-')
ax.plot(T,filteredaccX,label="accX (Filtered)",color='r',linestyle='--')
ax.plot(T,filteredaccY,label="accY (Filtered)",color='b',linestyle='--')
ax.plot(T,filteredaccZ,label="accZ (Filtered)",color='g',linestyle='--')
ax.plot(T,postFilteredaccX,label="accX (Post Filtered)",color='r',linestyle=':')
ax.plot(T,postFilteredaccY,label="accY (Post Filtered)",color='b',linestyle=':')
ax.plot(T,postFilteredaccZ,label="accZ (Post Filtered)",color='g',linestyle=':')
plt.title("Acceleration")
plt.ylabel("Acceleration [m/s^2]")
plt.xlabel("Time [s]")
plt.legend()
plt.show()

fft_accX = scipy.fftpack.fft(accX)
fft_accY = scipy.fftpack.fft(accY)
fft_accZ = scipy.fftpack.fft(accZ)
fft_x = np.linspace(0.0, 1.0/(2.0*deltaT), N//2)

amp_accX = np.abs(fft_accX)
amp_refX = np.sqrt(np.mean(amp_accX)**2)
amp_accY = np.abs(fft_accY)
amp_refY = np.sqrt(np.mean(amp_accY)**2)
amp_accZ = np.abs(fft_accZ)
amp_refZ = np.sqrt(np.mean(amp_accZ)**2)

amp_accX_smooth = amp_accX
amp_accY_smooth = amp_accY
amp_accZ_smooth = amp_accZ

#amp_accX_smooth = savgol_filter(amp_accX, 4, 3)
#amp_accY_smooth = savgol_filter(amp_accY, 4, 3)
#amp_accZ_smooth = savgol_filter(amp_accZ, 4, 3)

amp_accX_dB = 20 * np.log10(amp_accX / amp_refX);
amp_accY_dB = 20 * np.log10(amp_accY / amp_refY);
amp_accZ_dB = 20 * np.log10(amp_accZ / amp_refZ);

fig, ax = plt.subplots()
ax.plot(fft_x, 2.0/N * np.abs(amp_accX_smooth[:N//2]), label="Acc X (Unfiltered)")
ax.plot(fft_x, 2.0/N * np.abs(amp_accY_smooth[:N//2]), label="Acc Y (Unfiltered)")
ax.plot(fft_x, 2.0/N * np.abs(amp_accZ_smooth[:N//2]), label="Acc Z (Unfiltered)")
ax.set_xscale('log')
ax.set_xlim(1e-3, 1e1)
plt.title("FFT White noise signal")
plt.ylabel("Acc [m/s^2]")
plt.xlabel("Frequency [Hz]")
plt.legend()
plt.show()
