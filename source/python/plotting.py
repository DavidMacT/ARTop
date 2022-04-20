#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt 
import numpy as np
import os
import sys

def sort(dic, key):
    numbered=dict()
    var_col=dic[key]
    for n in range(len(var_col)):
        numbered[n]= var_col[n]
    
    ss=dict(sorted(numbered.items(), key=lambda item: item[1]))
    
    sorted_dic=dict()
    lis = list(ss.keys())
 #   inserted=dic
    lis_=[]
    for i in  list(dic.keys()):
        lis_=[]
        for v in lis:
            lis_.append(dic[i][v]) 
        sorted_dic[i]=lis_
  #      
    return sorted_dic

def plot_limit(Variable):
    V=[]
    for i in np.array(Variable).T:
        V.append(i)
    mx = np.max(V)
    mn = np.min(V)

    return (mn, mx) 

def convert_array(M,nx,ny):
    Z = np.zeros((ny,nx))
    s=0
    for j in range(nx):    
        for i in range(ny):
            Z[i,j]= M[i+s]
        s=s+i+1
    return Z

def creat_mesh(x,shape):
    M=[]
    for d in range(shape[1]):
        M.append(x)
    return np.array(M).T
       
def plot(X,Y,Z, Name, symbol,output,save=True):
    
    co_rang_st = plot_limit(Z)[0]
    co_rang_ed = plot_limit(Z)[1]
    
    tim_rang_st = 81.5
    tim_rang_ed = 84.5
    
    dis_rang_st = 1.00 
    dis_rang_ed = 12.1
    ##=============================================================================
    v = np.linspace(co_rang_st, co_rang_ed, 60, endpoint=True)
    
    plt.rcParams.update({'font.size':16})
    plt.figure(figsize=(12, 10))
    plt.contourf(X,Y,Z,v,cmap='nipy_spectral')
    plt.clim(co_rang_st , co_rang_ed)
    
    c = np.linspace(co_rang_st, co_rang_ed, 10, endpoint=True)
    plt.colorbar(ticks=c)
    
    plt.xlabel('X pixel')
    plt.ylabel('Y pixel')

    plt.title( symbol, loc='center', fontsize = 16)
    if save is True:
        plt.savefig(output+'/'+ Name+'_'+insert[len(insert)-20:len(insert)-4]+'.jpg')
    
#    plt.show()    


#path='/home/khd2/Glasgow/Data/output/' 
insert= sys.argv[3]#path+'windDatPotentialFastCO50_VS20_5_1063.dat' #sys.argv[3]
file=open(insert)

variables={
        'X1':list(),
        'X2':list(),
        'bfield':[],
        'vfield': [],
        'windvalCur': [],
        'helvalCur': [],
        'windvalPot': [],
        'helvalPot': [],
        'windvalVelOnly': [],
        'helvalVelOnly': [],
        'wind': [],
        'hel': [],
        'deltaLflux': [],
        'deltaHflux': []
    }
upscale_missing_values=dict()

for line in file:
    r=line.split()
    if len(r)!=1:
        variables['X1'].append(float(r[0]))
        variables['X2'].append(float(r[1]))
        variables['bfield'].append(float(r[2]))
        variables['vfield'].append(float(r[3]))
        variables['windvalCur'].append(float(r[4]))
        variables['helvalCur'].append(float(r[5]))
        variables['windvalPot'].append(float(r[6]))
        variables['helvalPot'].append(float(r[7]))
        variables['windvalVelOnly'].append(float(r[8]))
        variables['helvalVelOnly'].append(float(r[9]))
        variables['wind'].append(float(r[10]))
        variables['hel'].append(float(r[11]))
        variables['deltaLflux'].append(float(r[12]))
        variables['deltaHflux'].append(float(r[13]))
        
    else:
        upscale_missing_values['totWindCur']= float(r[0])
        line=file.read()
        break

r=line.split()
upscale_missing_values['totHelCur']= float(r[-9])
upscale_missing_values['totWindPot']= float(r[-8])
upscale_missing_values['totHelPot']= float(r[-7])
upscale_missing_values['totWindVel']= float(r[-6])
upscale_missing_values['totHelVel']= float(r[-5])
upscale_missing_values['totWind_Cur_Pot_Vel']= float(r[-4])
upscale_missing_values['totHel_Cur_Pot_Vel']= float(r[-3])
upscale_missing_values['deltaLflux']= float(r[-2])
upscale_missing_values['deltaHflux']= float(r[-1])





sorted_variables= sort(variables,'X1')


nx= int(sys.argv[1])#138
ny= int(sys.argv[2])#90
    
X= convert_array(sorted_variables['X1'],nx,ny)
Y= convert_array(sorted_variables['X2'],nx,ny)
bz= convert_array(sorted_variables['bfield'],nx,ny)


windvalCur= convert_array(sorted_variables['windvalCur'],nx,ny)# np.zeros((ny,nx))
windvalPot= convert_array(sorted_variables['windvalPot'],nx,ny)
windvalVelOnly= convert_array(sorted_variables['windvalVelOnly'],nx,ny)


total_winding = windvalCur + windvalPot - windvalVelOnly
#Z=total_winding

output=sys.argv[4]#'/home/khd2/Downloads/Data/AR_956'
plot(X,Y,bz,'bz','Bz',output)
plot(X,Y,total_winding,'Total_winding',r'$L_w$',output)

#plt.show()
