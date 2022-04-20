# -*- coding: utf-8 -*-
"""
Created on Sat Mar 26 23:45:42 2022
@author: hp
"""
import matplotlib.pyplot as plt
from datetime import timedelta
from datetime import datetime

import numpy as np
import netCDF4 as nc
import os
import requests
from scipy.signal import find_peaks

class goes_xray:
    def __init__(self, start_time, end_time, SatelliteNumber):
        self.SatelliteNumber = SatelliteNumber

        year, month, day= int(start_time[:4]),int(start_time[5:7]),int(start_time[8:10])
        hour, minute = int(start_time[11:13]), int(start_time[14:16])
    
        year_e, month_e, day_e= int(end_time[:4]),int(end_time[5:7]),int(end_time[8:10])
        hour_e, minute_e = int(end_time[11:13]), int(end_time[14:16])
        
        self.stime = datetime(int(year),int(month),int(day),int(hour), int(minute),0)
        self.etime = datetime(int(year_e),int(month_e),int(day_e),int(hour_e), int(minute_e),0)
        
    
    def get_file(self,date_time_):
        date_time = str(date_time_)
    
        if ((self.SatelliteNumber == 13) or (self.SatelliteNumber == 14) or (self.SatelliteNumber == 15)):
            year, month, day= int(date_time[:4]),int(date_time[5:7]),int(date_time[8:10])
            
            call_path = 'https://satdat.ngdc.noaa.gov/sem/goes/data/science/xrs/goes'+str(self.SatelliteNumber)+'/gxrs-l2-irrad_science/'+str(year)+'/'+str("{0:02d}".format(month))+'/'
            
            filename = 'sci_gxrs-l2-irrad_g'+str(self.SatelliteNumber ) +'_d'+str(year)+str("{0:02d}".format(month))+str("{0:02d}".format(day))+'_v0-0-0.nc'
        else:
            year, month, day= int(date_time[:4]),int(date_time[5:7]),int(date_time[8:10])
       
            call_path = 'https://data.ngdc.noaa.gov/platforms/solar-space-observing-satellites/goes/goes'+str(self.SatelliteNumber )+'/l2/data/xrsf-l2-flx1s_science/'+str(year)+'/'+str("{0:02d}".format(month))+'/'
    
            filename = 'sci_xrsf-l2-flx1s_g'+str(self.SatelliteNumber) +'_d'+str(year)+str("{0:02d}".format(month))+str("{0:02d}".format(day))+'_v2-1-0.nc'
        return call_path, filename
    
    
    def pick_xray_time(self,Z_seconds, timeseries1, timeseries2):
        stime = self.stime
        etime = self.etime
        startday = datetime(int(str(stime)[:4]),int(str(stime)[5:7]),int(str(stime)[8:10]),0,0,0)
        
        picked_values1=[]
        picked_values2=[]
        picked_time=[]
        time_range = (etime-stime).total_seconds() 
        for s in range(len(Z_seconds)):
            time_acc = startday + timedelta(seconds=Z_seconds[s])
    
            if str(time_acc)[:16] == str(stime)[:16]:
                c=0
                while (time_acc < etime):
                    picked_values1.append(timeseries1[s])
                    picked_values2.append(timeseries2[s])
                    picked_time.append(Z_seconds[s])
                    
                    if (str(time_acc)[:16] == str(etime)[:16]):
                    
                        break
                 #   else:
                    s+=1
    
                    time_acc = startday + timedelta(seconds=Z_seconds[s])
    
                break
    
        return picked_time, picked_values1, picked_values2
    
    
    def time_label(self):
        stime = self.stime
        etime = self.etime
        time_labels=[]
        time=[]
        
        time_range = (etime-stime).total_seconds()
        steps = time_range // 6
        
        t_iter = stime
        for t in range(0, int(time_range), int(steps)):
    
            time_acc = stime + timedelta(seconds= t)
            if (str(time_acc)[:10] != str(t_iter)[:10]):
                time_labels.append(str(time_acc.strftime("%b %d %H:%M")))
                time.append(t)
                t_iter = str(time_acc)
                      
            else:
                time_labels.append(str(time_acc.strftime("%b %d %H:%M")))
                time.append(t)
        
        time_acc = stime + timedelta(seconds= time_range)              # ending time
        time_labels.append(str(time_acc.strftime("%b %d %H:%M")))
        time.append(int(time_range))
        
        return  time, time_labels
    
    
    def plot_xray(self,time, data1=None, data2=None):
        stime = self.stime
        etime = self.etime    
        level = {'A':1e-8,'B':1e-7,'C':1e-6, 'M':1e-5,'X':1e-4}
    
        plt.rcParams['font.size'] = '14'
        fig, ax = plt.subplots(figsize=(8,6))
        
        if data1 is not None:
            ax.plot(time,data1 ,'b', label='0.5 - 4.0'+r'$\ {\AA}$')
        if data2 is not None:
            ax.plot(time, data2,'r', label='1.0 - 8.0'+r'$\ {\AA}$')        
        ax.set_yscale("log")
        ax.grid(axis='y')
        ax.set_ylabel('Watts m'+r'$^{-2}$')
        ax.set_xlabel(r'$Year$ '+str(stime.strftime("%Y")))
    
        ax.set_ylim([1e-9, 1e-2]);   ax.set_xlim([time[0], time[-1]])
        #------------
            
        ax2 = ax.twinx()
        ax2.set_ylim([1e-9, 1e-2]);  ax2.set_yscale("log")
        plt.yticks( list(level.values()) ,  list(level.keys()))
    
        #--------------------------------
        time_plot, timelabel = self.time_label()
        time_ = [t+time[0] for t in time_plot]
        
        plt.xticks( time_ ,  timelabel )
        #-------------------------------
        ax.set_title('GOES '+ str(self.SatelliteNumber) + ' X-ray flux')

        ax.legend(loc='upper right')
        fig.autofmt_xdate()
        
    
    def get_data(self, path=None, save_files = False):
        stime = self.stime
        etime = self.etime
        bands=['a_flux', 'b_flux']
        variables ={'a_flux':[],'b_flux':[]}
    
        iter_time = stime
    
        start = datetime(int(str(stime)[:4]),int(str(stime)[5:7]),int(str(stime)[8:10]))
        end = datetime(int(str(etime)[:4]),int(str(etime)[5:7]),int(str(etime)[8:10])) + timedelta(days=1)
        
    
        c = 1 if ((end-start).total_seconds()%86400)>0 else 0
        files_number = (end-start).total_seconds()//86400 + c
    
       
        for f in range(int(files_number)):
            url, filename= self.get_file(stime)
            r = requests.get(url+filename, allow_redirects=True)
    
            if path is not None:
                open(path +filename, 'wb').write(r.content)
                ds = nc.Dataset(path+filename)
                if save_files is False:
                    os.remove(path+filename)
            else:
                open(filename, 'wb').write(r.content)
                ds = nc.Dataset(filename)
                if save_files is False:
                    os.remove(filename)
    
            if ((self.SatelliteNumber  == 13) or (self.SatelliteNumber  == 14) or (self.SatelliteNumber  == 15)):
                variables['a_flux']+= list(ds.variables['a_flux'][:] )
                variables['b_flux']+= list(ds.variables['b_flux'][:] )
            else:
                variables['a_flux']+= list(ds.variables['xrsa_flux'][:] )
                variables['b_flux']+= list(ds.variables['xrsb_flux'][:] )
        
        Total_time = np.linspace(0,int(24*3600*files_number),len(variables['a_flux']))
       
        self.picked_time, fluxA, fluxb = self.pick_xray_time(Total_time , variables['a_flux'], variables['b_flux'])
    
        
        return  self.picked_time, fluxA, fluxb 
    
    
    
    def xray_peaks(self, data, xray_class):
        
        
        level = {'C':[1e-6, 'g--'], 'M':[1e-5, 'b--'],'X':[1e-4, 'r--'] }
        if xray_class == list(level.keys())[0]:
            prominence, color = level['C'][0], level['C'][1]
        elif xray_class == list(level.keys())[1]:
            prominence, color = level['M'][0], level['M'][1]
        else:
            prominence, color = level['X'][0], level['X'][1]
            
        peaks_indces, _ = find_peaks(data, prominence=prominence)
        
        peaks=[data[i] for i in peaks_indces]
        peak_times =[self.picked_time[i] for i in peaks_indces]
        
        return [peak_times, xray_class,color] , peaks
    
