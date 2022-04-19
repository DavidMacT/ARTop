#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Apr  2 13:43:01 2022

@author: khd2
"""
import winding as win
import observation

'''
^^^^^^^^^^
* Inputs *
^^^^^^^^^^
'''
activeRegion = 956
filenumber = 200
dt = 720

path_= '/media/khd2/Spare-Data-Disk/ARTop_winding/Data'

'''
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
Create a map
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
'''

Map =win.mapping(activeRegion,path_)

bz = Map.read_data('bz', filenumber)
Map.plotmap(bz,title=r'$B_z$')

## Creat gif file
##---------------

im_st_No = 150
im_ed_No = 300
variable_name = 'bz'  

#X=Map.plot_gif(im_st_No, im_ed_No, variable_name, title = 'bz')
'''
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
Create time series
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
'''

TS = win.timeseries(activeRegion, path_)

L = TS.read_data( 'totWind_Cur_Pot_Vel')
deltaL = TS.read_data('deltaLflux')

intL, time = TS.integrate_accum('totWind_Cur_Pot_Vel',dt)


# scale factors
int_factor = 1.0e5**4                # cm^4
factor = int_factor*720.0            # cm^4 s
intL = intL*int_factor
deltaL = deltaL*factor


mean, mstd = TS.mean_std(intL,n_points = 2,factor = 3)


overplot_variables = [intL,mean,mstd]
colour = ['c-','r-','b:']
label = ['$\dot{\delta L}$', '$\mu(\dot{\delta L})$', '$\mu+3\sigma$']

y_label = r'$winding$'+' '+ '$(km^4)$'
title = 'test'
X=TS.plot(time, overplot_variables, colour,label,y_label, 'test',unit='h')


'''
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
Plot X-ray flux
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
'''

# path1 = '/home/khd2/'                                         # insert the path if you want to download Goes files.
start_time = "2021-06-18 18:00"
end_time = "2021-06-20 00:00"


X=observation.goes_xray(start_time, end_time,SatelliteNumber = 16)     # select the time limits and goes satellite number

picked_time, fluxA, fluxb = X.get_data()                        #  get the data

X.plot_xray(picked_time, fluxA, fluxb)                          # plot_xray( time , fluxA , fluxb )  
 
pt, pv = X.xray_peaks(fluxb,'c')                                # xray_peaks(  wavelength  , x-ray class  ) 


## over plot xray with the time series
##------------------------------------
y_label = r'$winding$'+' '+ '$(km^4)$'
title = 'test'
TS.plot(time, overplot_variables, colour,label,y_label, title ,unit='h',Xray_class=pt)

'''
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
Plot SDO map
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
'''

location = 'S08E23'
start = '2017-09-02 00:00:00'
end = '2017-09-02 00:05:00'

Ob = observation.SDO(start, end, location)
Ob.get_map(graphs=['1600', '171','hmi'])
Ob.plot()
required_levels = [20, 100, 140, 300, 500]
Ob.plot(field_strength_spots = '1600', level=required_levels)