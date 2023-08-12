#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Aug 12 10:05:45 2023
@author: khd2
"""
import json
import codecs
import urllib

import astropy.table
from sunpy import log
from sunpy.net import attr
from sunpy.net.hek import attrs
from sunpy.util import dict_keys_same
from datetime import datetime, timedelta
from sunpy.net import attrs as a
import pandas as pd

class flares_time():
    def __init__(self, time_interval, event = a.hek.FL, Frame = "SWPC"):
        self.timerange = time_interval
        timerange = a.Time(self.timerange[0], self.timerange[1])

        self.data_discription = self.search(timerange, event, a.hek.FRM.Name == Frame)      
        
        self.data = pd.DataFrame( self.sortdata(self.data_discription ) )
        
        
    def _download(self, data):
        """ 
        This function is quoted from HEK client and has been adjusted
        Download all data, even if paginated. 
        source: https://docs.sunpy.org/en/stable/generated/api/sunpy.net.hek.HEKClient.html
        """

        DEFAULT_URL = 'https://www.lmsal.com/hek/her?'
      
        page = 1
        results = []
        new_data = data.copy()
        # Override the default name of the operatorX, where X is a number.
        for key in data.keys():
            if "operator" in key:
                new_data[f"op{key.split('operator')[-1]}"] = new_data.pop(key)
        while True:
            new_data['page'] = page
            url = DEFAULT_URL + urllib.parse.urlencode(new_data)
            log.debug(f'Opening {url}')
            fd = urllib.request.urlopen(url)

            try:
                result = codecs.decode(fd.read(), encoding='utf-8', errors='replace')
                result = json.loads(result)
            except Exception as e:
                raise OSError("Failed to load return from the HEKClient.") from e
            finally:
                fd.close()
            results.extend(result['result'])

            if not result['overmax']:
                if len(results) > 0:
                    table = dict_keys_same(results)
    
                    return table
                else:
                    return astropy.table.Table()
            page += 1


    def search(self,*args, **kwargs):
        """
        The function is quoted from HEK client and has been adjusted
        source: https://docs.sunpy.org/en/stable/generated/api/sunpy.net.hek.HEKClient.html
        """
        default = {
            'cosec': '2',  # Return .json
            'cmd': 'search',
            'type': 'column',
            'event_type': '**',
            }
        # Default to full disk.
        attrs.walker.apply(attrs.SpatialRegion(), {}, default)

        query = attr.and_(*args)
        data = attrs.walker.create(query, {})
        ndata = []
        for elem in data:
            new = default.copy()
            new.update(elem)
            ndata.append(new)
        if len(ndata) == 1:
            return self._download(ndata[0])
        else:
            return HEKTable(_merge(_download(data) for data in ndata))    

    def sortdata(self,data):
        dataset=[]
        for event in data:
            dataset.append( self.filterdata(event) )

        return dataset
           
    def datetimeform(self,dt):
        return datetime(int(dt[:4]), int(dt[5:7]), int(dt[8:10]), int(dt[11:13]), int(dt[14:16]) )
    
    def filterdata(self,data):
        information = ['ar_noaanum','SOL_standard','event_starttime', 'event_endtime','search_instrument','obs_channelid','fl_goescls','event_peaktime']
        table = dict()
        for key in information:
            try:
                table[key] = data[key]
            except Exception as e:    
                raise OSError(f"{key} is not found in the data") from e
                
        return table
 
    
    def XR_label(self,eventtype):
        if eventtype[0] == "A": label = 'c--'
        if eventtype[0] == "B": label = 'y--'
        if eventtype[0] == "C": label = 'g--'
        if eventtype[0] == "M": label = 'b--'
        if eventtype[0] == "X": label = 'r--'

        return label            

    
    def Timeframe(self,event_info):
        interval_sdt = self.datetimeform( self.timerange[0] )
        
        event_dt = self.datetimeform(event_info)
        
        return (event_dt-interval_sdt).total_seconds()

    def labels(self,required_time = 'event_peaktime', AR = None):
        labels = {f"{required_time}(seconds)":[], "color":[], "type":[] }
        for i, event in enumerate(self.data['fl_goescls']):
            if AR is not None:
                if AR == self.data['ar_noaanum'][i]:
                    labels[f"{required_time}(seconds)"].append(self.Timeframe(self.data[required_time][i])    )
                    labels["color"].append(self.XR_label(event))
                    labels["type"].append(event)
                else:
                    continue
            else:
                labels[f"{required_time}(seconds)"].append(self.Timeframe(self.data[required_time][i])    )
                labels["color"].append(self.XR_label(event))
                labels["type"].append(event)               

        return pd.DataFrame(labels)
        


""" Get the flar data """
   
time_interval = ['2017-09-02 10:00:00', '2017-09-03 20:00:00']

flares = flares_time(time_interval, event = a.hek.FL, Frame = "SWPC")

## Detail disscrption of the data
flares.data_discription

## get data of the flares
flares.data

## get labels of the flares to plot for all the active regions
flares.labels(required_time = 'event_peaktime') # The time could be event_peaktime or event_starttime or event_endtime

## To specify the data of an Active region
flares.labels(required_time = 'event_peaktime', AR = 12674) # put the number of the active region from the ar_noaanum column without quotations.
