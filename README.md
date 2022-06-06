
Active Region Topology (ARTop) - a tool for studying topological quantities in solar active regions
---------------------------------------------------------------------------------------------------

This code is for calculating and analysing photospheric topological
signatures of solar active regions. The code downloads SHARP magnetograms
and uses these to calculate maps and time series of quantities including
magnetic helicity and magnetic winding fluxes. Analysis routines allow for
the straightforward production of figures and allow for the detailed
comparison of time series to flare times.

Requirements
------------


The code runs on Linux and requires Python 3, a C++ compiler and OpenMP.
Several of the analysis routines make use of SunPy and, folowing their example,
we recommend that Python (and its necessary packages) be installed via Miniconda 3.


Running the code
----------------

In the main code directory, first open `read_data.txt`. You should see a list like:

```
Region number=7115,
Download_data=true,
Start year=2017,
Start month=08,
Start day=28,
Start hour=09,
End year=2017,
End month=08,
End day=28,
End hour=10,
Velocity smoothing=20,
Input directory=/home/your_directory/input,
Output directory=/home/your_directory/output,
Topology=true,
Cutoff=50,
Sampling=1,
Remove_downloaded_images=false,

```

The `Region number` corresponds to the SHARP number of the active region and not the
NOAA number. `Download data` is a binary variable. Setting this to `false` means that, if
you have already downloaded the magnetogram, you can run the code without having to
download them again. The time variables are self explanitory (always use two digits, e.g.
09, not 9). `Velocity smoothing` corresponds to how the window size used in the DAVE4VM
code. The directory variables are self explanitory but these directories must be created
before running the code. `Topology` means that you want to run the topological calculations
(which are performed via a C++ code). `Cutoff` is the minimum magnetic field strength that
you wish to consider for the topological calculations. This value is measures in Gauss.
`Sampling` determines the resolution at which you perform the topological calculations.
A value of 1 means that you include all the pixels in the calculation. A value of 3 means
that you down sample the number of pixels by a factor of 3. Obviously, the larger this value
is, the faster the calculation, but care is required to make sure that you do not miss important
information. `Remove_downloaded_images` allows you to delete all the downloaded magnetograms.
Copies of these files (in a different format) are created, so the information in these files is
not lost.

Once you have selected values for all the variables in `read_data.txt`, you can run the code by
typing:

```
./run_ARTop.sh > outfile &
```
Details of how the code is progressing will be stored in `outfile`.


This bash script calls a series of Python and C++ scripts. If selected, a python script
will first download the magnetograms between the specified start and end times. Another
Python script will determine the corresponding velocity profiles using the DAVE4VM code
(Schuck, P. W. Tracking vector magnetograms with the magnetic induction equation.
Astrophys. J. 683, 1134–1152 (2008)). The original Python version of DAVE4VM can be
found here: https://github.com/Chicrala/pydave4vm. A Python script is then called to
calculate the potential field based on the values of `Bz`. In the output directory, in a folder
called Data, all the magnetic field and velocity files are stored. These are used as input
for a C++ code that calculates all the topological quantities. Files beginning with `windDataPotentialFast`
are created and contain the variables that can be used to create maps and time series. These
are discussed later.


Output variables
----------------

There are several Python scripts that allow for the easy production of maps and time series.
Examples of how to perform specific plots are given in run.py.Before describing the different
plots, it is important to understand the structure of the variables in the
`windDataPotentialFast` files. The variables for creating maps are:

```
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
```
There are also variables for creating time series:

```
'totWindCur':           total current-carrying winding
'totWindPot':           total potential winding 
'totWindBraid':         total braiding winding
'totHelCur':            total current-carrying helcity 
'totHelPot':            total potential helicity
'totHelBraid':         	total braiding helicity 
'totWind':  	        total winding
'totHel':   	        total helicity
'deltaLflux':           delta winding flux
'deltaHflux':           delta helicity flux
```

These variables are actually time derivatives but it is straightforward to produce their
accummulations integrated in time. We will now go though an example of each type of plot.


Plotting routines
-----------------

All plotting routines are written in Python. Here, we follow the layout in `run.py`, please
consult this for further details. These routines are there to provide you with a fast and
informative look at the data. Publication-standard figures will be up to you. The packages
that need to be imported for plotting are:

```
import os
import sys
sys.path.insert(1, os.getcwd() + '/source/python/')
import winding as win
import observation
from observation import NOAAreport as noaa

```

It is important to specify the SHARP number of the active region, such as

```
activeRegion = 7115
```

Each time the code is run, it adds information to `savedpaths.dat` which stores the active
region SHARP number beside the output file directory. In this way, you do not need to specify
the file directory and can use the command

```
created_files_dir= win.getpaths(os.getcwd(), activeRegion)
```

To create a map, you can use

```
Map =win.mapping(activeRegion,created_files_dir)
vz = Map.read_data('vz', filenumber)
Map.plotmap(vz,title=r'$v_z$',save=True,variable_name='vz')
```

By setting `save=True`, you produce a jpg in the folder `generated_images`, which is created
automatically in the output directory. `filenumber` is the number of the file from which you
want to create a map. `variable_name` is used to create the file name.


Producing a gif of maps is also straightforward and can be achieved with a few lines of code:

```
Map =win.mapping(activeRegion,created_files_dir)
im_st_No = 1
im_ed_No = 100
variable_name = 'bz'  
X=Map.plot_gif(im_st_No, im_ed_No, variable_name, title = 'bz')

```

---------------

To create a time series, first read in the data for a particular active region:

```
TS = win.timeseries(activeRegion, created_files_dir)
```

To study the winding `L`, for example, first use the following commands:

```
dLdt = TS.read_data('totWind')
L, time = TS.integrate_accum('totWind')
```

The variable `dLdt`, its accumulation (`L(t)`) and `time` are now available for plotting. `time`
is in seconds and distance in km. To convert `L` to cm^4, set `L=l*1.0e5**4`.

As well as the basic plotting function, we also include here how to overplot a running mean with
a standard deviation envelope (which can help to identify important spikes in the time series).
These quantities are calculated using

```
mean, mstd = TS.mean_std(dLdt,n_points = 2,factor = 3)
```

`n_points` is the number of previous time steps used to create the running mean. `factor` determines
the number of standard deviations used for the envelope. Now you can combine the variables for
plotting

```
overplot_variables = [dLdt,mean,mstd]
colour = ['c-','r-','b:']
label = ['$\dot{L}$', '$\mu(\dot{L})$', '$\mu+3\sigma$']

y_label = r'$winding$'+' '+ '$(cm^4)$'
title = 'Winding_rate'
```

These commands are self-explanatory. `title` will appear both as a title and as part of the figure
file name if that option is selected. The plot command is

```
X=TS.plot(time,overplot_variables, colour,label,y_label,title,unit='h',oplot=None,save=True)

```

The only entries that need explanation are `oplot` (this is described below) and `save`. Selecting
`save` creates a jpg in the `generated_images` folder in the output directory.


To overplot flare times, information from the daily NOAA reports can be used. These data can easily
by included in time series plots. An example is given below. Before the time series commands given
above, consider the following commands:

```
start_time = '2011-10-11 20:00:00'
end_time = '2011-10-16 12:00:00'
variable = 'XRA'
AR = 'AR11318'
tr = 'Max'
report = noaa(start_time,end_time)
showAll = report.show()
XR = report.toseconds(AR,'XRA',tr)
```

These commands download the NOAA reports between the stated times. `report` allows you to see all the
details. In this example, a list of the maximum of GOES X-ray peaks has been selected for the given
time span. In the `TS.plot` command above, these times are included with `oplot=XR`. By exploring the
NOAA reports and modifying the above code, you can also plot other available data.


---------------

There is also the possibiilty to download GOES data directly and to plot SDO maps. Examples are given
in `run.py`. These are modifications of sunpy routines. Documentation for these can be found on the
sunpy webpage. More details of these may appear in the near future...


