import create_timeseries
import plotting
import numpy as np

# This script allows the user to plot maps of the variables outputted by the ARTop code

# input details
regionName='956'
filenumber = 10
cutoff = 50
v_smoothing = 20    
sampling = 1 
path= '/home/dmac/test_codes/Data'

# get the data ready for plotting
#path, filename = plotting.name_file(path,regionName,cutoff,v_smoothing,sampling,filenumber)
#nx, ny = plotting.getnxny(path)
#variables = plotting.read_data(path,filename)
#sorted_variables = plotting.sort(variables,'X1')

# example 1

# get the vertical component of the magnetic field
#bz = plotting.convert_array(sorted_variables['bz'],nx,ny)
# plot its map
#plotting.plotmap(bz,nx,ny,'bz','Bz',path,filename,save=True)

# example 2

# plot a map of the field line winding
#windvalCur= plotting.convert_array(sorted_variables['windvalCur'],nx,ny)
#windvalPot= plotting.convert_array(sorted_variables['windvalPot'],nx,ny)
#windvalVelOnly= plotting.convert_array(sorted_variables['windvalVelOnly'],nx,ny)
# create the winding variable from the above three    
#total_winding = windvalCur + windvalPot - windvalVelOnly
# plot its map
#plotting.plotmap(total_winding,nx,ny,'winding',r'$L_w$',path,filename,save=True)

# example 3 (comment the other examples before running this one)

# create a list of maps
temp_path = "first"
for filenumber in range(2,11):
    path, filename = plotting.name_file(path,regionName,cutoff,v_smoothing,sampling,filenumber)
    if temp_path == "first":
        temp_path = path
    nx, ny = plotting.getnxny(temp_path)
    variables = plotting.read_data(temp_path,filename)
    sorted_variables = plotting.sort(variables,'X1')

    uz = plotting.convert_array(sorted_variables['vz'],nx,ny)
    plotting.plotmap(uz,nx,ny,'uz','Uz',temp_path,filename,save=True)


