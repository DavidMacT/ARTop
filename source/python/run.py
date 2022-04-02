#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Apr  2 13:43:01 2022

@author: khd2
"""
import winding as win
import goes

'''
^^^^^^^^^^
* Inputs *
^^^^^^^^^^
'''
activeRegion = 956
filenumber = 1200
dt = 720

path_= '/home/khd2/Glasgow/Glasgow_combined/Data/'

'''
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
Create a map
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
'''

Map =win.mapping(activeRegion,path_)

bz = Map.read_data('bz', filenumber)
Map.plotmap(bz,title=r'$L_w$')


'''
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
Create time series
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
'''

TS = win.timeseries(activeRegion, path_)
f = TS.read_data(filenumber)

deltaLflux_series, time = TS.integrate(f['totWindCur+totWindPot-totWindVel'],dt,0)
mean, std = TS.mean_std(deltaLflux_series,n_points = 50)

overplot_variables = [deltaLflux_series, mean, 3*std]
color = ['c-', 'r-','b:']
Label = [ r'$\partial L_w/dt$', r'$<\partial L_w/dt>\  mean$', r'$3\times \sigma( \partial L_w/dt)\ $ std']


TS.plot(time, overplot_variables, color,Label,r'$winding$'+' '+ '$(km^4)$', 'test',unit='h')
'''
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
Plot X-ray flux
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
'''

# path1 = '/home/khd2/'                                         # insert the path if you want to download Goes files.
start_time = "2015-06-21 00:00"
end_time = "2015-06-22 00:00"


X=goes.goes_xray(start_time, end_time,SatelliteNumber = 15)     # select the time limits and goes satellite number

picked_time, fluxA, fluxb = X.get_data()                        #  get the data

X.plot_xray(picked_time, fluxA, fluxb)                          # plot_xray( time , fluxA , fluxb )  
 
pt, pv = X.xray_peaks(fluxb,'M')                                # xray_peaks(  wavelength  , x-ray class  ) 


## over plot xray with the time series
##------------------------------------
#TS.plot(time, overplot_variables, color,Label,r'$winding$'+' '+ '$(km^4)$', 'test',unit='h',save=True)

TS.plot(time, overplot_variables, color,Label,r'$winding$'+' '+ '$(km^4)$', 'test',unit='h',Xray_class=pt)
