#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed May 30 11:01:12 2022
@author: khaled Darwish
"""
'''
^^^^^^^^^^
* Inputs *
^^^^^^^^^^
'''
activeRegion = 2011
filenumber =  2

##=========================================================================================
import os
import sys
sys.path.insert(1, os.getcwd() + '/source/python/')
import winding as win
import observation
from observation import NOAAreport as noaa

created_files_dir= win.getpaths(os.getcwd(), activeRegion)
'''
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
Create a map
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
'''

Map =win.mapping(activeRegion,created_files_dir)

bz = Map.read_data('Sz', filenumber)
Map.plotmap(bz,title=r'$S_z$')

## Creat gif file
##---------------

im_st_No = 20
im_ed_No = 26
variable_name = 'bz'  

#X=Map.plot_gif(im_st_No, im_ed_No, variable_name, title = 'bz')
'''
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
Create time series
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
'''
start_time = '2017-09-02 10:00:00'
end_time = '2017-09-03 20:00:00'

TS = win.timeseries(activeRegion, created_files_dir)

L = TS.read_data( 'totWind_Cur_Pot_Vel')
deltaL = TS.read_data('deltaLflux')

intL, time = TS.integrate_accum(start_time, end_time,'totWind_Cur_Pot_Vel')


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
start_time = '2017-09-02 10:00:00'
end_time = '2017-09-03 20:00:00'

X=observation3.goes_xray(start_time, end_time,SatelliteNumber = 15)     # select the time limits and goes satellite number

picked_time, fluxA, fluxb = X.get_data()                        #  get the data

X.plot_xray(picked_time, fluxA, fluxb)                          # plot_xray( time , fluxA , fluxb )  
 
pt, pv = X.xray_peaks(fluxb,'C')                                # xray_peaks(  wavelength  , x-ray class  ) 



## over plot xray with the time series
##------------------------------------
y_label = r'$winding$'+' '+ '$(km^4)$'
title = 'test'
TS.plot(time, overplot_variables, colour,label,y_label, title ,unit='h',oplot=pt)

'''
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
Plot SDO map
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
'''

location = 'S08E23'
start_time = '2011-10-13 20:00:00'
end_time = '2011-10-13 20:05:00'

#Ob = observation3.SDO(start_time, end_time, location)
#Ob.get_map(graphs=['1600', '171','hmi'])
#Ob.plot()
required_levels = [20, 100, 140, 300, 500]
#Ob.plot(field_strength_spots = '1600', level=required_levels)

'''
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
Plot NOAA 
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
'''
start_time = '2017-09-02 10:00:00'
end_time = '2017-09-03 20:00:00'

variable = 'FLA'
activeregion = 'AR12674'
tr = 'Begin'

report = noaa(start_time,end_time)
showAll = report.show()

fltr_events = report.Filter(activeregion, variable)
XRs = report.toseconds(activeregion, 'XRA', tr)  
Fls = report.toseconds(activeregion, 'FLA', tr)            
combine = report.combine([XRs,Fls])    


overplot_variables = [intL,mean,mstd]
colour = ['c-','r-','b:']
label = ['$\dot{\delta L}$', '$\mu(\dot{\delta L})$', '$\mu+3\sigma$']

y_label = r'$winding$'+' '+ '$(km^4)$'
title = 'test'
TS.plot(time, overplot_variables, colour,label,y_label, title ,unit='h',oplot=combine)

