import numpy as np

f = np.array([1000,5000])
fc = np.array([500,1500,2500,3500,4500,6500,8500])
L = 0.1
pI = np.pi

def R_HP(f_HPc):
    return 2*pI*L*f_HPc

def R_LP(f_LPc):
    return 2*pI*L*f_LPc

def C_LP(f_LPc):
    return 1/(2*pI*R_LP(f_LPc)*f_LPc)

def C_HP(f_HPc):
    return 1/(2*pI*R_HP(f_HPc)*f_HPc)

def G_HP(C,R,f):
    return 1/np.sqrt(1+(1/(2*pI*f*C*R))**2)

def G_LP(C,R,f):
    return 1/np.sqrt(1+(2*pI*f*C*R)**2)

def toDezibel(frac):
    return 20*np.log10(frac)

print("High-Pass Filter")
for fi in f:
    for fci in fc:
        c_HP = C_HP(fci)
        r_HP = R_HP(fci)
        amp = G_HP(c_HP,r_HP,fi)
        ampDB = toDezibel(amp)
        print("Frequency:",'%4.f'%fi,"  Cut-off Frequency:",'%4.f'%fci,"  Damping:",'%2.2f'%ampDB,"dB")
        
print("Low-Pass Filter")
for fi in f:
    for fci in fc:
        c_LP = C_LP(fci)
        r_LP = R_LP(fci)
        amp = G_LP(c_LP,r_LP,fi)
        ampDB = toDezibel(amp)
        print("Frequency:",'%4.f'%fi,"  Cut-off Frequency:",'%4.f'%fci,"  Damping:",'%2.2f'%ampDB,"dB")
