#!/usr/bin/env python
# coding: utf-8

from __future__ import division, print_function
import os
import sys
from typing_extensions import Literal
import datetime


def number_list(stime, etime, output_dir):
    expct_filesNo = (etime - stime).total_seconds()/720
    print(output_dir)
    add = open(output_dir+'/specifications.txt', 'a')
    add.write(str(int(expct_filesNo))+'\n')
    add.close()
    return expct_filesNo



sYear = sys.argv[1]
sMonth = sys.argv[2]
sDay = sys.argv[3]
sHour = sys.argv[4]

eYear = sys.argv[5]
eMonth = sys.argv[6]
eDay = sys.argv[7]
eHour = sys.argv[8]

outputdir = sys.argv[9]


start_time = datetime.datetime(int(sYear),int(sMonth),int(sDay),int(sHour),int(00),int(00))
end_time = datetime.datetime(int(eYear),int(eMonth),int(eDay),int(eHour),int(00),int(00))

nof = number_list(start_time, end_time, outputdir)

print('number of files = ', int(nof))

print('time span = ', end_time - start_time)
