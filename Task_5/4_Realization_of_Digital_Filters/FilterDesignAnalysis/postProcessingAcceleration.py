import scipy.fftpack
import numpy as np
import matplotlib.pyplot as plt
import csv
from scipy.signal import savgol_filter
import FIRFilterWrapper
Nut_N20_x = FIRFilterWrapper.FIRFilterWrapper()
Nut_N20_y = FIRFilterWrapper.FIRFilterWrapper()
Nut_N20_z = FIRFilterWrapper.FIRFilterWrapper()


file = "AccelerationDataTrack.csv"

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
print("Frequency:",1/deltaT)


UnfilteredAccX = (accX,"accX (Unfiltered)",'r','-')
UnfilteredAccY = (accY,"accY (Unfiltered)",'r','-')
UnfilteredAccZ = (accZ,"accZ (Unfiltered)",'r','-')

FilteredAccX = (filteredaccX,"accX (Filtered)",'b','-')
FilteredAccY = (filteredaccY,"accY (Filtered)",'b','-')
FilteredAccZ = (filteredaccZ,"accZ (Filtered)",'b','-')

PostFilteredAccX = (postFilteredaccX,"accX (Post Filtered)",'g','-')
PostFilteredAccY = (postFilteredaccY,"accY (Post Filtered)",'g','-')
PostFilteredAccZ = (postFilteredaccZ,"accZ (Post Filtered)",'g','-')


def plotData(data,name):
    fig, ax = plt.subplots()
    ax.yaxis.grid(color='gray', linestyle='dashed')
    for item in data:
        values = item[0]
        label = item[1]
        color = item[2]
        linestyle = item[3]
        ax.plot(T,values,label=label,color=color,linestyle=linestyle)
    plt.title("Acceleration")
    plt.ylabel("Acceleration [m/s^2]")
    plt.xlabel("Time [s]")
    plt.legend()
    plt.savefig(name,dpi=600)

def plotFFT(data,name):
    fig, ax = plt.subplots()
    ax.yaxis.grid(color='gray', linestyle='dashed')
    fft_x = np.linspace(0.0, 1.0/(2.0*deltaT), N//2)
    for item in data:
        values = item[0]
        label = item[1]
        color = item[2]
        linestyle = item[3]
        fft = scipy.fftpack.fft(values)
        amp_fft = fft #np.abs(fft)
        ax.plot(fft_x, 2.0/N * np.abs(amp_fft[:N//2]),label=label,color=color,linestyle=linestyle)
    ax.set_xscale('log')
    ax.set_ylim(-0.25, 2.5)
    ax.set_xlim(1e-4, 0.6e2)
    plt.title("FFT")
    plt.ylabel("Acceleration [m/s^2]")
    plt.xlabel("Frequency [Hz]")
    plt.legend()
    plt.savefig(name,dpi=600)

plotData([UnfilteredAccX,FilteredAccX],"TransientPlotX.png")
plotData([UnfilteredAccY,FilteredAccY],"TransientPlotY.png")
plotData([UnfilteredAccZ,FilteredAccZ],"TransientPlotZ.png")

plotFFT([UnfilteredAccX,FilteredAccX],"FFTPlotX.png")
plotFFT([UnfilteredAccY,FilteredAccY],"FFTPlotY.png")
plotFFT([UnfilteredAccZ,FilteredAccZ],"FFTPlotZ.png")
