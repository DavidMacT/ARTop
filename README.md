
Active Region Topology (ARTop) - a framework for studying topological quantities in solar active regions
---------------------------------------------------------------------------------------------------

This framework is for calculating and analysing photospheric topological
signatures of solar active regions. The code downloads SHARP magnetograms
and uses these to calculate maps and time series of quantities including
magnetic helicity and magnetic winding fluxes. Analysis routines allow for
the straightforward production of figures and allow for the detailed
comparison of time series to flare times. The framework is desiged to give 
flexibilty to the user for downloading, processing, and visualizing the data.

Requirements
------------


The code runs on Linux and requires Python 3, a C++ compiler and OpenMP.
SDO (Solar Dynamics Observatory) data analysis routines make use of SunPy (https://sunpy.org/) and, following their example,
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

`Region number` corresponds to the SHARP number of the active region and not the NOAA number. If you only know the NOAA number for a region and not the SHARP number, you can use the script `source/python/noaa_sharp.py` to find the SHARP number.

`Download data` is a binary variable. Setting this to `false` (not in uppercase) means that, if you have already downloaded the magnetogram, you can run the code without having to download them again. 

The time/date variables are self explanitory (always use two digits, e.g. 09, not 9). 

`Velocity smoothing` corresponds to how the window size used in the DAVE4VM code. 

The directory variables require the full paths in order to work properly, e.g. `/home/folder/input_folder` and not just `input_folder`. 

`Topology` means that you want to run the topological calculations (which are performed via a C++ code). (not in uppercase)

`Cutoff` is the minimum magnetic field strength that you wish to consider for the topological calculations. This value is measures in Gauss.

`Sampling` determines the resolution at which you perform the topological calculations. A value of 1 means that you include all the pixels in the calculation. A value of 3 means that you down sample the number of pixels by a factor of 3. Obviously, the larger this value is, the faster the calculation, but care is required to make sure that you do not miss important information (we offer no guarantees, only use this if you are confident that it is producing sensible output). 

`Remove_downloaded_images` allows you to delete all the downloaded magnetograms. Copies of these files (in a different format) are created, so the information in these files is not lost.

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

If you have changed the directory of the `run.py`, you will need to adjust the directory of the source file in the `sys.path` line.

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

If there are missing data, these will be skipped when producing the gif. Therefore, if many 
timesteps are missing, you should see a jump in the gif animation.

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

If you are woking on many active regions and are not able to store all of the downloaded magnetograms, you can store the raw data of the time series using the command

```
TS.save_raw(time,L,'winding')
```
which creates a file with `time` in the first column (this can be the variable from `TS.integrate_accum`, or it can be one that you create yourself) and the variable of choice (`L` in the above example) in the second column. The file is stored in `generated_images` and the format of the file name is the active region number followed by the name you give (`winding` in the above example).



NOAA reports
------------

The functionality of attaining flare data from daily NOAA reports was demonstrated in the above example.
We will now look into this useful feature in more detail.

The minimum information required is a time window in which to look at. For example,

```
start_time = '2011-02-10 20:00:00'
end_time = '2011-02-22 12:00:00'
report = noaa(start_time,end_time)
```
will extract information from the NOAA between the specified times. The reports are downloaded from 
www.solarmonitor.org  and the variable `report` contains the follwing the variables from the reports:

```
Event
Begin
Max
End
Type
Loc/Frq
Particulars
Reg
```
For flares, `Type` = `FLA` and for X-ray, `Type`= `XRA`. For the latter, `Particulars` gives the GOES classification. `Begin`, 
`Max` and `End` correspond to the beginning, maximum and end of the flare. `Reg` is the active region number.
`report` contains too much information, but it is simple to extract key flare data. For example, if you wish
to find the maxima of the X-ray peaks during the specified timespan for a particular active region, you just
need to specify that you want `Max` and give the NOAA active region name:

```
activeRegion = `AR11158`
variable = `XRA`
fltr_events = report.Filter(activeRegion,variable)
print(fltr_events)
```

For the above times, the output of the above code is

```
    Event  Begin    Max   End Type  Loc/Frq Particulars   Reg
12   6210   7236   7242   724  XRA  1-8A           B8.5  1158
18   6270   8429   8436   844  XRA  1-8A           C1.1  1158
19   6280   8544   8556   861  XRA  1-8A           C4.7  1158
26   6310   8928   8938   894  XRA  1-8A           M6.6  1158
64   6560   9835   9842   984  XRA  1-8A           C1.6  1158
69   6580  10029  10049  1010  XRA  1-8A           C8.3  1158
76   6620  10251  10258  1030  XRA  1-8A           C6.6  1158
81   6660  10439  10449  1050  XRA  1-8A           C1.8  1158
84   6680  10751  10800  1082  XRA  1-8A           C1.7  1158
87   6710  10947  11027  1104  XRA  1-8A           C7.0  1158
142  7650  17209  17215  1722  XRA  1-8A           C1.5  1158
143  7710  17333  17336  1734  XRA  1-8A           C1.0  1158
144  7720  17444  17447  1745  XRA  1-8A           C1.2  1158
146  7730  17609  17613  1761  XRA  1-8A           C1.2  1158
148  7760  17724  17730  1773  XRA  1-8A           C1.9  1158
151  7770  17825  17828  1783  XRA  1-8A           C2.6  1158
155  7800  18033  18036  1803  XRA  1-8A           C2.4  1158
158  8010  19302  19308  1931  XRA  1-8A           C1.7  1158
182  8460  21733  21737  2174  XRA  1-8A           C1.3  1158
185  8480  21821  21825  2183  XRA  1-8A           C1.8  1158
204  8660  22340  22344  2234  XRA  1-8A           C1.9  1158
213  9100  26613  26624  2662  XRA  1-8A           C4.6  1158
214  9110  26710  26715  2671  XRA  1-8A           C1.8  1158
215  9130  26804  26810  2681  XRA  1-8A           C1.5  1158
216  9140  26900  26906  2690  XRA  1-8A           C4.1  1158
218  9160  27301  27306  2730  XRA  1-8A           C2.7  1158
219  9170  27400  27404  2740  XRA  1-8A           C1.8  1158
220  9180  27408  27412  2741  XRA  1-8A           C7.8  1158
```

Note that if a report does not exist, the message `Report missing` will appear and the code will skip
to the next day. If there is a report but all that is written is `NO EVENT REPORTS.`, the code will also
skip to the next day.

For plotting flare information on time series, you can further filter the information using

```
ft = `Max`
XRAs = report.toseconds(activeRegion,variable,ft)
```
`XRAs` is a list that contains the maximum flare times in seconds relative to the start time, a colour for 
plotting (see below) and the GOES classification. When plotting, the colour classification is  

```
A-class cyan
B-class yellow
C-class green
M-class blue
X-class red
```

Batch mode
----------

The code has been desgined to process one active region at a time. This is because many files are downloaded
for the calculations and this way users can run the code easily on a laptop. If you have the resources, however,
you can run the code in batch mode and perform the calculations for several active regions in paralel. To do this,
first create a folder called `batch_info`. Inside this, create files `read_data_x.txt`, in which each file contains 
all the details from `read_data.txt` and `x` is a number to be looped through, e.g if you are running two active regions, the files should be numbered as read_data_1.txt and read_data_2.txt. Every `read_data_x.txt` can represent 
a different active region.` 

In `batch_run.sh`, change `number_of_files` to the number of files you created in `batch_info`. In `run_ARTop.sh`, 
comment out the line below `for a single active region` and uncomment the block below `for batch mode`. Then run 

```
./batch_run.sh
```


---------------

There is also the possibiilty to plot SDO maps and to download/plot GOES data directly and overplot with the time series data. Examples are given
in `run.py`. These are modifications of sunpy routines. Documentation for these can be found on the
sunpy webpage. More details of these may appear in the near future...


