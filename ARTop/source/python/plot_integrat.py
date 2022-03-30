#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Mar  8 09:59:55 2022

@author: khd2
"""
# import pandas as pd
# from astropy import units as u
# from sunpy import timeseries as ts
# from sunpy.time import parse_time
# from collections import OrderedDict

# noaa_file = "https://services.swpc.noaa.gov/json/goes/primary/xrays-7-day.json"
# units = OrderedDict([('xrsa', u.W/u.m**2),('xrsb', u.W/u.m**2)])

# data = pd.read_json(noaa_file)
# data_short = data[data['energy']=='0.05-0.4nm']
# data_long = data[data['energy']=='0.1-0.8nm']
# time_array = [parse_time(x).datetime for x in data_short['time_tag'].values]

# df = pd.DataFrame({'xrsa': data_short['flux'].values, 'xrsb': data_long['flux'].values}, index=time_array)
# df.sort_index(inplace=True)
   
# goes_data = ts.TimeSeries(df, units, source="xrs")
# goes_data.peek()

# goes_flare = goes_data.truncate("2015-06-21 09:35", "2015-06-21 10:30")
# goes_flare.index

# from sunpy.net import Fido, attrs as a
# #from sunpy.net.attrs import Time, Instrument

# tstart = "2011-06-21 01:00"
# tend = "2011-06-21 23:00"
# result_goes15 = Fido.search(a.Time(tstart, tend), a.Instrument("XRS"), a.goes.SatelliteNumber(15))
# file_goes15 = Fido.fetch(result_goes15)  
# #print(filepaths[0])

# goes_15 = ts.TimeSeries(file_goes15)

from astropy.io import fits
#fits_image_filename = fits.util.get_testdata_filepath('/home/khd2/Downloads/go1520110607.fits')

#with fits.open('/home/khd2/Downloads/go1520110607.fits') as hdul:
#    hdul.verify('fix')
#    data = hdul[1].data

hdul = fits.open('/home/khd2/Downloads/go1520110607.fits')


import matplotlib.pyplot as plt
import numpy as np

from sunpy import timeseries as ts
from sunpy.net import Fido
from sunpy.net import attrs as a

tstart = "2015-06-21 01:00"
tend = "2015-06-21 23:00"
result = Fido.search(a.Time(tstart, tend), a.Instrument("XRS"))
print(result)
result_goes15 = Fido.search(a.Time(tstart, tend), a.Instrument("XRS"), a.goes.SatelliteNumber(15))
print(result_goes15)

file_goes15 = Fido.fetch(result_goes15)


goes_15 = ts.TimeSeries(file_goes15)
goes_15.peek()
# with open(filepaths[0], encoding="utf8", errors='ignore') as f:
#     data = f.read()
# data_short = data[data['energy']=='0.05-0.4nm']
# data_long = data[data['energy']=='0.1-0.8nm']
# time_array = [parse_time(x).datetime for x in data_short['time_tag'].values]

# import matplotlib.pyplot as plt
# import numpy as np

# from sunpy.data.sample import GOES_XRS_TIMESERIES
# from sunpy.timeseries import TimeSeries

# goes_lc = TimeSeries(GOES_XRS_TIMESERIES)
# my_timeseries = goes_lc.truncate('2011/06/21 00:00', '2011/06/21 23:00')
# fig, ax = plt.subplots()
# my_timeseries.plot()