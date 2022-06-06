#!/usr/bin/env python
# coding: utf-8

import os
import sys

def getnxny(inputdir,outputdir,SHARP):
    input = open(inputdir+'/'+'bx_'+str(SHARP)+'_0.txt','r')
    xlength = input.readline()
    nx = len(xlength.split())

    counter = 1 # we have already read one line in
    content = input.read()
    CoList = content.split("\n")

    for i in CoList:
        if i:
           counter += 1

    ny = counter
    input.close()
    	
    return nx,ny

inputdir = sys.argv[1]
outputdir = sys.argv[2]
SHARPnum = sys.argv[3]
start_y = sys.argv[4]
start_m = sys.argv[5]
start_d = sys.argv[6]
start_h = sys.argv[7]

nx,ny = getnxny(inputdir,outputdir,SHARPnum)
print(nx)
print(ny)
datfl=open(outputdir+"/regionData.dat",'w')
datfl.write(SHARPnum+"\n")
datfl.write(start_y+"\n")
datfl.write(start_m+"\n")
datfl.write(start_d+"\n")
datfl.write(start_h+"\n")
datfl.write(str(nx)+"\n")
datfl.write(str(ny))
datfl.close()
