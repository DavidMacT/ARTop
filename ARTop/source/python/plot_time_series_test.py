import create_timeseries
import plotting
import numpy as np

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


create_timeseries.plot(time,overplot_variables,colour,label,r'$\dot{\delta L}$' + 'cm^4 s', 'times series example',path,filename_sr,unit='h',save=True)
