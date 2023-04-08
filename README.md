
Active Region Topology (ARTop) - a package for studying topological quantities in solar active regions
---------------------------------------------------------------------------------------------------

ARTop allows you to calculate and analyse topological signatures of solar active regions at the photosphere. The code downloads SHARP magnetograms and uses these to create maps and time series of quantities including magnetic helicity and magnetic winding fluxes. Analysis routines allow for the straightforward production of figures and for the detailed comparison of time series to flare times.



Installation
------------

ARTop is designed to run on Linux and has the following dependencies: a C++ compiler, OpenMP, Python 3, SunPy (https://sunpy.org/), LAPACK (http://www.netlib.org/lapack/) and BLAS (http://www.netlib.org/blas/).

In developing ARTop, we have used GNU compilers (https://gcc.gnu.org/).

For users who do not have Python installed currently on their machines, we recommend installation via Miniconda 3 (https://docs.conda.io/).

------------------

The installation of LAPACK and BLAS requires some extra steps which we now outline. First, download LAPACK and BLAS from the above cites.

In the BLAS folder type:

`make`

After that, type:

`mv blas_LINUX.a libblas.a`

to create a library called `libblas.a`. Now copy this to your library folder:

`sudo cp libblas.a /usr/local/lib`

Now go to the LAPACK folder and, if necessary, type:

`mv make.inc.example make.inc`

Then type:

`make`

Now copy this to your library folder:

`sudo cp liblapack.a /usr/local/lib`

ARTop's makefile is configured to read these libraries in this folder. If you wish to save them elsewhere, you will need to edit the path in the makefile.

-------------------

Running the code
----------------
Pease read through this entire section before running ARTop.


In the main code directory, first type:

`make`

which will compile the C++ part of the code.

The next file to consider is `read_data.txt`. This is the only file that needs to edited in order to run ARTop. Open `read_data.txt' to see a list like:

```
Region number=7115
Download data=true
Start year=2017
Start month=08
Start day=28
Start hour=09
End year=2017
End month=08
End day=28
End hour=10
Velocity smoothing=20
Input directory=/home/your_directory/input
Output directory=/home/your_directory/output
Topology=true
Cutoff=50
Sampling=1
Remove_downloaded_images=false
Registered email=mocenigo@palazzo_ducale.it

```

`Region number` corresponds to the SHARP number of the active region and not the NOAA number. If you only know the NOAA number for a region and not the SHARP number, you can use the script `source/python/noaa_sharp.py` to find the SHARP number.

`Download data` has three settings: 

`true` means that the code will automatically download the data and create the necessary input and parameter files. 

`false` assumes that the necessary input and parameter files already exist (e.g. from a previous run). 

`manual` assumes that the user has placed a tar file (containing the FITS files to be read) in the input folder. The code will then process this file to produce the necessary input and parameter files. This last option is for users with slow connection speeds. 

The time/date variables are self explanitory (always use two digits, e.g. 09, not 9). 

`Velocity smoothing` corresponds to the window size used in the DAVE4VM code. 

The directory variables require the FULL PATHS in order to work properly, e.g. `/home/folder/input_folder` and not just `input_folder`. 

`Topology` means that you want to run the topological calculations.

`Cutoff` is the minimum magnetic field strength that you wish to consider for the topological calculations. This value is measured in Gauss.

`Sampling` determines the resolution at which you perform the topological calculations. A value of 1 means that you include all the pixels in the calculation. A value of 3 means that you down sample the number of pixels by a factor of 3.

`Remove downloaded images` allows you to delete all the files in the input folder.

`Registered email` is required for downloading SHARP magnetograms from JSOC (http://jsoc.stanford.edu/). To register your email, see http://jsoc.stanford.edu/ajax/register_email.html. YOU MUST REGISTER YOUR EMAIL BEFORE YOU CAN RUN ARTop.

Once you have selected values for all the variables in `read_data.txt` and saved it, you can run the code by
typing:

`python run_ARTop.py`



Output variables
----------------

There are several Python scripts that allow for the easy production of maps and time series. Examples of how to use the analysis routines are provided in Jupyter notebooks that accompany ARTop.

The variables for creating maps are:

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
'totWind':              total winding
'totHel':               total helicity
'deltaLflux':           delta winding flux
'deltaHflux':           delta helicity flux
```

These variables are actually time derivatives but it is straightforward to produce their accummulations integrated in time. 

The output files containing these variables are located in the Data folder of the output directory. 
