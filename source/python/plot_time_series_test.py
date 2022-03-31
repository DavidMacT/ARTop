import create_timeseries
import plotting
import numpy as np
import goes

# region details
regionName = '956'
cutoff = 50
v_smoothing = 20
sampling = 1
dt = 720
path= '/home/dmac/test_codes/Data/AR_' + regionName
filenumber = 19  # number of netWindDatPotFast
filename_sr = create_timeseries.Name_file_sr(cutoff,v_smoothing,sampling,filenumber)

# ARTop variables
variables = create_timeseries.read_timeseries_data(path,filename_sr)

# pick variables
L = variables['totWind']
deltaL = variables['deltaL']
intL, time = create_timeseries.acctrapz(L,dt,0)

# scale factors
int_factor = 1.0e5**4     # cm^4
factor = int_factor*720.0 # cm^4 s
intL = intL*int_factor
deltaL = np.array(deltaL)*factor

# find running mean and standard deviation envelope
mean, std = create_timeseries.mean_std(deltaL, n_points = 2) # 50 means 10h

# place 3*std envelope relative to the running mean
mstd = mean + 3*std

# plot variables
overplot_variables = [deltaL,mean,mstd]
colour = ['c-','r-','b:']
label = ['$\dot{\delta L}$', '$\mu(\dot{\delta L})$', '$\mu+3\sigma$']
# uncomment if you want a plot without flare times
#create_timeseries.plot(time,overplot_variables,colour,label,r'$\dot{\delta L}$' + 'cm^4 s', 'times series example',path,filename_sr,unit='h',save=True)

# overplot flare times and type from GOES data
# select start and end times for GOES data (these should match the times of your time series)
start_time = "2015-06-21 00:00"
end_time = "2015-06-22 00:00"

# get data from the correct satellite (this will be GOES 15, 16 or 17)
flares = goes.goes_xray(start_time,end_time,SatelliteNumber = 15)

# get the data
picked_time, fluxA, fluxB = flares.get_data()

# plot the X-ray times series
# flares.plot_xray(picked_time,fluxA,fluxB)

# find the X-ray peaks (flare times) and select the class (C, M or X)
pt, pv = flares.xray_peaks(fluxB,'M')

# overplot the flare times on the time series
create_timeseries.plot(time,overplot_variables,colour,label,r'$\dot{\delta L}$' + 'cm$^4$ s', 'times series example',path,filename_sr,unit='h',Xray_class=pt,save=True)
