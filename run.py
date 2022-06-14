#!/usr/bin/env python3
# -*- coding: utf-8 -*-

'''
^^^^^^^^^^
* Inputs *
^^^^^^^^^^
'''
activeRegion = 956
filenumber = 3

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

bz = Map.read_data('bz', filenumber)
Map.plotmap(bz,title=r'$B_z$')

## Creat gif file
##---------------

im_st_No = 1
im_ed_No = 3
variable_name = 'bz'  

X=Map.plot_gif(im_st_No, im_ed_No, variable_name, title = 'bz')
'''
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
Create time series
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
'''

TS = win.timeseries(activeRegion, created_files_dir)

# examples of reading variables
dhbdt = TS.read_data( 'totHelBraid')
ddeltaL = TS.read_data('deltaLflux')
hb, time = TS.integrate_accum('totHelBraid')


# scale factor
factor = 1.0e5**4                # cm^4
hb = hb*factor
ddeltaL = ddeltaL*factor

mean, mstd = TS.mean_std(ddeltaL,n_points = 30,factor = 3)


overplot_variables = [ddeltaL,mean,mstd]
colour = ['c-','r-','b:']
label = ['$\dot{\delta L}$', '$\mu(\dot{\delta L})$', '$\mu+3\sigma$']

y_label = r'$\dot{\delta L}$'+' '+ '$(cm^4/s)$'
title = 'test'
X=TS.plot(time, overplot_variables, colour,label,y_label, 'test',unit='h')

'''
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
Plot NOAA 
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
'''
# search NOAA reports between these times
start_time = '2017-09-02 10:00:00'
end_time = '2017-09-03 20:00:00'

# choose some parameters for searching
variable = 'FLA'
activeregion = 'AR12673'
tr = 'Begin'

report = noaa(start_time,end_time)
showAll = report.show()

fltr_events = report.Filter(activeregion, variable)
XRs = report.toseconds(activeregion, 'XRA', tr)  
Fls = report.toseconds(activeregion, 'FLA', tr)            
combine = report.combine([XRs,Fls])    


overplot_variables = [ddeltaL,mean,mstd]
colour = ['c-','r-','b:']
label = ['$\dot{\delta L}$', '$\mu(\dot{\delta L})$', '$\mu+3\sigma$']

y_label = r'$winding$'+' '+ '$(km^4)$'
title = 'test'
TS.plot(time, overplot_variables, colour,label,y_label, title ,unit='h',oplot=combine)





'''
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
Plot X-ray flux
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

'''

start_time = '2017-09-02 10:00:00'
end_time = '2017-09-03 20:00:00'

X=observation.goes_xray(start_time, end_time,SatelliteNumber = 15)     # select the time limits and goes satellite number

picked_time, fluxA, fluxb = X.get_data()                        #  get the data

X.plot_xray(picked_time, fluxA, fluxb)                          # plot_xray( time , fluxA , fluxb )  
 
pt, pv = X.xray_peaks(fluxb,'C')                                # xray_peaks(  wavelength  , x-ray class  ) 



## over plot xray with the time series
##------------------------------------
y_label = r'$winding$'+' '+ '$(cm^4)$'
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

#Ob = observation.SDO(start_time, end_time, location)
#Ob.get_map(graphs=['1600', '171','hmi'])
#Ob.plot()
required_levels = [20, 100, 140, 300, 500]
#Ob.plot(field_strength_spots = '1600', level=required_levels)

