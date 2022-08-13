#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt 
import numpy as np
import os
import datetime
import sys


def getpaths(filepath, activeregion):
    readdatfl=open(filepath+"/savedpaths.dat",'r')
    pth=False
    for p in readdatfl:
        path = p.split(':')
        AR =path[0]
        if (str(activeregion) == str(AR)):
            inputdir =path[1].split()[0][2:-2]
            outputdir =path[1].split()[1][2:-2]
            pth = True
            return '/'+str(outputdir)
            break
    if pth is False:
        sys.exit('The region is not found')
    

class mapping:
    def __init__(self,regionName, path_):
        self.path = path_
        self.regionName = regionName
       
        
    def get_specifications(self):
 #       self.path = self.path_# +'/AR_'+str(self.regionName)
        file = open(self.path+  'specifications.txt')
        r=[]
        for line in file:
            r.append(line.split()[0])
        V_smooth, cutoff, Sampling = r[0], r[1], r[2]
        r=[]
        return  V_smooth, cutoff, int(Sampling)   
        
    def sort(self,dic, key):
        numbered=dict()
        var_col=dic[key]
        for n in range(len(var_col)):
            numbered[n]= var_col[n]
    
        ss=dict(sorted(numbered.items(), key=lambda item: item[1]))
        
        sorted_dic=dict()
        lis = list(ss.keys())

        lis_=[]
        for i in  list(dic.keys()):
            lis_=[]
            for v in lis:
                lis_.append(dic[i][v]) 
            sorted_dic[i]=lis_
           
        return sorted_dic


    def getnxny(self):
        file = open(self.path+ '/regionData.dat')
        r=[]
        for line in file:
            r.append(line.split()[0])
        nx, ny = r[-2], r[-1]
        r=[]
        return int(nx), int(ny)


    def convert_array(self,nx,ny, M):
        Z = np.zeros((ny,nx))
        s=0
        for j in range(nx):    
            for i in range(ny):
                Z[i,j]= M[i+s]
            s=s+i+1
        return Z


    def read_data(self, Variable, filenumber):
        
        V_smooth, cutoff, Sampling  = self.get_specifications()

        self.filename = 'windDatPotentialFastCO'+str(cutoff)+'_VS'+str(V_smooth)+'_'+str(Sampling)+'_'+str(filenumber)+'.dat'

        Name = self.path + '/'+self.filename
        
        file=open(Name)
        check = file.readline()
        check = check.strip()
        file.seek(0)
        if check == 'N':
            variable = None
        else:
            variables={
                'X1':list(),
                'X2':list(),
                'bz':[],
                'vz': [],
                'sz':[],
                'windvalCur': [],
                'helvalCur': [],
                'windvalPot': [],
                'helvalPot': [],
                'windvalBraidOnly': [],
                'helvalBraidOnly': [],
                'wind': [],
                'hel': [],
                'deltaLflux': [],
                'deltaHflux': []
            }    
            for line in file:
                r=line.split()
                if len(r)!=1:
                    variables['X1'].append(float(r[0]))
                    variables['X2'].append(float(r[1]))
                    variables['bz'].append(float(r[2]))
                    variables['vz'].append(float(r[3]))
                    variables['sz'].append(float(r[4]))
                    variables['windvalCur'].append(float(r[5]))
                    variables['helvalCur'].append(float(r[6]))
                    variables['windvalPot'].append(float(r[7]))
                    variables['helvalPot'].append(float(r[8]))
                    variables['windvalBraidOnly'].append(float(r[9]))
                    variables['helvalBraidOnly'].append(float(r[10]))
                    variables['wind'].append(float(r[11]))
                    variables['hel'].append(float(r[12]))
                    variables['deltaLflux'].append(float(r[13]))
                    variables['deltaHflux'].append(float(r[14]))
                
                else:
                    break
            
            sorted_variable = self.sort(variables, 'X1')
            nx, ny = self.getnxny()
        
            if (nx%Sampling != 0):
                nx = int(nx/Sampling) + 1
            else:
                nx = int(nx/Sampling)
            if (ny%Sampling != 0):
                ny = int(ny/Sampling) + 1
            else:
                ny = int(ny/Sampling)
            
            variable = self.convert_array(nx,ny, sorted_variable[Variable])
        return variable        
   
         
    def plot_limit(self, Variable_):
        V=[]
        for i in np.array(Variable_).T:
            V.append(i)
        mx = np.max(V)
        mn = np.min(V)
    
        return (mn, mx) 
    
           
    def plotmap(self,Z, title,save=False, variable_name=None):
        if Z is None:
            print('Missing file')
        else:
            nx, ny = self.getnxny()
            km_factor = 360
            ny = np.linspace(0, ny*km_factor,len(Z))
            nx = np.linspace(0, nx*km_factor,len(Z[0]))
            X,Y = np.meshgrid(nx,ny)
        
            co_rang_st = self.plot_limit(Z)[0]
            co_rang_ed = self.plot_limit(Z)[1]
        
            v = np.linspace(co_rang_st, co_rang_ed, 60, endpoint=True)
        
            plt.rcParams.update({'font.size':16})
            plt.figure(figsize=(12, 10))
            plt.contourf(X,Y,Z,v,cmap='Greys_r')
            plt.ticklabel_format(axis="both", style="sci", scilimits=(0,0))
            plt.clim(co_rang_st , co_rang_ed)
        
            c = np.linspace(co_rang_st, co_rang_ed, 10, endpoint=True)
            plt.colorbar(ticks=c)
        
            plt.xlabel('X (km)')
            plt.ylabel('Y (km)')

            plt.title( title, loc='center', fontsize = 16)
            if save is True:
                self.image_name = str(self.path)[:-5] +'/generated_images/'+ variable_name+'_'+self.filename[len(self.filename)-17:len(self.filename)-4]+'.jpg'
                plt.savefig(self.image_name)
                plt.close()

            plt.show()


        
    def plot_gif(self, im_st_No, im_ed_No,variable_name, title):
        import glob
        from PIL import Image
        imgs=[]
        fp_out = str(self.path)[:-5] +'/generated_images/' + variable_name+'_'+str(self.regionName) +".gif"
        
        for im in range(im_st_No, im_ed_No+1):
            image_array = self.read_data(variable_name, im)
            if image_array is None:
                print('Skipping empty file')
            else:
                self.plotmap(image_array , title, save=True, variable_name=variable_name)
                imgs.append(Image.open(self.image_name))
                os.remove(self.image_name)
            
        imgs[0].save(fp_out, save_all=True, append_images=imgs[1:], duration=200, loop=0)




class timeseries:
    def __init__(self, regionName, path_):
        self.path = path_
        self.regionName = regionName

    def get_specifications(self):
 #       self.path = self.path_ #+'/AR_'+str(self.regionName)
        file = open(self.path+  'specifications.txt')
        r=[]
        for line in file:
            r.append(line.split()[0])
        V_smooth, cutoff, Sampling = r[0], r[1], r[2]
        self.st, self.et = r[4], r[6]
        r=[]
        return V_smooth, cutoff, Sampling

                
    def read_data(self, variable):
        V_smooth, cutoff, Sampling = self.get_specifications()

        self.Name_file = 'netWindDatPotFast'+str(cutoff)+'_VS'+str(V_smooth)+'_'+str(Sampling)+'.dat'
#        self.path = self.path_ #+'/AR_'+str(self.regionName)

        Name = self.path + self.Name_file

        file=open(Name)
        upscale_missing_values={'totWindCur':[],
                                'totHelCur':[],
                                'totWindPot':[],
                                'totHelPot':[],
                                'totWindBraid':[],
                                'totHelBraid':[],
                                'totWind':[],                              
                                'totHel':[],                               
                                'deltaLflux':[],
                                'deltaHflux':[]
                                }
        values=[]
        for line in file:
            r=line.split()        
            values.append(float(r[0]))
               
        for i in range(0,len(values),10):
            upscale_missing_values['totWindCur'].append(float(values[i]))
            upscale_missing_values['totHelCur'].append(float(values[i+1]))
            upscale_missing_values['totWindPot'].append(float(values[i+2]))
            upscale_missing_values['totHelPot'].append(float(values[i+3]))
            upscale_missing_values['totWindBraid'].append(float(values[i+4]))
            upscale_missing_values['totHelBraid'].append(float(values[i+5]))
            upscale_missing_values['totWind'].append(float(values[i+6]))
            upscale_missing_values['totHel'].append(float(values[i+7]))
            upscale_missing_values['deltaLflux'].append(float(values[i+8]))
            upscale_missing_values['deltaHflux'].append(float(values[i+9]))

       
            
        return np.array(upscale_missing_values[variable])


    def integrate_accum(self, Vrbs):
    
        stime= datetime.datetime(int(self.st[:4]),int(self.st[4:6]),int(self.st[6:8]),int(self.st[9:11]))
        etime= datetime.datetime(int(self.et[:4]),int(self.et[4:6]),int(self.et[6:8]),int(self.et[9:11]))

        tlength = (etime - stime).total_seconds()
        print('tlength = ', (tlength/3600))
        # accumulation trapzoid
        integrated=[]
        st_time = 0
        data=[]
        dt = 720
        data = list(self.read_data(Vrbs))
        for i in range(1,(len(data)+1)):    
            y_right = data[1:i]                          # right endpoints
            y_left = data[:i-1]                          # left endpoints
            integrated.append( (dt/2) * np.sum(y_right + y_left) )   
        
        t_range = np.linspace(st_time,((len(data)-1)*dt),len(integrated)) 
    
        return np.array(integrated), t_range
    
    def save_raw(self,time,var,name):
        dt = 720   
        if len(var) == len(time):
            data = np.column_stack([time, var])
            file_path = str(self.path)[:-5] + '/generated_images/' + str(self.regionName) + '_' + name  + '.txt'
            np.savetxt(file_path , data)
        else:
            print("The two variables are of different lengths. Perhaps you have input the wrong timespan?\n")

    def mean_std(self,Z, n_points, factor = 3):
        mean = []
        std = []
        com = 0
        for i in range(0,(len(Z)-n_points)):
            mean.append(np.mean(Z[ i: i + n_points]))
            std.append(np.std(Z[ i: i + n_points]))
            if (com == 0):
                for com_ in range(n_points//2 - 1):
                    mean.append(mean[0])
                    std.append(std[0])
                com=1
        for con in range(len(Z) - len(mean)):
            mean.append(mean[-1])
            std.append(std[-1])
    
        return np.array(mean), np.array(mean) + factor*np.array(std)
    
    def arrange(self,lst):
        labl = [l[2] for l in lst]
        colr = [l[1] for l in lst]
        time = [int(l[0]) for l in lst]
        uniq=[]
        for l in labl:
            if l not in uniq:
                uniq.append(l)
                
        df=dict()
        for v in uniq:
            indx=[]
            for l in range(len(labl)):
                if v == labl[l]:
                    indx.append(l)
            df[v]=[[time[l] for l in indx], colr[indx[0]]]
                              
        return df
        
    def plot(self,X,Y,colour,label, ylabel,Name,unit, oplot = None, save=False, **kwargs):   
        

        plt.rcParams.update({'font.size':16})

        plt.figure(figsize=(12, 10))
        plt.title( Name, loc='center', fontsize = 16)
        
        if unit == 'h':
            u=[3600,'hour'] 
        elif unit == 'm':
            u=[60,'minute'] 
        else:
            u=[1,'seconds'] 

        c=0
        for v in Y:
            plt.plot(X/u[0],v[:len(X)], colour[c], linewidth=2 ,label = label[c])
            c+=1
        
        if oplot is not None:
            data = self.arrange(oplot)
            for p in data.keys():
                t = data[p][0]           
                for l in range(len(t)):
                    if (l == (len(t)-1)):
                        plt.plot(((t[l]/u[0]),(t[l]/u[0])), (min(Y[0][:len(X)]),max(Y[0][:len(X)])), data[p][1] , label= p + ' x-ray' )
                    else:
                        plt.plot(((t[l]/u[0]),(t[l]/u[0])), (min(Y[0][:len(X)]),max(Y[0][:len(X)])), data[p][1] )
        plt.ylabel(ylabel)
        plt.xlabel('Time ('+ u[1] + ')')   
        plt.ylim([min(Y[0][:len(X)]), max(Y[0][:len(X)])])
        plt.xlim([(X[0]/u[0]), (X[-1]/u[0])])
#        plt.legend()
        plt.legend(loc=0, bbox_to_anchor=(1, 0.5), fontsize = 'x-small')

    
        if save is True:
            imagename = Name+'_CO'+self.Name_file[len(self.Name_file)-13:len(self.Name_file)-4]+'.jpg'
            plt.savefig(str(self.path)[:-5] +'/generated_images/'+imagename)

        plt.show()

