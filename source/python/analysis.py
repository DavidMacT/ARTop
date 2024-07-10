#!/usr/bin/env python
# coding: utf-8

import os
import os.path
import numpy as np
import matplotlib.pyplot as plt
import sunpy.map
from astropy.coordinates import SkyCoord
import astropy.units as u
from sunpy.coordinates import frames
from sunpy.map.header_helper import make_fitswcs_header
import pandas as pd
import requests
import datetime


def readHead(outputDir):
    '''
    Read header.txt file in the output directory.


    Input:
    ------
    outputDir:  directory containing specifications.txt

    Output:
    -------
    variables  :  array with all the parameters stored in header.txt

    '''

    with open(outputDir,'r') as f:
        contents = f.readlines()

    variables = []
    n = 10
    for i in range(n):
        variables.append(contents[i].strip())
    return variables



def readSpec(outputDir):
    '''
    Read specifications.txt file in the output directory.


    Input:
    ------
    outputDir:  directory containing specifications.txt

    Output:
    -------
    details  :  array with all the parameters stored in specifications.txt

    '''

    with open(outputDir,'r') as f:
        contents = f.readlines()

    variables = []
    n = 7
    for i in range(n):
        variables.append(contents[i].strip())
    return variables


def readSeries(outputDir):
    '''
    Read net file and output dataframe with each variable in a different column
    '''

    specPath = os.path.join(outputDir,'specifications.txt')
    specVar =  readSpec(specPath)     # read specifications.txt
    nt      = int(specVar[3])         # number of time steps
    VS      = specVar[4]              # velocity smoothing parameter
    CO      = specVar[5]              # mag. field cut-off
    d       = specVar[6]              # downsampling parameter

    netName = 'netWindDatPotFast'+CO+'_VS'+VS+'_'+d+'.dat'
    netPath = os.path.join(outputDir,'Data',netName)

    f = open(netPath,'r')
    lines = f.readlines()
    lines2 = []
    for i in lines:
        lines2.append(i.strip())
    list = np.array(lines2)
    
    # create variable arrays
    windCur   = np.zeros(nt)
    helCur    = np.zeros(nt)
    windPot   = np.zeros(nt)
    helPot    = np.zeros(nt)
    windBraid = np.zeros(nt)
    helBraid  = np.zeros(nt)
    wind      = np.zeros(nt)
    hel       = np.zeros(nt)
    dL        = np.zeros(nt)
    dH        = np.zeros(nt)

    n = 10
    for i in range(nt):
        windCur[i]   = list[n*i]
        helCur[i]    = list[n*i+1]
        windPot[i]   = list[n*i+2]
        helPot[i]    = list[n*i+3]
        windBraid[i] = list[n*i+4]
        helBraid[i]  = list[n*i+5]
        wind[i]      = list[n*i+6]
        hel[i]       = list[n*i+7]
        dL[i]        = list[n*i+8]
        dH[i]        = list[n*i+9]

    data = {'windCur'   : windCur,
            'helCur'    : helCur,
            'windPot'   : windPot,
            'helPot'    : helPot,
            'windBraid' : windBraid,
            'helBraid'  : helBraid,
            'wind'      : wind,
            'hel'       : hel,
            'dL'        : dL,
            'dH'        : dH}

    del windCur, windPot, windBraid, helCur, helPot, helBraid, wind, hel, dL, dH

#    times = np.zeros(nt)
#    for i in range(nt):
#        times[i] = 0.2*i
    times = np.arange(0,nt*720,720)
    
    
    df = pd.DataFrame(data)
    df.insert(0, 'times(seconds)', times)
    del times
    
    return df
            
def running_mean_std(df,offset,nsigma):
    '''
    create dataframe with running mean

    offset = num of times
    '''
    times     = df['times(seconds)'].to_numpy()
    windCur   = df['windCur'].to_numpy()
    helCur    = df['helCur'].to_numpy()
    windPot   = df['windPot'].to_numpy()
    helPot    = df['helPot'].to_numpy()
    windBraid = df['windBraid'].to_numpy()
    helBraid  = df['helBraid'].to_numpy()
    wind      = df['wind'].to_numpy()
    hel       = df['hel'].to_numpy()
    dL        = df['dL'].to_numpy()
    dH        = df['dH'].to_numpy()

    nt = len(df['dH'])
     
    # create variable arrays for running means and std
    windCur_ms   = np.zeros(nt)
    windPot_ms   = np.zeros(nt)
    windBraid_ms = np.zeros(nt)
    helCur_ms    = np.zeros(nt)
    helPot_ms    = np.zeros(nt)
    helBraid_ms  = np.zeros(nt)
    wind_ms      = np.zeros(nt)
    hel_ms       = np.zeros(nt)
    dL_ms        = np.zeros(nt)
    dH_ms        = np.zeros(nt)

    windCur_std   = np.zeros(nt)
    windPot_std   = np.zeros(nt)
    windBraid_std = np.zeros(nt)
    helCur_std    = np.zeros(nt)
    helPot_std    = np.zeros(nt)
    helBraid_std  = np.zeros(nt)
    wind_std      = np.zeros(nt)
    hel_std       = np.zeros(nt)
    dL_std        = np.zeros(nt)
    dH_std        = np.zeros(nt)


    for i in range(nt):
        if(i < offset):
            pass
        else:
            windCur_ms[i]    = np.mean(windCur[i-offset:i-1])
            windCur_std[i]   = np.std(windCur[i-offset:i-1])
            windCur_ms[i]    = windCur_ms[i] + nsigma*windCur_std[i]
            
            windPot_ms[i]    = np.mean(windPot[i-offset:i-1])
            windPot_std[i]   = np.std(windPot[i-offset:i-1])
            windPot_ms[i]    = windPot_ms[i] + nsigma*windPot_std[i]
            
            windBraid_ms[i]  = np.mean(windBraid[i-offset:i-1])
            windBraid_std[i] = np.std(windBraid[i-offset:i-1])
            windBraid_ms[i]  = windBraid_ms[i] + nsigma*windBraid_std[i]
            
            helCur_ms[i]     = np.mean(helCur[i-offset:i-1])
            helCur_std[i]    = np.std(helCur[i-offset:i-1])
            helCur_ms[i]     = helCur_ms[i] + nsigma*helCur_std[i]
            
            helPot_ms[i]     = np.mean(helPot[i-offset:i-1])
            helPot_std[i]    = np.std(helPot[i-offset:i-1])
            helPot_ms[i]     = helPot_ms[i] + nsigma*helPot_std[i] 
            
            helBraid_ms[i]   = np.mean(helBraid[i-offset:i-1])
            helBraid_std[i]  = np.std(helBraid[i-offset:i-1])
            helBraid_ms[i]   = helBraid_ms[i] + nsigma*helBraid_std[i]
            
            wind_ms[i]       = np.mean(wind[i-offset:i-1])
            wind_std[i]      = np.std(wind[i-offset:i-1])
            wind_ms[i]       = wind_ms[i] + nsigma*wind_std[i]
            
            hel_ms[i]        = np.mean(hel[i-offset:i-1])
            hel_std[i]       = np.std(hel[i-offset:i-1])
            hel_ms[i]        = hel_ms[i] + nsigma*hel_std[i]
            
            dL_ms[i]         = np.mean(dL[i-offset:i-1])
            dL_std[i]        = np.std(dL[i-offset:i-1])
            dL_ms[i]         = dL_ms[i] + nsigma*dL_std[i]
            
            dH_ms[i]         = np.mean(dH[i-offset:i-1])
            dH_std[i]        = np.std(dH[i-offset:i-1])
            dH_ms[i]         = dH_ms[i] + nsigma*dH_std[i]

    data = {'times(seconds)': times,
            'windCur_ms'    : windCur_ms,
            'helCur_ms'     : helCur_ms,
            'windPot_ms'    : windPot_ms,
            'helPot_ms'     : helPot_ms,
            'windBraid_ms'  : windBraid_ms,
            'helBraid_ms'   : helBraid_ms,
            'wind_ms'       : wind_ms,
            'hel_ms'        : hel_ms,
            'dL_ms'         : dL_ms,
            'dH_ms'         : dH_ms}

    del times, windCur, windPot, windBraid, helCur, helPot, helBraid, wind, hel, dL, dH
    del windCur_ms, windPot_ms, windBraid_ms, helCur_ms, helPot_ms, helBraid_ms, wind_ms, hel_ms, dL_ms, dH_ms
    del windCur_std, windPot_std, windBraid_std, helCur_std, helPot_std, helBraid_std, wind_std, hel_std, dL_std, dH_std

    
    ndf = pd.DataFrame(data)
    return ndf
            

            

def integrate_series(df):
    '''
    create dataframe with time integrated versions of the variables
    integrated in seconds
    '''
    dt = 720
    nt = len(df)

    times     = df['times(seconds)'].to_numpy()
    windCur   = df['windCur'].to_numpy()
    windPot   = df['windPot'].to_numpy()
    windBraid = df['windBraid'].to_numpy()
    helCur    = df['helCur'].to_numpy()
    helPot    = df['helPot'].to_numpy()
    helBraid  = df['helBraid'].to_numpy()
    wind      = df['wind'].to_numpy()
    hel       = df['hel'].to_numpy()
    dL        = df['dL'].to_numpy()
    dH        = df['dH'].to_numpy()

    windCur_int   = np.zeros(nt)
    windPot_int   = np.zeros(nt)
    windBraid_int = np.zeros(nt)
    helCur_int    = np.zeros(nt)
    helPot_int    = np.zeros(nt)
    helBraid_int  = np.zeros(nt)
    wind_int      = np.zeros(nt)
    hel_int       = np.zeros(nt)
    dL_int        = np.zeros(nt)
    dH_int        = np.zeros(nt)

    for i in range(nt):
        windCur_int[i]   = np.trapz(windCur[0:i+1], dx=dt)
        windPot_int[i]   = np.trapz(windPot[0:i+1], dx=dt)
        windBraid_int[i] = np.trapz(windBraid[0:i+1], dx=dt)
        helCur_int[i]    = np.trapz(helCur[0:i+1], dx=dt)
        helPot_int[i]    = np.trapz(helPot[0:i+1], dx=dt)
        helBraid_int[i]  = np.trapz(helBraid[0:i+1], dx=dt)
        wind_int[i]      = np.trapz(wind[0:i+1], dx=dt)
        hel_int[i]       = np.trapz(hel[0:i+1], dx=dt)
        dL_int[i]        = np.trapz(dL[0:i+1], dx=dt)
        dH_int[i]        = np.trapz(dH[0:i+1], dx=dt)

    data = {'times(seconds)': times,
            'windCur_int'   : windCur_int,
            'helCur_int'    : helCur_int,
            'windPot_int'   : windPot_int,
            'helPot_int'    : helPot_int,
            'windBraid_int' : windBraid_int,
            'helBraid_int'  : helBraid_int,
            'wind_int'      : wind_int,
            'hel_int'       : hel_int,
            'dL_int'        : dL_int,
            'dH_int'        : dH_int}

    del times, windCur, windPot, windBraid, helCur, helPot, helBraid, wind, hel, dL, dH
    del windCur_int, windPot_int, windBraid_int, helCur_int, helPot_int, helBraid_int, wind_int, hel_int, dL_int, dH_int

    
    ndf = pd.DataFrame(data)
    return ndf    

def readMap(outputDir,fileNum,varCode):
    
    '''
    This function will read an ARTop datafile and produce
    a map (using the SunPy Map class). 

    Input
    -----

    outputDir:   top output directory
    fileNum  :   number of the ARTop datafile (iteration number)
    varCode  :   code of the variable to select (see below)

    Output
    ------

    mapOut   :    map of selected data with FITS header data


    To view or maipulate readMap, please consult
    https://docs.sunpy.org/en/stable/guide/data_types/maps.html

    varCode key:
    
    bz         :  z-component of mag. field
    vz         :  z-component of vel. field
    sz         :  z-component of Poynting flux
    windCur    :  current-carrying part of winding
    helCur     :  current-carrying part of helicity
    windPot    :  potential part of winding
    helPot     :  potential part of helicity
    windBraid  :  braiding component of winding
    helBraid   :  braiding component of helicity
    wind       :  total winding
    hel        :  total helicity
    deltaL     :  delta winding flux
    deltaH     :  delta helicity flux


    '''
    headPath = os.path.join(outputDir,'header.txt')
    specPath = os.path.join(outputDir,'specifications.txt')
    outData = os.path.join(outputDir,'Data')

    specVar = readSpec(specPath)
    
    sharpNum = specVar[0]
    nx = specVar[1]
    ny = specVar[2]
    velSmooth = specVar[4]
    cutoff = specVar[5]
    sampling = specVar[6]
    fileName = 'windDatPotentialFastCO'+str(cutoff)+'_VS'+str(velSmooth)+'_'+str(sampling)+'_'+str(fileNum)+'.dat'
    filePath = os.path.join(outData,fileName)

    # Read and sort data files
    df = pd.read_table(filePath, sep=' ',header=None)
    df.columns = ['X1', 'X2', 'bz', 'vz', 'sz', 'windCur', 'helCur', 'windPot', 'helPot', 'windBraid', 'helBraid', 'wind', 'hel', 'deltaL', 'deltaH']
    n = 10
    df.drop(df.tail(n).index,inplace=True)
    df.sort_values(by=['X1','X2'],inplace=True)

    var = df[varCode].values

    # Adjust for sampling
    nx = int(nx)
    ny = int(ny)
    sampling = int(sampling)
    if (nx%sampling != 0):
        nx = int(nx/sampling) + 1
    else:
        nx = int(nx/sampling)
    if (ny%sampling != 0):
        ny = int(ny/sampling) + 1
    else:
        ny = int(ny/sampling)
                
    # Convert to an array
    Z = np.zeros((ny,nx))
    s=0
    for j in range(nx):
        for i in range(ny):
            Z[i,j] = var[i+s]
        s = s+i+1

    # Create header data
    varHead = readHead(headPath)
        
    rpx = float(varHead[0])/float(sampling)
    rpy = float(varHead[1])/float(sampling)

    observatory = varHead[2]
    instrument = varHead[3]
    detector = varHead[4]
    obsTime = varHead[5]
    rcx = float(varHead[6])
    rcy = float(varHead[7])
    scaleX = float(varHead[8])
    scaleY = float(varHead[9])
        
    scaleX = scaleX*float(sampling)
    scaleY = scaleY*float(sampling)

    # Units
    if varCode == 'bz':
        un = u.gauss
    elif varCode == 'vz':
        un = u.km/u.s
    elif varCode == 'sz':
        un = u.gauss * u.gauss * u.km / u.s
    elif varCode == 'windCur':
        un = u.km * u.km / u.s
    elif varCode == 'helCur':
        un = u.gauss * u.gauss * u.km * u.km / u.s
    elif varCode == 'windPot':
        un = u.km * u.km / u.s
    elif varCode == 'helPot':
        un = u.gauss * u.gauss * u.km * u.km / u.s
    elif varCode == 'windBraid':
        un = u.km * u.km / u.s
    elif varCode == 'helBraid':
        un = u.gauss * u.gauss * u.km * u.km / u.s
    elif varCode == 'wind':
        un = u.km * u.km / u.s
    elif varCode == 'hel':
        un = u.gauss * u.gauss * u.km * u.km / u.s
    elif varCode == 'deltaLf':
        un = u.km * u.km / u.s
    else:
        un = u.gauss * u.gauss * u.km * u.km / u.s
    
    # Build header
    coord = SkyCoord(rcx*u.deg, rcy*u.deg, 695700000*u.m, obstime=obsTime, observer='self', frame='heliographic_carrington')
        
    header = make_fitswcs_header(Z,coord,reference_pixel=u.Quantity(([rpx,rpy])*u.pixel), scale=u.Quantity(([scaleX,scaleY])*u.deg/u.pixel), telescope='SDO', instrument = 'HMI SIDE1',unit=un)
    
    # Build map
    mapOut = sunpy.map.Map(Z,header)

    return mapOut

def plot(X,Y,colour,label, ylabel,Name,unit, oplot = None, save=False, imagename = False, **kwargs):   
    '''
    Plot function for the timeseries data
    '''
    
    plt.rcParams.update({'font.size':16})
    plt.figure(figsize=(12, 10))
    plt.title( Name, loc='center', fontsize = 16)
    
    if unit == 'h':
        u=[3600,'hours'] 
    elif unit == 'm':
        u=[60,'minutes'] 
    else:
        u=[1,'seconds'] 
    
    xx = []
    for v in range(len(Y)):
        plt.plot(X/u[0],Y[v], colour[v], linewidth=2 ,label = label[v])
        xx.append(Y[v])
    
    if oplot is not None:
        combined = pd.concat(oplot)
        combined = combined.sort_values(by=[combined.columns[0]]).reset_index().drop('index', axis=1)
        time, color, label = combined[combined.columns[0]], combined[combined.columns[1]], combined[combined.columns[2]] 
        no_FL = True
        for v in range(len(time)):
            if no_FL == True and label[v] == 'FL':
                plt.plot(((time[v]/u[0]), (time[v]/u[0])), (np.min(xx),np.max(xx)), color[v], label=label[v])
                no_FL = False
            elif label[v] == 'FL':
                plt.plot(((time[v]/u[0]), (time[v]/u[0])), (np.min(xx),np.max(xx)), color[v])
            else:
                plt.plot(((time[v]/u[0]), (time[v]/u[0])), (np.min(xx),np.max(xx)), color[v], label=label[v])
            

    plt.ylabel(ylabel)
    plt.xlabel('Time ('+ u[1] + ')')   
    plt.ylim([np.min(xx), np.max(xx)])
    plt.xlim([np.min(X/u[0]),np.max(X/u[0])])
    plt.legend(loc=0, bbox_to_anchor=(1, 0.5), fontsize = 'x-small')
    plt.show()

    if save is True:
        imageName = imagename +'.jpg'
        plt.savefig(save + '/generated_images/'+ imageName)



'''
create dataframe with x rays and flare times 
'''
class flare_times:

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
                noaa= pd.DataFrame(l)
                noaa.rename(columns={0:time+'(seconds)',1:'color',2:'type'}, inplace=True)
                return noaa 
            else:
                print('No data for '+variable + ' for this region')
                return None 
        else:
            print('The region number '+str(activeregion)[-4:]+' is not found. Try another day.')

   

    def combine(list_of_lists):
        List=[]
        for l in list_of_lists:
            
            if l is not None:
                List += l.tolist()

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
            return pd.DataFrame(sorted_list)
        else:
            sys.exit('There is no data')
            


