import scipy.fftpack
import numpy as np
import matplotlib.pyplot as plt
import csv
from scipy.signal import savgol_filter

file = "NewFile11.csv"

T0 = 0
deltaT = 0
X = []
CH1 = []
CH2 = []

with open(file, newline='') as csvfile:
    spamreader = csv.reader(csvfile, delimiter=' ', quotechar='|')
    rowInd=0
    for row in spamreader:
        if rowInd==0:
            print(', '.join(row))
        elif rowInd==1:
            print(', '.join(row))
            T0 = float(row[0].split(',')[-2])
            deltaT = float(row[0].split(',')[-1])
        else:
            X.append(int(row[0].split(',')[0]))
            CH1.append(float(row[0].split(',')[1]))
            CH2.append(float(row[0].split(',')[2]))
        rowInd+=1

print("T0:",T0)
print("deltaT:",deltaT)

X = np.array(X)
CH1 = np.array(CH1)
CH2 = np.array(CH2)

N = len(X)
T = deltaT
TE = N*T-T0
print("T0:",T0)
print("T",T)
print("TE:",TE)
print("N",N)

x = np.linspace(T0, N*T, N)
fig, ax = plt.subplots()
ax.plot(x,CH1,label="Channel 1 (Unfiltered)")
ax.plot(x,CH2,label="Channel 2 (Filtered)")
plt.title("White noise signal")
plt.ylabel("Voltage [V]")
plt.xlabel("Time [s]")
plt.legend()
plt.savefig("WhiteNoiseSignal_LowPassCap_600K.png",dpi=300)

fft_CH1 = scipy.fftpack.fft(CH1)
fft_CH2 = scipy.fftpack.fft(CH2)
fft_x = np.linspace(0.0, 1.0/(2.0*T), N//2)

amp_CH1 = np.abs(fft_CH1)
amp_CH2 = np.abs(fft_CH2)
amp_ref = np.sqrt(np.mean(amp_CH1)**2)

print("amp_ref:",amp_ref)
amp_CH1_dB = 20 * np.log10(amp_CH1 / amp_ref);
amp_CH2_dB = 20 * np.log10(amp_CH2 / amp_ref);

fig, ax = plt.subplots()
ax.plot(fft_x, 2.0/N * np.abs(amp_CH1[:N//2]), label="FFT Channel 1 (Unfiltered)")
ax.plot(fft_x, 2.0/N * np.abs(amp_CH2[:N//2]), label="FFT Channel 2 (Filtered)")
ax.set_xscale('log')
ax.set_xlim(1e2, 1e4)
plt.title("FFT White noise signal")
plt.ylabel("Amp [V]")
plt.xlabel("Frequency [Hz]")
plt.legend()
plt.savefig("FFT_LowPassCap_600K.png",dpi=300)

amp_CH1_smooth = savgol_filter(amp_CH1, 30, 5)
amp_CH2_smooth = savgol_filter(amp_CH2, 30, 5)

fig, ax = plt.subplots()
ax.plot(fft_x, 2.0/N * np.abs(amp_CH1_smooth[:N//2]), label="FFT Channel 1 (Unfiltered)")
ax.plot(fft_x, 2.0/N * np.abs(amp_CH2_smooth[:N//2]), label="FFT Channel 2 (Filtered)")
ax.set_xscale('log')
ax.set_xlim(1e2, 1e4)
plt.title("Smooth FFT White noise signal (Savgol / n=30 / p=5)")
plt.ylabel("Amp [V]")
plt.xlabel("Frequency [Hz]")
plt.legend()
plt.savefig("FFT_LowPassCap_600K_smoothed.png",dpi=300)
#plt.show()
