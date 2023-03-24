#!/usr/bin/env python
# coding: utf-8

import subprocess
import os


'''
run_ARTop.py

Run this script to call other python and C++ scripts that combine to produce
the output files of ARTop. 

REMEMBER: before running this script, complete 'read_data.txt'.

'''

# Run the initial setup (download files, get velocities, get potential fields)
gap = ' '
cwd = os.getcwd()
pathpy = os.path.join(cwd,'source','python')
pathInitial = os.path.join(pathpy,'InitialSetup.py')
cmd = 'python'+gap+pathInitial
subprocess.call(cmd,shell=True)


# Run the topological calculations
gap = ' '
cwd = os.getcwd()
pathcpp = os.path.join(cwd,'source','python')
pathTop = os.path.join(pathcpp,'Topology.py')
cmd = 'python'+gap+pathTop
subprocess.call(cmd,shell=True)
