import plotting
import numpy as np

# produce a gif movie of maps that have been saved as jpg

# basic details
regionName = '956'
path = "/home/..."
filename = "bz_CO50_VS20_1_*.jpg"
variable_name = "Bz"
ploting.plot_gif(path,filename,variable_name,regionName)
