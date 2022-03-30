#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Mar  2 11:36:10 2022
@author: khd2
"""
import matplotlib.pyplot as plt 
import numpy as np
import os
import sys

def Name_file_sr(cutoff, VS, Sampling,filenumber):
    return 'netWindDatPotFast'+str(cutoff)+'_VS'+str(VS)+'_'+str(Sampling)+'_'+str(filenumber)+'.dat'
    
    
def read_timeseries_data(path, filename):
    Name=path +'/output/'+ filename
    file=open(Name)
    upscale_missing_values={'totWindCur':[],
                            'totHelCur':[],
                            'totWindPot':[],
                            'totHelPot':[],
                            'totWindVel':[],
                            'totHelVel':[],
                            'totWind':[],
                            'totHel':[],
                            'deltaL':[],
                            'deltaH':[]
                            }
    values=[]
    for line in file:
        r=line.split()
    
        values.append(float(r[0]))
    
    
    for i in range(0,len(values),10):
        upscale_missing_values['totWindCur'].append(float(values[i]))
        upscale_missing_values['totHelCur'].append(float(values[i+1]))
        upscale_missing_values['totWindPot'].append(float(values[i+2]))
        upscale_missing_values['totHelPot'].append(float(values[i+3]))
        upscale_missing_values['totWindVel'].append(float(values[i+4]))
        upscale_missing_values['totHelVel'].append(float(values[i+5]))
        upscale_missing_values['totWind'].append(float(values[i+6]))
        upscale_missing_values['totHel'].append(float(values[i+7]))
        upscale_missing_values['deltaL'].append(float(values[i+8]))
        upscale_missing_values['deltaH'].append(float(values[i+9]))

    return upscale_missing_values
    

def acctrapz(X,dt,st_time):
    integrated=[]
    for i in range(1,len(X)):    
        y_right = X[1:i]                          # right endpoints
        y_left = X[:i-1]                          # left endpoints
        integrated.append( (dt/2) * np.sum(y_right + y_left) )
#        T.append( (dx/2)*(X[0] + (2 * sum(X[1:i])) + X[i]) )        
    
    time = np.linspace(st_time,(len(X)-1)*dt,len(integrated)) # N+1 points make N subintervals

    return np.array(integrated), time

def mean_std(Z,n_points):
#    n_points = int(n_points)
    mean = []
    std = []
    com = 0
    for i in range(0,(len(Z)-n_points)):
        mean.append(np.mean(Z[ i: i + n_points]))
        std.append(np.std(Z[ i: i + n_points]))
        if (com == 0):
            for com_ in range(n_points//2 - 1):
                mean.append(mean[0])
                std.append(std[0])
            com=1
    for con in range(len(Z) - len(mean)):
        mean.append(mean[-1])
        std.append(std[-1])

    return np.array(mean), np.array(std)

 
def plot(X,Y,color,label, ylabel,Name,path, filename_sr,unit, Xray_class = None, save=True):   

    plt.rcParams.update({'font.size':16})
    plt.figure(figsize=(12, 10))
    plt.title( Name, loc='center', fontsize = 16)

    if unit == 'h':
        u=[3600,'hour'] 
    elif unit == 'm':
        u=[60,'minute'] 
    else:
        u=[1,'seconds'] 

    c=0
    for v in Y:
        plt.plot(X/u[0],v[:len(X)], color[c], linewidth=2 ,label = label[c])
        c+=1
    
    if Xray_class is not None:
        for p in range(len(Xray_class[0])):
            peak = Xray_class[0][p]
#            plt.plot(((peak/u[0]),(peak/u[0])), (min(Y[0][:len(X)]),max(Y[0][:len(X)])), Xray_class[2])
            if (p==(len(Xray_class[0])-1)):
                plt.plot(((peak/u[0]),(peak/u[0])), (min(Y[0][:len(X)]),max(Y[0][:len(X)])), Xray_class[2], label= Xray_class[1] + ' x-ray class' )
            else:
                plt.plot(((peak/u[0]),(peak/u[0])), (min(Y[0][:len(X)]),max(Y[0][:len(X)])), Xray_class[2])
    plt.ylabel(ylabel)
    plt.xlabel('Time ('+ u[1] + ')')   
    plt.ylim([min(Y[0][:len(X)]), max(Y[0][:len(X)])])
    plt.xlim([(X[0]/u[0]), (X[-1]/u[0])])
    plt.legend()
    plt.show()

    if save is True:
        plt.savefig(path + '/generated_images/'+ Name+'_'+filename_sr[len(filename_sr)-20:len(filename_sr)-4]+'.jpg')


