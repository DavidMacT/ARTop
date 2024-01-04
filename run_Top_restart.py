#!/usr/bin/env python
# coding: utf-8

import subprocess
import os


'''
run_Top_restart.py

If your cluster kills your calculations while you are performing the topology calculations, you can restart from the last time dump using this scrip. You need to specify the starting time dump.

'''


# Restart the topological calculations
starting_dump = 9
sd = str(starting_dump)
gap = ' '
cwd = os.getcwd()
pathcpp = os.path.join(cwd,'source','python')
pathTop = os.path.join(pathcpp,'Top_restart.py')
cmd = 'python'+gap+pathTop+gap+sd
subprocess.call(cmd,shell=True)
