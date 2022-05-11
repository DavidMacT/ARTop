#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from scipy.fft import fftn, ifftn, fftfreq, fftshift, ifftshift
import numpy as np
import os
import sys


regionname = sys.argv[1]
startfl=int(sys.argv[2])
endfl=int(sys.argv[3])
ly=int(sys.argv[4])
lx=int(sys.argv[5])

print(lx,ly)

freqx = fftfreq(lx)
freqy = fftfreq(ly)

kx= fftshift(freqx)
ky= fftshift(freqy)


Kx=np.zeros((ly,lx), dtype=float)*1j 
Ky=np.zeros((ly,lx), dtype=float)*1j 


for i in range(0,lx):
    for r in range(0,ly):
        if ((kx[i]**2+ky[r]**2) !=0):
            Kx[r,i]= -1j*kx[i]/np.sqrt(kx[i]**2+ky[r]**2)
            Ky[r,i]= -1j*ky[r]/np.sqrt(kx[i]**2+ky[r]**2)

#print(Kx[:6,0])
#print(Ky.shape)

#s1=sys.argv[6]+"/AR_"+regionname+"/bz_"+regionname+"_"
s1=sys.argv[6]+"/bz_"+regionname+"_"

s3=".txt"
Bxp=np.zeros((ly,lx), dtype=float)
Byp=np.zeros((ly,lx), dtype=float)
L=lx*ly


for i in range(startfl,endfl):
    print("pot field ",i,"\n")
    s2=str(i)#, base = 10, pad = 1)
    path=s1+s2+s3
#    print(os.path.isfile(path)==True)
    if os.path.isfile(path)==True:
        pol=np.loadtxt(path, delimiter=" ", dtype=np.float64)
        M=np.array(pol)
#        print(np.reshape(M,(ly,lx))[0][0:6])
        BR=np.zeros((ly,lx), dtype=float)
        BR=BR+np.reshape(M,(lx,ly)).T
#        print(BR[0:6,0])
#        print(BR.shape)          
        FFTB=fftshift(fftn(BR))
#        print(FFTB[0:3,0:3])
#        print(FFTB.shape)
        Mx=FFTB * Kx
        My=FFTB * Ky
#        print(My[0:3,0:3])
        Bxp=ifftn(ifftshift(Mx)).real
        Byp=ifftn(ifftshift(My)).real
#        print(Bxp[0][0:6])

        Bxpf=np.reshape(Bxp.T,(L,1))
        Bypf=np.reshape(Byp.T,(L,1))
        Nx="Bxp_"+regionname+"_"
        Ny="Byp_"+regionname+"_"
#        z1=sys.argv[6]+"/AR_"+regionname+"/";
        z1=sys.argv[6]+"/";

        z3=".txt";             
        
        a_file = (z1+Nx+s2+z3)
        np.savetxt(a_file, Bxpf[:])
#        print(a_file)

        a_file = (z1+Ny+s2+z3)
        np.savetxt(a_file, Bypf[:])        


