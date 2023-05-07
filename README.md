
Active Region Topology (ARTop) - a tool for studying topological quantities in solar active regions
---------------------------------------------------------------------------------------------------

ARTop allows you to calculate and analyse topological signatures of solar active regions at the photosphere. The code downloads SHARP magnetograms and uses these to create maps and time series of quantities including magnetic helicity and magnetic winding fluxes. Analysis routines allow for the straightforward production of figures and for the detailed comparison of time series to flare times.



Installation
------------

ARTop is designed to run on Linux and has the following dependencies: a C++ compiler, OpenMP, Python 3, SunPy (https://sunpy.org/), LAPACK (http://www.netlib.org/lapack/) and BLAS (http://www.netlib.org/blas/).

In developing ARTop, we have used GNU compilers (https://gcc.gnu.org/).

For users who do not have Python installed currently on their machines, we recommend installation via Miniconda 3 (https://docs.conda.io/).

------------------

The installation of LAPACK and BLAS requires some extra steps which we now outline. 

Note: depending on how your system is setup, you may need to install gfortran (https://gcc.gnu.org/fortran/) and binutils (https://www.gnu.org/software/binutils/).

First, download LAPACK and BLAS.

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
Please read through this entire section before running ARTop.


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

`manual` assumes that the user has placed a tar file (containing the FITS files to be read) in the input folder (normally, this folder is created automatically by ARTop but in this instance, the user must create it). The code will then process this file to produce the necessary input and parameter files. This last option is for users with slow connection speeds or who already have the tar files they require. For more details on downloading tar files from the JSOC website, please cosult http://jsoc.stanford.edu/How_toget_data.html.

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

Note: when running different batches of parameters, we recommend that you use separate input and output folders for each set of parameters - don't cross the streams! 

Output variables
----------------

There are several Python scripts that allow for the production of maps and time series. Examples of how to use the analysis routines are provided in Jupyter notebooks that accompany ARTop. Below are lists of the variables names used in the analysis routines of ARTop.

The variables for creating maps are:

```
bz         :  z-component of mag. field (Gauss)
vz         :  z-component of vel. field (km/s)
sz         :  z-component of Poynting flux (Gauss^2km/s)
windCur    :  current-carrying part of field line winding rate (km^2/s)
helCur     :  current-carrying part of field line helicity rate (Gauss^2km^2/s)
windPot    :  potential part of field line winding rate (km^2/s)
helPot     :  potential part of field line helicity rate (Gauss^2km^2/s)
windBraid  :  braiding component of field line winding rate (km^2/s)
helBraid   :  braiding component of field line helicity rate (Gauss^2km^2/s)
wind       :  total field line winding rate (km^2/s)
hel        :  total field line helicity rate (Gauss^2km^2/s)
deltaL     :  field line delta winding rate (km^2/s)
deltaH     :  field line delta helicity rate (Gauss^2km^2/s)
```
There are also variables for creating time series:

```
windCur   : current-carrying part of winding rate (km^4/s)
helCur    : current-carrying part of field line helicity rate (Gauss^2km^4/s)
windPot   : potential part of winding rate (km^4/s)
helPot    : potential part of helicity rate (Gauss^2km^4/s)
windBraid : braiding component of winding rate (km^4/s)
helBraid  : braiding component of helicity rate (Gauss^2km^4/s)
wind      : total winding rate (km^4/s)
hel       : total helicity rate (Gauss^2km^4/s)
dL        : delta winding rate (km^4/s)
dH        : delta helicity rate (Gauss^2km^4/s)
```

