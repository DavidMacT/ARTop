#!/usr/bin/env python
# coding: utf-8

# In[5]:


from __future__ import division, print_function
import os.path
from typing_extensions import Literal

import matplotlib.pyplot as plt
import sunpy.map
import sunpy.data.sample
import drms,sys
import numpy as np
import json, urllib, numpy as np, matplotlib.pylab as plt, math, requests
from astropy.io import fits
#rom sunpy.visualization.colormaps import color_tables as ct
import datetime
#import matplotlib.image as mpimg#
#get_ipython().run_line_magic('matplotlib', 'inline')
#get_ipython().run_line_magic('config', "InlineBackend.figure_format='retina'")
client = drms.Client()  


# In[6]:




c = drms.Client(email='brenorfs@gmail.com', verbose=True)


# In[8]:


def get_the_data(data):
    #print("Approximating",data['segments'][0]['name'],"as Bx.")
    #print("Approximating",data['segments'][1]['name'],"as Bz.")
    #print("Approximating",data['segments'][2]['name'],"as -By.")
    bxin = fits.open("http://jsoc.stanford.edu"+data['segments'][0]['values'][0])
    bzin = fits.open("http://jsoc.stanford.edu"+data['segments'][1]['values'][0])
    byin = fits.open("http://jsoc.stanford.edu"+data['segments'][2]['values'][0])
    byin[1].data = -1.0*(np.array(byin[1].data)) # flip the sign of by
    bx = bxin[1].data
    by = byin[1].data
    bz = bzin[1].data
    IMCRVAL1 = float(data['keywords'][0]['values'][0])
    NOAA_ARS = str(data['keywords'][1]['values'][0])
    #T_REC = str(data['keywords'][2]['values'][0])
    xsize = float(data['segments'][0]['dims'][0].rsplit('x', 1)[0])
    ysize = float(data['segments'][0]['dims'][0].rsplit('x', 1)[1])
    #print((2*np.pi*6.955e8*bz[1].meta['CDELT2']/360)/1000)
    print(xsize,ysize);
    return bx, by, bz, IMCRVAL1, NOAA_ARS, xsize, ysize

def removeNANs(field):
    size1=np.size(field,0)
    for i in range(0,size1):
        size2 =field[i].size-1
        for j in range(0,size2):
            if math.isnan(field[i][j]):
                field[i][j]=0.0         



                
regionNumber = sys.argv[1]
st_y = sys.argv[2]
M = int(sys.argv[3])
d = int(sys.argv[4])
H = int(sys.argv[5])

start_time = datetime.datetime(int(st_y),int(M),int(d),int(H),int(00),int(00))
end_time = datetime.datetime(int(sys.argv[6]),int(sys.argv[7]),int(sys.argv[8]),int(sys.argv[9]),int(00),int(00))


print('Time span = ', end_time - start_time)

print("downloading.."+'\n')

inputdir= sys.argv[10]
outputdir= sys.argv[11]
writeInitialData=True
#datfl=open(outputdir+"/AR_"+regionNumber+"/regionData.dat",'w')
datfl=open(outputdir+"/regionData.dat",'w')

'''
each 1 hour has 5 readings with time resolution of 12 minutes.
'''

i=0
while(start_time <end_time):
    for j in range(5):      # 5 instances in each 1 hour.
        if (5*i+j)>-1 : 
            url='http://jsoc.stanford.edu/cgi-bin/ajax/jsoc_info?ds=hmi.sharp_cea_720s['+regionNumber+']['+st_y+'.'+str(M)+'.'+str(d)+'_'+str(H)+':'+str("{:02d}".format(12*j))+':00_TAI]&op=rs_list&key=IMCRVAL1,NOAA_ARS&seg=Bp,Br,Bt'
            response = requests.get(url)
            data = response.json()
            print(H,d,M,12*j,5*i+j)
            if data['count']==1: 
                bx, by, bz, IMCRVAL1, NOAA_ARS, xsize, ysize = get_the_data(data)
                removeNANs(bx)
                removeNANs(by)
                removeNANs(bz)
                if writeInitialData==True:
                    datfl.write(sys.argv[1]+"\n")  
                    datfl.write(sys.argv[2]+"\n")  
                    datfl.write(sys.argv[3]+"\n")  
                    datfl.write(sys.argv[4]+"\n")  
                    datfl.write(sys.argv[5]+"\n")  
                    datfl.write(str(int(xsize))+"\n")                    
                    datfl.write(str(int(ysize)))
                    datfl.close()
                    writeInitialData=False

             

                a_file = (inputdir+"/bx_"+regionNumber+"_{:d}.txt".format(5*i+j))
                np.savetxt(a_file, bx[:,:])
                a_file = (inputdir+"/by_"+regionNumber+"_{:d}.txt".format(5*i+j))
                np.savetxt(a_file, by[:,:])
                a_file = (inputdir+"/bz_"+regionNumber+"_{:d}.txt".format(5*i+j))
                np.savetxt(a_file, bz[:,:])

    i+=1
    if H<23:
        H=H+1
    else:
        d=d+1
        H=0
    if M % 2==0:
        #check if it's February
        if M==2:
            if d==29:
                M=M+1
                d=d+1
        else:
            if d==31:
                M=M+1
                d=1
    else:
        if d==32:
            M=M+1
            d=1
    start_time = datetime.datetime(int(st_y),M,d,H,int(00),int(00))
    print('Hour mark: ',start_time)
    

    
# numbering the full downloaded files
#====================================

strtime = sys.argv[12]
endtime = sys.argv[13]
regionName = sys.argv[1]
Vsample, cutoff, sample = int(sys.argv[14]), int(sys.argv[15]), int(sys.argv[16])
ptint(Vsample,cutoff,sample)
input_dir = sys.argv[10]
output_dir =  sys.argv[11]
p

def numbering(strtime, endtime, regionName,Vsample, cutoff, sample,input_dir, output_dir):
    stime= datetime.datetime(int(str(strtime)[:4]),int(str(strtime)[4:6]),int(str(strtime)[6:8]),int(str(strtime)[8:]))
    etime= datetime.datetime(int(str(endtime)[:4]),int(str(endtime)[4:6]),int(str(endtime)[6:8]),int(str(endtime)[8:]))
    t_range = ((etime - stime).total_seconds()/3600)*5
    print(int(t_range))
    files=os.listdir(input_dir)
    numbering_files=[]
    for f in range(int(t_range)):
        bx = 'bx_'+str(regionName)+'_'+str(f)+'.txt'
        by = 'by_'+str(regionName)+'_'+str(f)+'.txt'
        bz = 'bz_'+str(regionName)+'_'+str(f)+'.txt'
        if ((bx in files) and (by in files) and (bz in files)):
            numbering_files.append(f)
        else:
            numbering_files.append(0)
    filename = output_dir+'/numbering'+str(cutoff)+'_VS'+str(Vsample)+'_'+str(sample)+'.dat'
    np.savetxt(filename, numbering_files)

numbering(strtime, endtime, regionName,Vsample, cutoff, sample,input_dir, output_dir)
