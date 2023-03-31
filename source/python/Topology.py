#!/usr/bin/env python
# coding: utf-8

import subprocess
import os
import glob
import sys

'''
Topology.py

This script reads in necessary input data and runs a C++ script that 
calculates the topological quantities. 

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

# Check if topology calculations are required
immediateExit = False
if (topology == 'false' or topology == 'False'):
    immediateExit = True
    sys.exit('No topology calculations performed.')

# Perform topology calculations
gap = ' '
specLoc = os.path.join(outputDir,'specifications.txt')
specDetails = readSpec(specLoc)
nx = specDetails[1]
ny = specDetails[2]
nt = specDetails[3]
ntt = str(int(nt)-1)
pathData = os.path.join(outputDir,'Data')
outFile = os.path.join(pathData,'windDatPotentialFast')
dx = str(360)
dy = str(360)
argTop = inputDir+gap+regionNum+gap+velSmooth+gap+'0'+gap+ntt+gap+nx+gap+ny+gap+dx+gap+dy+gap+cutoff+gap+sampling+gap+outFile
cwd = os.getcwd()
pathTop = os.path.join('.',cwd,'source','cpp','observationalWindingPotentialFast')
cmd = pathTop+gap+argTop
subprocess.call(cmd,shell=True)

# Produce file for time series outputs
seriesFileName = 'netWindDatPotFast'+cutoff+'_VS'+velSmooth+'_'+sampling+'.dat'
netPath = os.path.join(pathData,seriesFileName)
f1 = open(netPath, 'a')
for i in range(int(nt)):
    fname = 'windDatPotentialFastCO'+cutoff+'_VS'+velSmooth+'_'+sampling+'_'+str(i)+'.dat'
    fpath = os.path.join(pathData,fname)
    dfile = open(fpath,'r')
    lines = dfile.readlines()
    last10 = lines[-10:]
    for i in range(10):
        f1.write(last10[i])
    dfile.close()
f1.close()

# End of topology calculations

# Check if files from the input directory are to be removed
if removeImages.casefold() == 'true':
    allFiles = os.path.join(inputDir,'*')
    fileList = glob.glob(allFiles)
    for f in fileList:
        os.remove(f)
