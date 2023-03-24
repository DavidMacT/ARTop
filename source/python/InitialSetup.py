#!/usr/bin/env python
# coding: utf-8

import subprocess
import os
import glob
import sys

'''
InitialSetup.py

This script downloads and prepares files for the C++ code
that calculates the topological quantities.

The inputs to this script are the variables from read_data.txt.

hmi.sharp_cea_720s FITS files are downloaded and processed by 
calling MagDown.py.

Velocity maps are calculated by calling DAVE4vm.py

Potential field components are calculated by calling potentialbxby.py

The magnetic field and velocity component files are stored in the Data
folder of the output directory, ready to be read by the C++ code.  

'''

def afterEquals(s,part=1):
    '''
    Keeps text after an equals sign

    Input
    -----
    s   :   string to be cut
    part:   0: keep left of '=', 1: keep right of '=' (default)

    Output:
    -------
    out:    edited string

    '''
    
    out = s.split('=')[part].strip()
    return out


def readData():
    '''
    Read the read_data.txt file in the main ARTop directory.

    Output:
    ------
    variables:  array with all the variables from read_data.txt

    '''
    with open('read_data.txt','r') as f:
        contents = f.readlines()

    variables = []
    n = 18 # number of variables in read_data.txt
    for i in range(n):
        temp = afterEquals(contents[i])
        variables.append(temp)
    return variables


def readSpec(outputDir):
    '''
    Read specifications.txt file in the output directory.


    Input:
    ------
    outputDir:  directory containing specifications.txt

    Output:
    -------
    details  :  array with all the parameters stored in specifications.txt

    '''

    with open(outputDir,'r') as f:
        contents = f.readlines()

    variables = []
    n = 7
    for i in range(n):
        variables.append(contents[i].strip())
    return variables


# Read read_data.txt
varAR = readData()

# Variables
regionNum = varAR[0]
downloadData = varAR[1]
startYear = varAR[2]
startMonth = varAR[3]
startDay = varAR[4]
startHour = varAR[5]
endYear = varAR[6]
endMonth = varAR[7]
endDay = varAR[8]
endHour = varAR[9]
velSmooth = varAR[10]
inputDir = varAR[11]
outputDir = varAR[12]
topology = varAR[13]
cutoff = varAR[14]
sampling = varAR[15]
removeImages = varAR[16]
regEmail = varAR[17]


# Check input/output directories exist and, if not, create them

if os.path.exists(inputDir) == False:
    os.mkdir(inputDir)

if os.path.exists(outputDir) == False:
    os.mkdir(outputDir)
    os.mkdir(os.path.join(outputDir,'Data'))    

 
# Check if download needed

gap = ' '
if( downloadData == 'true' or downloadData == 'True'):
    # Download the magnetograms
    argMD = regionNum+gap+startYear+gap+startMonth+gap+startDay+gap+startHour+gap+endYear+gap+endMonth+gap+endDay+gap+endHour+gap+inputDir+gap+outputDir+gap+regEmail+gap+downloadData+gap+velSmooth+gap+cutoff+gap+sampling
    cwd = os.getcwd()
    pathMD = os.path.join(cwd,'source','python','MagDown.py')
    cmd = 'python'+gap+pathMD+gap+argMD
    subprocess.call(cmd,shell=True)


# Run DAVE4VM code for velocity maps

# Get details from specifications.txt
specLoc = os.path.join(outputDir,'specifications.txt')
specDetails = readSpec(specLoc)
nx = specDetails[1]
ny = specDetails[2]
nt = specDetails[3]
 
# Call the DAVE4VM script
outData = os.path.join(outputDir,'Data')
ntt = str(int(nt)-1)
argDAVE = inputDir+gap+regionNum+gap+nx+gap+ny+gap+'0'+gap+ntt+gap+velSmooth
cwd = os.getcwd()
pathDAVE = os.path.join('.',cwd,'source','cpp','getVelocity')
cmd = pathDAVE+gap+argDAVE
subprocess.call(cmd,shell=True)

# Check if remove files
if (removeImages == 'true' or removeImages == 'True'):
    allFiles = os.path.join(inputDir,'*')
    fileList = glob.glob(allFiles)
    for f in fileList:
        print('Deleting :'+f)
        os.remove(f)

# Call potential field solver
numPot = str(nt)
argPot = regionNum+gap+'0'+gap+numPot+gap+nx+gap+ny+gap+inputDir
pathPot = os.path.join(cwd,'source','python','potentialbxby.py')
cmd = 'python'+gap+pathPot+gap+argPot
subprocess.call(cmd,shell=True)

# End of initial setup