#!/usr/bin/env python
# coding: utf-8


import os
import matplotlib.pyplot as plt
import numpy as np
import math
import sys
from sunpy.net import jsoc, attrs as a
import sunpy.map
from astropy.io import fits
import datetime
from datetime import timedelta
import tarfile
import glob
import drms

'''
MagDown.py

This script downloads the magnetogram fits files and
converts the data into a sutable formatfor the C++ code 
that calculates the topological quantities.

The inputs to this script are:

1.  regionNum
2.  startYear
3.  startMonth
4.  startDay
5.  startHour
6.  endYear
7.  endDay
8.  endHour
9.  inputDir
10. outputDir
11. regEmail
12. downloadData
13. velSmooth
14. cutoff
15. Sampling
'''

# Inputs
regionNum = sys.argv[1] # SHARP number
startYear = sys.argv[2]
startMonth = sys.argv[3]
startDay = sys.argv[4]
startHour = sys.argv[5]
endYear = sys.argv[6]
endMonth = sys.argv[7]
endDay = sys.argv[8]
endHour = sys.argv[9]
inputDir = sys.argv[10]
outputDir = sys.argv[11]
regEmail = sys.argv[12]
downloadData = sys.argv[13]
velSmooth = sys.argv[14]
cutoff = sys.argv[15]
sampling = sys.argv[16]

startTime = startYear+'.'+startMonth+'.'+startDay+'_'+startHour+':00:00'
endTime = endYear+'.'+endMonth+'.'+endDay+'_'+endHour+':12:00'


def downloadMag(start,end,inputFolder,sharp,email):
    '''
    This function queries JSOC and downloads vector magnetograms of
    the type hmi.sharp_cea_720s. At each time, all three components
    are downloaded.

    Note that for this function to work, you will need to register your
    email address with JSOC. If you have not done this, please go to
    http://jsoc.stanford.edu/ajax/register_email.html. 

    Input:
    ------
    start      : start time (format YYYY.MM.DD_HH:00:00)
    end        : end time (format YYYY.MM.DD_HH:00:00)
    inputFolder: where the fits files are saved
    sharp      : SHARP number of active region
    email      : email registered with JSOC

    Output:
    -------
     in inputFolder 	


    A tar file is downloaded to inputFolder, containing Br, Bp 
    and Bt fits files for every time dump between start and end 
    (inclusive). This is the preferred method of download for JSOC.
    '''

    client = drms.Client(email=email, verbose=True) 
    query_string = 'hmi.sharp_cea_720s['+sharp+']['+start+'-'+end+']'+'{Bp,Br,Bt}'
    export_request = client.export(query_string, method='url-tar', protocol='fits')
    export_request.wait()
    export_request.download(inputFolder)
    
    
def removeNaNs(field):
    '''
    This function removes all NaNs from the array of a magnetic field component

    Input:
    ------
    Bp, Bt or Br array
    
    Output:
    -------
    Array with NaNs removed (it does what it says on the tin!)
    '''
    
    size1 = np.size(field,0)
    for i in range(0,size1):
        size2 = field[i].size-1
        for j in range(0,size2):
            if math.isnan(field[i][j]):
                field[i][j] = 0.0


def frontName(time,regionNum):
    '''
    Builds the front part of the fits filename for reading files.

    Input:
    ------
    time      : (datetime) timestep of loop through magnetograms
    regionNum : (string) SHARP number of active region
    

    Output:
    -------
    String with front part of filename, e.g. hmi.sharp_cea_720s.377.20110214_003600_TAI
    '''
    
    yt = str(time.year)
    if time.month < 10:
        mt = str(time.month).zfill(2)
    else:
        mt = str(time.month)
    if time.day < 10:
        dt = str(time.day).zfill(2)
    else:
        dt = str(time.day)
    if time.hour < 10:
        ht = str(time.hour).zfill(2)
    else:
        ht = str(time.hour)
    if time.minute == 0:
        mint = '00'
    else:
        mint = str(time.minute)
        
    return 'hmi.sharp_cea_720s.'+regionNum+'.'+yt+mt+dt+'_'+ht+mint+'00_TAI'


def readBx(pathB,inputDir,regionNumber,i):
    '''
    Read FITS file for Bp and output Bx file

    Input:
    ------
    pathB       :  fits file path
    inputDir    :  location of files
    regionNumber:  SHARP number of active region
    i           :  iteration number


    Output:
    -------
    Bx files in inputDir

    '''

    bxIn = sunpy.map.Map(pathB)
    bx = bxIn.data
    removeNaNs(bx)
    a_file = os.path.join(inputDir,'bx_'+regionNumber+'_{:d}.txt'.format(i))
    np.savetxt(a_file, bx[:,:].flatten())

def readBy(pathB,inputDir,regionNumber,i):
    '''
    Read FITS file for Bt and output By file

    Input:
    ------
    pathB       :  FITS file path
    inputDir    :  location of files
    regionNumber:  SHARP number of active region
    i           :  iteration number


    Output:
    -------
    By files in inputDir
    
    '''
    byIn = sunpy.map.Map(pathB)
    by = byIn.data
    by = -1.0*by
    removeNaNs(by)
    a_file = os.path.join(inputDir,'by_'+regionNumber+'_{:d}.txt'.format(i))
    np.savetxt(a_file, by[:,:].flatten())

def readBz(pathB,inputDir,regionNumber,i):
    '''
    Read FITS file for Br and output Bz file

    Input:
    ------
    pathB       :  FITS file path
    inputDir    :  location of files
    regionNumber:  SHARP number of active region
    i           :  iteration number


    Output:
    -------
    Bz files in inputDir
    
    '''
    bzIn = sunpy.map.Map(pathB)
    bz = bzIn.data
    removeNaNs(bz)
    a_file = os.path.join(inputDir,'bz_'+regionNumber+'_{:d}.txt'.format(i))
    np.savetxt(a_file, bz[:,:].flatten())


'''

Main code:

'''
    
                
# Download tar file and extract FITS files
if downloadData.casefold() == 'true':
    downloadMag(startTime,endTime,inputDir,regionNum,regEmail)

# Check that the tar file has reached the input folder
if not glob.glob(os.path.join(inputDir,'*.tar')):
    raise sys.exit('ERROR: tar file not found, possible connection issue. Consider a manual download (see online documentation).')
    
# Extract tar file to get FITS files
for file in glob.glob(os.path.join(inputDir,'*.tar')):
    nameTar = file

with tarfile.open(nameTar,'r') as tf:
    print('Opened tar file')
    tf.extractall(inputDir)
    print('All FITS files extracted')
    
# Determine the number of time steps
st = datetime.datetime(int(startYear),int(startMonth),int(startDay),int(startHour),0,0)
et = datetime.datetime(int(endYear),int(endMonth),int(endDay),int(endHour),12,0)
jump = timedelta(minutes=12)
nt = int((et-st)/jump)
writeInitialData = True
# Time dumps between start and end times inclusive, plus one extra for topology calculations later
for i in range(nt+1):
    # Time step
    print('Iteration number: '+str(i))
    time = st + i*jump
    print(time)
    # Build FITS filename structure
    front = frontName(time,regionNum)

    # Check that all three vector component files exist
    pathBp = os.path.join(inputDir,front+'.Bp.fits')
    pathBt = os.path.join(inputDir,front+'.Bt.fits')
    pathBr = os.path.join(inputDir,front+'.Br.fits')
    checkBp = os.path.exists(pathBp)
    checkBt = os.path.exists(pathBt)
    checkBr = os.path.exists(pathBr)

    if ((checkBp == True) and (checkBr == True) and (checkBt == True)):
        # Read the Bx, By and Bz component files and generate files for input into the C++ code
        # Note that in the files, Bx=Bp, By=-Bt and Bz=Br
        readBx(pathBp,inputDir,regionNum,i)
        readBy(pathBt,inputDir,regionNum,i)
        readBz(pathBr,inputDir,regionNum,i)

        # Output useful data to text files for other scripts to use
        if (writeInitialData == True):
            # Read and store relevant header data
            tempFile = os.path.join(inputDir,front+'.Br.fits')
            tempIn = sunpy.map.Map(tempFile)
            temp = tempIn.data
            xdim = str(tempIn.dimensions[0])
            ydim = str(tempIn.dimensions[1])
            xdim = xdim.split('.')[0].strip()
            ydim = ydim.split('.')[0].strip()
            refPx = str(tempIn.reference_pixel[0])
            refPx = refPx.split(' ')[0].strip()
            refPy = str(tempIn.reference_pixel[1])
            refPy = refPy.split(' ')[0].strip()
            observatory = tempIn.observatory
            instrument = tempIn.instrument
            detector = tempIn.detector
            obstime = str(tempIn.date)
            obstime = obstime.replace('-','/')
            rc = str(tempIn.reference_coordinate).split('(lon, lat) in deg')[1].strip()
            rcx = rc.split(' ')[0].strip()
            rcx = rcx[1:-1]
            rcy = rc.split(' ')[1].strip()
            rcy = rcy[:-2]
            scaleX = str(tempIn.scale[0]).split(' ')[0].strip()
            scaleY = str(tempIn.scale[1]).split(' ')[0].strip()
            
            # Write specifications.txt
            print('Writing specifications.txt')
            specFile = os.path.join(outputDir,'specifications.txt')
            datFile = open(specFile,'w')
            datFile.write(regionNum+'\n')                            # Write the SHARP number
            datFile.write(xdim+'\n')                                 # Write no. x points
            datFile.write(ydim+'\n')                                 # Write no. y points
            datFile.write(str(nt)+'\n')                              # Write no. of time steps
            datFile.write(velSmooth+'\n')                            # Write velocity smooth. window 
            datFile.write(cutoff+'\n')                               # Write mag. field cutoff
            datFile.write(sampling)                                  # Write grip sampling parameter 
            datFile.close()

            # Write header.txt
            print('Writing header.txt')
            headFile = os.path.join(outputDir,'header.txt')
            datFile = open(headFile,'w')
            datFile.write(refPx+'\n')                                # Write reference pixel (x-direction)
            datFile.write(refPy+'\n')                                # Write reference pixel (y-direction)
            datFile.write(observatory+'\n')                          # Write observatory 
            datFile.write(instrument+'\n')                           # Write instrument
            datFile.write(detector+'\n')                             # Write detector
            datFile.write(obstime+'\n')                              # Write observation time
            datFile.write(rcx+'\n')                                  # Write reference x-coordinate (deg)
            datFile.write(rcy+'\n')                                  # Write reference y-coordinate (deg)
            datFile.write(scaleX+'\n')                               # Write scale (deg/pix) in x-direction
            datFile.write(scaleY)                                    # Write scale (deg/pix) in y-direction
            datFile.close()
            
            writeInitialData = False


        
            
# Remove the FITS files
print('Removing FITS files')
listDir = os.listdir(inputDir)
for item in listDir:
    if item.endswith('.fits'):
        os.remove(os.path.join(inputDir, item))

