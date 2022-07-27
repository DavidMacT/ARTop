#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
from datetime import timedelta
import datetime
import pandas as pd
import numpy as np
import netCDF4 as nc
import os
import requests
from scipy.signal import find_peaks
import requests

import sunpy.map
from sunpy.net import Fido, attrs as a
from astropy.coordinates import SkyCoord
from astropy import units as u
import sys

import warnings
warnings.filterwarnings("ignore")



class goes_xray:
    def __init__(self, start_time, end_time, SatelliteNumber):
        self.SatelliteNumber = SatelliteNumber

        year, month, day= int(start_time[:4]),int(start_time[5:7]),int(start_time[8:10])
        self.hour_s, self.minute_s = int(start_time[11:13]), int(start_time[14:16])
    
        year_e, month_e, day_e= int(end_time[:4]),int(end_time[5:7]),int(end_time[8:10])
        hour_e, minute_e = int(end_time[11:13]), int(end_time[14:16])
        
        self.stime = datetime.datetime(int(year),int(month),int(day),int(self.hour_s), int(self.minute_s),0)
        self.etime = datetime.datetime(int(year_e),int(month_e),int(day_e),int(hour_e), int(minute_e),0)
        
    
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
        startday = datetime.datetime(int(str(stime)[:4]),int(str(stime)[5:7]),int(str(stime)[8:10]),0,0,0)
        
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
    
    def clear(self,data):
        cl_data=[]
        for i in data:
            if (i<=0):
                cl_data.append(None)
            else:
                cl_data.append(i)
        return cl_data    
    
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
    
        plt.rcParams['font.size'] = '20'
        fig, ax = plt.subplots(figsize=(8,6))
        
        if data1 is not None:
            ax.plot(time,self.clear(data1) ,'b', label='0.5 - 4.0'+r'$\ {\AA}$')
        if data2 is not None:
            ax.plot(time, self.clear(data2),'r', label='1.0 - 8.0'+r'$\ {\AA}$')        
            
        ax.plot(((12120),(12120)), (min(data2+data1),max(data2+data1)+100),'k--')              ## new
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
    
        start = datetime.datetime(int(str(stime)[:4]),int(str(stime)[5:7]),int(str(stime)[8:10]))
        end = datetime.datetime(int(str(etime)[:4]),int(str(etime)[5:7]),int(str(etime)[8:10])) + timedelta(days=1)
        
    
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
    
    def get_relativetime(self,time):
        return time - ((self.hour_s*3600)+ self.minute_s*60)   
    
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
        
        
        return [[self.get_relativetime(t),color, xray_class] for t in peak_times], peaks


class SDO:
    def __init__(self, starttime, endtime, Location):
        self.str_time = starttime
        self.end_time = endtime
        self.location = Location
    def check_file(self, file_name):
        if file_name[0][-3:] == '.gz':
            file = file_name[1]
        else:
            file = file_name[0]
        return file
        
    def AR_location(self,location):
        N=1
        E=1
        r=1
        i=1
        if location[0] == 'S':
            N = -1
            i = -1
        if location[3] == 'W':
            E = -1
            r = -1
        NS, EW = str(location)[1:3], str(location)[4:]
    
        East_West = SkyCoord( (E*int(EW) + (r*5)) * u.deg, (N*int(NS)+(i*1)) * u.deg, frame='heliographic_stonyhurst')
        boundaries = SkyCoord( (E* int(EW) + (r*22)) * u.deg, (i*4) * u.deg, frame='heliographic_stonyhurst')
        boundaries2 = SkyCoord( (E* int(EW) + (r*20)) * u.deg, (i*2) * u.deg, frame='heliographic_stonyhurst')
    
        return East_West, boundaries, boundaries2


    def get_map(self, graphs=None):
 
        if (graphs is None):
            sys.exit('Wavelengths are missing ... ')

        self.data={}           
        for Wl in graphs:
            if Wl == "hmi":
                result = Fido.search(a.Time(self.str_time,  self.end_time),
                                     a.Instrument.hmi, a.Physobs.los_magnetic_field)
                self.data['hmi']= self.check_file(Fido.fetch(result)) 
            else:
                result_ = Fido.search(a.Time(self.str_time,  self.end_time),
                                      a.Instrument("aia"),  a.Wavelength(int(Wl)*u.angstrom))
                self.data[Wl]= Fido.fetch(result_[0, 3], site='ROB')
        return self.data

    
    def peek(self, spectrum):
        spc = sunpy.map.Map(self.data[spectrum])
        spc.plot()

    def plot(self, field_strength_spots=None, level=None):
        
        bottom_left, top_right, top_right2 = self.AR_location(self.location)
        
        fig = plt.figure(figsize=(11, 5))
        i=1
        for p in self.data.keys():
            X = sunpy.map.Map(self.data[p]).submap(bottom_left, top_right=top_right)             

            ax2 = plt.subplot(1, len(self.data.keys()), i, projection=X)
            
            X.plot(clip_interval=(1, 99.99)*u.percent)
            plt.colorbar(shrink=0.5, pad=0.02)
            i+=1

        plt.show()
         
        if field_strength_spots is not None:
            if field_strength_spots in self.data.keys():
                fig = plt.figure(figsize=(7, 5))
                             
                sub_map = sunpy.map.Map(self.data[field_strength_spots]).submap(bottom_left, top_right=top_right2)
                          
                ax = plt.subplot(projection=sub_map)
                sub_map.plot(clip_interval=(1, 99.99)*u.percent)
                grid = sub_map.draw_grid()
                
                ax.set_title( "AIA " +field_strength_spots+" with magnetic field strength values", y=1.2)
                if field_strength_spots == 'hmi':
    
                    ax.set_title( 'HMI' +" with magnetic field strength values", y=1.2)
                

                levels = level  * u.Gauss    
                levels = np.concatenate((-1 * levels[::-1], levels))
                
                hmi = self.get_map(graphs=['hmi'])
                sub_hmi = sunpy.map.Map(hmi['hmi']).submap(bottom_left, top_right=top_right2)
                cset = sub_hmi.draw_contours(levels, axes=ax, cmap='nipy_spectral', alpha=1)
                
                plt.colorbar(cset,
                             label=f"Magnetic Field Strength [{sub_map.unit}]",
                             ticks=list(levels.value) + [0], 
                             shrink=0.7, pad=0.12)
                plt.show()
            else:
                sys.exit('datumn of '+ field_strength_spots +' is missing ... ')

class NOAAreport:
    def __init__(self,start, end):
        self.stime = (int(start[11:13])*3600) + ( int(start[14:16])*60)
        c=0
        daframe={'Event':[], 'Begin':[],'Max':[], 'End':[],
                 'Type':[],'Loc/Frq':[], 'Particulars':[], 'Reg':[]}

        start = datetime.date(int(start[:4]),int(start[5:7]),int(start[8:10]))
        end = datetime.date(int(end[:4]),int(end[5:7]),int(end[8:10]))
        while (start <= end):
            
            start = str(start)
            urlpage = 'https://www.solarmonitor.org/data/'+start[:4]+'/'+start[5:7]+'/'+ start[8:10]+'/meta/noaa_events_raw_'+start[:4]+start[5:7]+start[8:10]+'.txt' 
            response = requests.get(urlpage)
            if response.status_code == 200:
                df = pd.read_fwf(urlpage,header=None)
                if 'NO EVENT REPORTS.' not in list(df[0]):
                    print(start)            
                    for i in range(12, len(df[0])):
                        if ((df[0][i][58]=='V') or (df[0][i][58]=='I')):
                            continue
                        else:
                            daframe['Event'].append(df[0][i][:4])
                        if not (df[0][i][11:15].isnumeric()):
                            daframe['Begin'].append( str(np.nan))
                        else:
                            daframe['Begin'].append(   str(int(df[0][i][11:13])+c) + df[0][i][13:15])
                        if not (df[0][i][18:22].isnumeric()):
                            daframe['Max'].append( str(np.nan))
                        else:
                            daframe['Max'].append(  str(int(df[0][i][18:20])+c) +df[0][i][20:22])
                        if not (df[0][i][28:32].isnumeric()):
                            daframe['End'].append( str(np.nan))
                        else:
                            daframe['End'].append( str(int(df[0][i][28:30])+c) + df[0][i][30:31])
                        daframe['Type'].append(df[0][i][43:46])
                        daframe['Loc/Frq'].append(df[0][i][48:55])
                        daframe['Particulars'].append(df[0][i][58:62])
                        daframe['Reg'].append(df[0][i][76:80])
            else:
                print('Report missing')
            start = datetime.date(int(start[:4]),int(start[5:7]),int(start[8:10]))+ datetime.timedelta(days=1)
            c+=24
            self.df = pd.DataFrame(daframe)
    
    def unique(self,list_):
        set_val=[]
        for v in list_:
            if v not in set_val:
                if len(str(v)) !=0:
                    set_val.append(v)
        return set_val
    
    def show(self, variable=None):
        if variable is not None:
            return self.unique(self.df[variable])
        else:
            return self.df
    def Filter(self,activeregion, variable):
        region = str(activeregion)[-4:]
        index = []
        for i in range(len(self.df['Type'])):
            if ((self.df['Reg'][i] == region) and (self.df['Type'][i]==variable)):
                index.append(i)

        if len(index) == 0:
            print('The region is not found, try another date')
            return index
        else:
            return self.df.iloc[index]

    def get_relativetime(self,time):
        return time - self.stime

    def toseconds(self, activeregion, variable, time):
        if str(activeregion)[-4:] in self.show('Reg'):
            if len(self.Filter(activeregion, variable)) != 0: 
                time_secd = [((int(i[:-2])*3600)+(int(i[-2:])*60)) for i in self.Filter(activeregion, variable)[time]] 
                l=[]
                if ((variable == 'XRA') or (variable == 'FLA')):
                    Xrayclass = [i for i in self.Filter(activeregion, variable)['Particulars']]
                    for c in range(len(Xrayclass)):
                        if (time_secd[c] >= self.stime):
                            if Xrayclass[c][0] == 'A':
                                l.append([self.get_relativetime(time_secd[c]),'c--', Xrayclass[c]])
                            if Xrayclass[c][0] == 'B':
                                l.append([self.get_relativetime(time_secd[c]),'y--', Xrayclass[c]])
                            if Xrayclass[c][0] == 'C':
                                l.append([self.get_relativetime(time_secd[c]),'g--', Xrayclass[c]])
                            if Xrayclass[c][0] == 'M':
                                l.append([self.get_relativetime(time_secd[c]),'b--', Xrayclass[c]])
                            if Xrayclass[c][0] == 'X':
                                l.append([self.get_relativetime(time_secd[c]),'r--', Xrayclass[c]])
                if variable == 'FLA':
                    flare = [i for i in self.Filter(activeregion, variable)['Particulars']]
                    for c in range(len(flare)):
                        if (time_secd[c] >= self.stime):
                            l.append([self.get_relativetime(time_secd[c]),'k-', 'FL'])
                return l      
            else:
                print('No data for '+variable + ' for this region')
                return None 
        else:
            print('The region number '+str(activeregion)[-4:]+' is not found. Try another day.')

   

    def combine(self,list_of_lists):
        List=[]
        for l in list_of_lists:
            if l is not None:
                List += l

        if len(List)>1:
            l=[]
            for v in List:
                l.append(v[0])
            sortd_t = sorted(l)
            sorted_list=[]
            for s in sortd_t:
                for t in range(len(List)):
                    if s == List[t][0]:
                        sorted_list.append(List[t])
            return sorted_list
        else:
            sys.exit('There is no data')
            
     

