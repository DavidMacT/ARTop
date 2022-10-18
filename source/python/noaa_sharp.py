# -*- coding: utf-8 -*-


from __future__ import division, print_function
import os.path
import pandas as pd


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
    return sharp_number

            

x = getSHARP_number([12673, 11318])  # choose NOAA region numbers
print(x)
