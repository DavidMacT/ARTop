# ARTop
Active Region Topology - a tool for studying topological quantities in solar active regions

<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
Requirements
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

The code runs on Linux and requires a C++ complier (for the main calculations) and Python (for the analysis routines).

<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
Downloading and calculating the topological quantities 
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

- Insert the active region number in the read_data.txt file WITHOUT any space.

- The Output directory = '/home/...' 
  ---> is the directory where the files of input and output will be created at the beginning.

- To download the data of the active region, make the Download_data=true.
 
- To calculate the topological quantities, make the Topology=true. 


The tool is designed to give the user flexibily to download the data and compute the topological quantities at the same time or at different times.
For example, if Download_data = false, the code will read the saved data that are downloaded previously in the input file without downloading the data again.

There is an option to remove the downloaded data in input file after calculating the model finish the topological quantities. These latter quantities are saved in the output file.

***Note: since many files are downloaded as part of these calculations, it is worth keeping a check on the cache folder. This is not done automatically in the code as the location of the cache folder will vary between different machines.

<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
Visualizing the data
<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>

There are two main packages for:
(1) Visualizing and mapping the computed quantities in the output file.
(2) Downloading and plotting the data of GOES x-ray and SDO.

The way to use these two packages are demonstrated in the run.py script in the directory ".../source/python/run.py".
The guidence of using thes packages are explained in the follows.


1. Visualizing and mapping the computed quantities.
---------------------------------------------------
To map and plot the integrated time series of the quantities, call the package named "winding" as follows

>>> import winding 

Then insert the following:
(1) Active region number
(2) File number that you are interested to plot.
(3) The path directory that is used in the file "read_data.txt". 
<For example> if the pathe in the file is ".../ARTop_winding", put the path directory as ".../ARTop_winding/Data"

That is commented in the run.py script.


1.1 Mapping the topological quantities
--------------------------------------
The quantities that are avialable in the created file in the output folder as:

'bz':               z-component of the magnetic field
'vz':               z-component of the field line velocity field
'sz':               z-component of the Poynting flux
'wind':             winding
'windvalCur':       current-carrying winding
'windvalPot':       potential winding
'windvalBraidOnly': braiding winding
'hel':              helicity
'helvalCur':        current-carrying helicity
'helvalPot':        potetnial helicity
'helvalBraidOnly':  braiding helcity
'deltaLflux':       delta winding flux
'deltaHflux':       delta helecity flux


To map <for example> 'bz', the winding mapping should be called as

>>> Map = winding.mapping(activeRegion, path_directory)

Then, read the 'bz' data first from the interested file that is created in the output folder as

>>> bz = Map.read_data('bz', number_of_file)

NOTE, you should USE the same abbreviation names of the quantities in the code to get the data. To map this quantity, run the following

>>> Map.plotmap(bz,title=r'$B_z$')

You can save and zoom in the created map. Also, you can creat animation for the quabtity "bz" from the file number, for example, 150 to file number 300 as follows

>>> im_start_No = 150
>>> im_end_No = 300
>>> variable_name = 'bz' 
>>> Map.plot_gif(im_start_No, im_end_No, variable_name, title = 'bz')



1.2 Plot integrated time series of the quantity
-----------------------------------------------
This function uses the trapzoidal method to integrate a quantity over the whole time range that is specified in the read_data.txt file, i.e. the whole time range of the downloaded data.

The avialable integrated quantities are:

'totWindCur':           total current-carrying winding
'totWindPot':           total potential winding 
'totWindBraid':         total braiding winding
'totHelCur':            total current-carrying helcity 
'totHelPot':            total potential helicity
'totHelBraid':          total braiding helicity 
'totWind':  		total winding
'totHel':   		total helicity
'deltaLflux':           delta winding flux
'deltaHflux':           delta helicity flux



call the time series function as

>>> TS = winding.timeseries(activeRegion, path_directory)

You can get the data from the files, <for example> 'totWind', as

>>> L = TS.read_data( 'totWind')

You can integrate the quantity of 'totWind' directly without the previous command by runnig

>>> intL, time = TS.integrate_accum('totWind',dt)

The value dt is the time step of calculating the quantities in the files. This command returns the accumulated integrated value with time, that will be used for the time series plot.

** DO rescale the units of the quantities. 


To calculate the average anf standard deviation of any quantity, use the command

>>> mean, mstd = TS.mean_std(intL, n_points = 2, factor = 3)

intL is the required variable. This function creat a dynamic window of length n_points to calculate the mean and standard deviation. <For example> if n_points = 2, the mean and standard deviation will be calculated for each two consecutive points. 
factor = 3 is a factor to return mstd as three times the standard deviation, i.e. 3*standard deviation.


To overplot the integrated variable with the average values and mstd, put them in a form of list as following:

>>> overplot_variables = [intL, mean, mstd]

choose their colors and labels on the plot as

>>> colour = ['c-','r-','b:']
>>> label = ['$\dot{\delta L}$', '$\mu(\dot{\delta L})$', '$\mu+3\sigma$']

plot them using the command

>>> y_label = r'$winding$'+' '+ '$(km^4)$'
>>> title = 'test'
>>> TS.plot(time, overplot_variables, colour,label,y_label, title, unit='h')

If unit='h' or 'm' or 's', the time unit on x-axis will be hour or minute or second, respectively.


---------------------------------*************************************---------------------------------
2. Downaloading and plotting the data of GOES x-ray and SDO.
------------------------------------------------------------
To download data from Goes satelites or SDO, call the observation package as

>>> import observation


2.1 Goes x-ray
--------------

specify the time range and satelite number of Goes x-ray data as

>>> start_time = "2021-06-18 18:00"
>>> end_time = "2021-06-20 00:00"
>>> X = observation.goes_xray(start_time, end_time,SatelliteNumber = 16)


SatelliteNumber could be GOES number of 13, 14, 15, 16, and 17. To get the data, run

>>> picked_time, fluxA, fluxb = X.get_data() 

This give the data of the flux A and flux B of x-rays and their corresponding time. Flux A and flux B are different waveslengths of x-ray. 

If you want to save the files that are download from GOES, make save_files = True and you can also put the directory to save the files

>>> path = ".../Data/goesdata"
>>> picked_time, fluxA, fluxb = X.get_data(path, save_files = False) 


Plot these variables with time as

>>> X.plot_xray(picked_time, fluxA, fluxb)

To pick the peaks above a particular x-ray class and their time during the selected time period, run the command
 
>>> pt, pv = X.xray_peaks(fluxb,'C') 

where pt return the time of the peaks and pv contains the peaks values. This example gives the peaks above 'c' class. If you replace it with 'M' or 'X', it will give the peaks above M-class or X-class of x-ray. 

fluxb could also be replaced by fluxA as required by the user.

** It is better to not use this peak function for long time periods, i.e. days, because peaks are relative and when the data is so long, some peaks will not be considered any more repect to the others. 


- The obtianed times of x-ray peaks could be over plotted the integrated time series plot as following:

>>> y_label = r'$winding$'+' '+ '$(km^4)$'
>>> title = 'test'
>>> TS.plot(time, overplot_variables, colour,label,y_label, title ,unit='h', Xray_class=pt)


2.2 SDO solar map
-----------------
To get the location of an active region, go to the link 
"https://www.spaceweatherlive.com/en/solar-activity/region/... .html"
<For example> the location of the active region AR12673 could be obtained from
AR + 12673
"https://www.spaceweatherlive.com/en/solar-activity/region/12673.html"

To download and map the SDO files, run command as

>>> location = 'S08E23'
>>> start = '2017-09-02 00:00:00'
>>> end = '2017-09-02 00:05:00'
>>> Ob = observation.SDO(start, end, location)

The plotting function can present the AIA map of the active region for a single wavelength or different wavelengths or hmi side by side. The required map should be put in as list as following

>>> Ob.get_map(graphs=['1600', '171','hmi'])
>>> Ob.plot()

If you want to spot the magnetic field strength on the AIA map or hmi, specify the required values of magnetid field strength and the desired map as 

>>> required_levels = [20, 100, 140, 300, 500]
>>> Ob.plot(field_strength_spots = '1600', level=required_levels)



