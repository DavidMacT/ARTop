#!/usr/bin/env python3
# -*- coding: utf-8 -*-


from __future__ import division, print_function
import os.path
import pandas as pd
import requests
import os
import webbrowser


def NOAA_HARP_number(year, month, day,hour):
    Date = "{:02d}".format(year)+'/'+"{:02d}".format(month)+'/'+"{:02d}".format(day)
    DateTime = "{:02d}".format(year)+'.'+"{:02d}".format(month)+'.'+"{:02d}".format(day)+'_'+ "{:02d}".format(hour)        
    image = "http://jsoc.stanford.edu/doc/data/hmi/harp/harp_definitive/"+Date+"/harp."+DateTime+":00:00_TAI.png"
    response = requests.get(image)
    
    if response.status_code == 200:
        webbrowser.open(image)
    else:
        image = "http://jsoc.stanford.edu/doc/data/hmi/harp/harp_nrt/harp."+DateTime+":00:00_TAI.png"
        webbrowser.open(image)


def getSHARP_number(NOAA_ARegion):

    url='http://jsoc.stanford.edu/doc/data/hmi/harpnum_to_noaa/all_harps_with_noaa_ars.txt'
    SHARP_list = pd.read_fwf(url,header=None)[0]
    data={'SHARP':[], 'NOAA_ARS':[]}
    for n in range(1,len(SHARP_list)):
        AR = SHARP_list[n].split(' ')
        data['SHARP'].append(AR[0])
        data['NOAA_ARS'].append(AR[1].split(','))

    sharp_number=[]
    for R in NOAA_ARegion:
        for i,S in enumerate(data['NOAA_ARS']):
            if str(R) in S:
                sharp_number.append(data['SHARP'][i])
    if len(sharp_number) == 0:
        print('More details are needed:')
        year = input('insert the year: ')
        month = input('insert the month: ')
        day = input('insert the day: ')
        hour = input('insert the hour: ')
        NOAA_HARP_number(int(year),int(month), int(day),int(hour))
    else:
        print(sharp_number)
        return sharp_number
          
"""
Get the SHARP number
"""
Noaa = int(input('insert NOAA number: '))
getSHARP_number([Noaa])  # Insert NOAA region numbers
