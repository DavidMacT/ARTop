#!/usr/bin/env python3
# -*- coding: utf-8 -*-

'''
potentialbxby.py

This script reads in the Bz component files for all time
steps and produces the potential Bx and By components based
on a Fourier decomposition.

Output files are saved in the Data folder of the Data directory
of the output directory.

'''


from scipy.fft import fftn, ifftn, fftfreq, fftshift, ifftshift
import numpy as np
import os
import os.path
import sys


regionNum = sys.argv[1]
startfl=int(sys.argv[2])
endfl=int(sys.argv[3])
ly=int(sys.argv[4])      # Transpose
lx=int(sys.argv[5])
outDir = sys.argv[6]


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


s1 = os.path.join(outDir,'bz_'+regionNum+'_')            
s3='.txt'
Bxp=np.zeros((ly,lx), dtype=float)
Byp=np.zeros((ly,lx), dtype=float)
L=lx*ly


for i in range(startfl,endfl):
    print("Potential field iteration: ",i)
    s2 = str(i)
    path = os.path.join(s1+s2+s3)
    if os.path.isfile(path)==True:
        pol=np.loadtxt(path, delimiter=" ", dtype=np.float64)
        M=np.array(pol)
        BR=np.zeros((ly,lx), dtype=float)
        BR=BR+np.reshape(M,(lx,ly)).T          
        FFTB=fftshift(fftn(BR))

        Mx=FFTB * Kx
        My=FFTB * Ky

        Bxp=ifftn(ifftshift(Mx)).real
        Byp=ifftn(ifftshift(My)).real

        BxpT = Bxp
        BypT = Byp
        
        Bxp = BypT.T
        Byp = BxpT.T
        
        Bxpf=np.reshape(Bxp,(L,1))
        Bypf=np.reshape(Byp,(L,1))
        Nx = 'Bxp_'+regionNum+'_'
        Ny = 'Byp_'+regionNum+'_'

        z1 = sys.argv[6]+'/'

        z3='.txt'             
        
        a_file = os.path.join(outDir,Nx+s2+s3)
        np.savetxt(a_file, Bxpf[:])


        a_file = os.path.join(outDir,Ny+s2+s3)
        np.savetxt(a_file, Bypf[:])        


