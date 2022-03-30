#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Mar  7 13:35:45 2022
@author: khd2
"""
import create_timeseries
import plotting
import numpy as np

'''
This script read and plot the variables in the windDatPotentialFast file. 

The variables are:
X1, X2, bz, vz, windvalCur, helvalCur, windvalPot, helvalPot,
windvalVelOnly, helvalVelOnly, wind, hel, deltaLflux, deltaHflux

For saving the image, keep save = True, otherwise change it to False

To create gif file, keep gif = True, otherwise change it to False.
'''

'''
^^^^^^^^^^
* Inputs *
^^^^^^^^^^
'''
regionName='956'
filenumber = 10
cutoff = 50
V_sampling = 20                          # velocity sampling
Sampling = 1
dt = 720                                 # time interval 
 
mapping = True
gif = False
time_series = False

title = ' winding'

path= '/home/dmac/test_codes/Data'


path, filename= plotting.name_file(path,regionName,cutoff,V_sampling,Sampling,filenumber)

nx, ny = plotting.getnxny(path)
             
'''
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
Create a map
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
'''
if mapping is True:
    variables = plotting.read_data(path,filename)
    sorted_variables= plotting.sort(variables,'X1')
    
    
    ## calculate the variable
    ## ----------------------
    bz= plotting.convert_array(sorted_variables['bz'],nx,ny)
    
    
    windvalCur= plotting.convert_array(sorted_variables['windvalCur'],nx,ny)# np.zeros((ny,nx))
    windvalPot= plotting.convert_array(sorted_variables['windvalPot'],nx,ny)
    windvalVelOnly= plotting.convert_array(sorted_variables['windvalVelOnly'],nx,ny)
    
    total_winding = windvalCur + windvalPot - windvalVelOnly
    
    
    ## Plot the variable
    ## -----------------

    plotting.plotmap(bz,nx,ny,'bz','Bz',path,filename,save=True)
    plotting.plotmap(total_winding,nx,ny,title,r'$L_w$',path,filename,save=True)
    
    
    ## Plot movie image for a variable
    ## -------------------------------
    if gif == True:
        filename = "bz_CO50_VS20_5_*.jpg"
        variable_name = 'Bz'
        plotting.plot_gif(path,filename,variable_name,regionName)

'''
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
Create time series
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
'''
filename_sr = create_timeseries.Name_file_sr(cutoff,V_sampling,Sampling,filenumber)

if time_series is True:
    
    upscale_missing_values = create_timeseries.read_timeseries_data(path,filename_sr)
    
    
    ## calculate the variable
    ## ----------------------
    total_winding_series = upscale_missing_values['totWindCur+totWindPot-totWindVel']
    deltaLflux_series = upscale_missing_values['deltaLflux']    
    total_winding_acc, time = create_timeseries.acctrapz(total_winding_series,dt,0)    
    
    #time = time/3600                                                  # convert seconds to hours
    total_winding_acc = total_winding_acc *(1e5)**4                    # converting km to cm
    
    mean, std = create_timeseries.mean_std(deltaLflux_series,n_points = 50)

    ## Plot the variable
    ## -----------------
    overplot_variables = [deltaLflux_series, mean, 3*std]
    color = ['c-', 'r-','b:']
    Label = [ r'$\partial L_w/dt$', r'$<\partial L_w/dt>\  mean$', r'$3\times \sigma( \partial L_w/dt)\ $ std']

    create_timeseries.plot(time, overplot_variables, color,Label,r'$winding$'+' '+ '$(km^4)$', title,path,filename_sr,unit='h',save=True)

'''
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
Plot X-ray flux
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
'''
import goes

# path1 = '/home/khd2/'                                         # insert the path if you want to download Goes files.
start_time = "2015-06-21 00:00"
end_time = "2015-06-22 00:00"


X=goes.goes_xray(start_time, end_time,SatelliteNumber = 15)     # select the time limits and goes satellite number

picked_time, fluxA, fluxb = X.get_data()                        #  get the data

X.plot_xray(picked_time, fluxA, fluxb)                          # plot_xray( time , fluxA , fluxb )  
 
pt, pv = X.xray_peaks(fluxb,'M')                                # xray_peaks(  wavelength  , x-ray class  ) 


## over plot xray with the time series
##------------------------------------

create_timeseries.plot(time, overplot_variables, color,Label,r'$winding$'+' '+ '$(km^4)$', title,path,filename_sr,unit='h',Xray_class=pt,save=True)
