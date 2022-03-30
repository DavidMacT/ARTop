#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Feb 22 08:40:31 2022
@author: khd2
"""
import matplotlib.pyplot as plt
import numpy as np
import os
import sys

def sort(dic, key):
    numbered=dict()
    var_col=dic[key]
    for n in range(len(var_col)):
        numbered[n]= var_col[n]
    
    ss=dict(sorted(numbered.items(), key=lambda item: item[1]))
    
    sorted_dic=dict()
    lis = list(ss.keys())
 #   inserted=dic
    lis_=[]
    for i in  list(dic.keys()):
        lis_=[]
        for v in lis:
            lis_.append(dic[i][v]) 
        sorted_dic[i]=lis_
  #      
    return sorted_dic

def plot_limit(Variable):
    V=[]
    for i in np.array(Variable).T:
        V.append(i)
    mx = np.max(V)
    mn = np.min(V)

    return (mn, mx) 

def convert_array(M,nx,ny):
    Z = np.zeros((ny,nx))
    s=0
    for j in range(nx):    
        for i in range(ny):
            Z[i,j]= M[i+s]
        s=s+i+1
    return Z


def name_file(path,regionName, cutoff, VS, Sampling,filenumber):
    path = path +'/AR_'+str(regionName)
    filename= 'windDatPotentialFastCO'+str(cutoff)+'_VS'+str(VS)+'_'+str(Sampling)+'_'+str(filenumber)+'.dat'
    return path, filename

      
def plotmap(Z,nx,ny, variable_name, symbol,path, filename,save=True): 
    ny = np.linspace(0, ny,len(Z))
    nx = np.linspace(0, nx,len(Z[0]))
    X,Y = np.meshgrid(nx,ny)
    
    co_rang_st = plot_limit(Z)[0]
    co_rang_ed = plot_limit(Z)[1]
    
    v = np.linspace(co_rang_st, co_rang_ed, 60, endpoint=True)
    
    plt.rcParams.update({'font.size':16})
    plt.figure(figsize=(12, 10))
    plt.contourf(X,Y,Z,v,cmap='nipy_spectral')
    plt.clim(co_rang_st , co_rang_ed)
    
    c = np.linspace(co_rang_st, co_rang_ed, 10, endpoint=True)
    plt.colorbar(ticks=c)
    
    plt.xlabel('X pixel')
    plt.ylabel('Y pixel')

    plt.title( symbol, loc='center', fontsize = 16)
    #plt.show()
    if save is True:        
        plt.savefig(path +'/generated_images/'+ variable_name+'_'+filename[len(filename)-20:len(filename)-4]+'.jpg')

    
def getnxny(path):
    file = open(path+ '/output/regionData.dat')
    r=[]
    for line in file:
        r.append(line.split()[0])
    nx, ny = r[-2], r[-1]
    r=[]
    return int(nx), int(ny)


#def read_data(path, cutoff, VS, Sampling,filenumber):
def read_data(path, filename):

#    Name=path + '/output/windDatPotentialFastCO'+str(cutoff)+'_VS'+str(VS)+'_'+str(Sampling)+'_'+str(filenumber)+'.dat'
    Name= path + '/output/'+filename
    
    file=open(Name)

    variables={
            'X1':list(),
            'X2':list(),
            'bz':[],
            'vz': [],
            'windvalCur': [],
            'helvalCur': [],
            'windvalPot': [],
            'helvalPot': [],
            'windvalVelOnly': [],
            'helvalVelOnly': [],
            'wind': [],
            'hel': [],
            'deltaLflux': [],
            'deltaHflux': []
        }
    upscale_missing_values=dict()

    
    for line in file:
        r=line.split()
        if len(r)!=1:
            variables['X1'].append(float(r[0]))
            variables['X2'].append(float(r[1]))
            variables['bz'].append(float(r[2]))
            variables['vz'].append(float(r[3]))
            variables['windvalCur'].append(float(r[4]))
            variables['helvalCur'].append(float(r[5]))
            variables['windvalPot'].append(float(r[6]))
            variables['helvalPot'].append(float(r[7]))
            variables['windvalVelOnly'].append(float(r[8]))
            variables['helvalVelOnly'].append(float(r[9]))
            variables['wind'].append(float(r[10]))
            variables['hel'].append(float(r[11]))
            variables['deltaLflux'].append(float(r[12]))
            variables['deltaHflux'].append(float(r[13]))
            
        else:
            break

    return variables       
            




def plot_gif(images_path, imagename, variable_name,regionName):
    import glob
    from PIL import Image
    

#    path = "/home/khd2/Glasgow/Glasgow_combined/Data/AR_956/generated_images"
    fp_in = images_path +'/'+ imagename 
    fp_out = images_path + "/" + variable_name+'_'+regionName +".gif"
    
    # https://pillow.readthedocs.io/en/stable/handbook/image-file-formats.html#gif
    imgs = (Image.open(img) for img in sorted(glob.glob(fp_in)))
    img = next(imgs)  # extract first image from iterator
    img.save(fp=fp_out, format='GIF', append_images=imgs, save_all=True, duration=200, loop=0)

# path='/home/khd2/Glasgow/Data/output/' 
# insert=path+'windDatPotentialFastCO50_VS20_5_1063.dat' #sys.argv[3]
# file=open(insert)

# variables={
#         'X1':list(),
#         'X2':list(),
#         'bfield':[],
#         'vfield': [],
#         'windvalCur': [],
#         'helvalCur': [],
#         'windvalPot': [],
#         'helvalPot': [],
#         'windvalVelOnly': [],
#         'helvalVelOnly': [],
#         'wind': [],
#         'hel': [],
#         'deltaLflux': [],
#         'deltaHflux': []
#     }
# upscale_missing_values=dict()

# regionName='956'
# outputdir='/home/khd2/Downloads/Data/AR_956' #sys.argv[4]



# for line in file:
#     r=line.split()
#     if len(r)!=1:
#         variables['X1'].append(float(r[0]))
#         variables['X2'].append(float(r[1]))
# #            f.write(str(float(r[1]))+'\n')
# #            f.write()
# #        f.write('\n'+ str(float(r[0])))
#         variables['bfield'].append(float(r[2]))
#         variables['vfield'].append(float(r[3]))
#         variables['windvalCur'].append(float(r[4]))
#         variables['helvalCur'].append(float(r[5]))
#         variables['windvalPot'].append(float(r[6]))
#         variables['helvalPot'].append(float(r[7]))
#         variables['windvalVelOnly'].append(float(r[8]))
#         variables['helvalVelOnly'].append(float(r[9]))
#         variables['wind'].append(float(r[10]))
#         variables['hel'].append(float(r[11]))
#         variables['deltaLflux'].append(float(r[12]))
#         variables['deltaHflux'].append(float(r[13]))
        
#     else:
#         a_file = open(outputdir+"/totHelCur_"+regionName+'.txt','w')
#         a_file.write(r[0])
#         a_file.close()

# #        upscale_missing_values['totWindCur']= float(r[0])
#         line=file.read()
#         break


# r=line.split()

# #a_file = open(outputdir+"/totHelCur_"+regionName+'.txt','w')
# #a_file.write( str(sorted_variables['X2']))


# a_file = open(outputdir+"/totWindPot"+regionName+'.txt','w')
# a_file.write(r[-8])
# a_file.close()

# a_file = open(outputdir+"/totHelPot"+regionName+'.txt','w')
# a_file.write(r[-7])
# a_file.close()

# a_file = open(outputdir+"/totWindVel"+regionName+'.txt','w')
# a_file.write(r[-6])
# a_file.close()

# a_file = open(outputdir+"/totHelVel"+regionName+'.txt','w')
# a_file.write(r[-5])
# a_file.close()

# a_file = open(outputdir+"/totWind_Cur_Pot_Vel"+regionName+'.txt','w')
# a_file.write(r[-4])
# a_file.close()

# a_file = open(outputdir+"/totHel_Cur_Pot_Vel"+regionName+'.txt','w')
# a_file.write(r[-3])
# a_file.close()

# a_file = open(outputdir+"/deltaLflux"+regionName+'.txt','w')
# a_file.write(r[-2])
# a_file.close()

# a_file = open(outputdir+"/deltaHflux"+regionName+'.txt','w')
# a_file.write(r[-1])
# a_file.close()

# sorted_variables= sort(variables,'X1')
# with open(outputdir+"/totHelCur_"+regionName+'.txt','a') as f:
#     for line in sorted_variables['X1']:
#         f.write(str(line)+'\n')
# f.close()

# # upscale_missing_values['totHelCur']= float(r[-9])
# # upscale_missing_values['totWindPot']= float(r[-8])
# # upscale_missing_values['totHelPot']= float(r[-7])
# # upscale_missing_values['totWindVel']= float(r[-6])
# # upscale_missing_values['totHelVel']= float(r[-5])
# # upscale_missing_values['totWind_Cur_Pot_Vel']= float(r[-4])
# # upscale_missing_values['totHel_Cur_Pot_Vel']= float(r[-3])
# # upscale_missing_values['deltaLflux']= float(r[-2])
# # upscale_missing_values['deltaHflux']= float(r[-1])



# sorted_variables= sort(variables,'X1')


# nx= 138 #int(sys.argv[1])#138
# ny= 90 #int(sys.argv[2])#90
    
# X= convert_array(sorted_variables['X1'],nx,ny)
# Y= convert_array(sorted_variables['X2'],nx,ny)
# bz= convert_array(sorted_variables['bfield'],nx,ny)


# windvalCur= convert_array(sorted_variables['windvalCur'],nx,ny)# np.zeros((ny,nx))
# windvalPot= convert_array(sorted_variables['windvalPot'],nx,ny)
# windvalVelOnly= convert_array(sorted_variables['windvalVelOnly'],nx,ny)


# total_winding = windvalCur + windvalPot - windvalVelOnly



# plot(X,Y,bz,'bz','Bz',outputdir)
# plot(X,Y,total_winding,'Total_winding',r'$L_w$',outputdir)

