#!/usr/bin/env python
# coding: utf-8

# In[1]:


import numpy as np
import sunpy
import sunpy.map
import sys
from astropy.convolution import convolve
import os.path
import datetime
import gc
import copy

#using scipy.
from scipy.linalg import lstsq


# In[2]:


def odiff(image):
    '''
    Image refers to the image of the magnetic field in x,y,z or, in this case, to
    the average of the initial and stop values of them.
    '''
    
    #defining the constants
    c1 = 0.12019
    c2 = 0.74038

    a1 = np.roll(image[:], -2, axis = 1) #two pxs to the left
    a2 = np.roll(image[:], -1, axis = 1) #one px to the left
    a3 = np.roll(image[:], 1, axis = 1) #one px to the right
    a4 = np.roll(image[:], 2, axis = 1) #two pxs to the right

    b1 = np.roll(image[:], -2, axis = 0) #two pxs downward
    b2 = np.roll(image[:], -1, axis = 0) #one px downward
    b3 = np.roll(image[:], 1, axis = 0) #one px upward
    b4 = np.roll(image[:], 2, axis = 0) #two pxs upward
    
    #Calculating the differentials dx and dy using the shift_pxs function results
    dx = (np.multiply(-c1,a1) + np.multiply(c2,a2) + np.multiply(-c2,a3) +
          np.multiply(c1,a4))
    dy =(np.multiply(-c1,b1) + np.multiply(c2,b2) + np.multiply(-c2,b3) +
          np.multiply(c1,b4))
    
    return(dx,dy)


# In[3]:


def the_matrix(bx, bxx, bxy, by, byx, byy, bz, bzx, bzy,
               bzt, psf, psfx, psfy, psfxx, psfyy, psfxy):
    '''
    This function will be used to perform the convolutions and construct
    the matrix from which the solutions will be calculated.
    '''
    
    #Constructing the matrix for the LKA algorithm
    #Later I should take those unecessary terms to hold the multiplication away
    GG = np.multiply(bz,bz)
    G = convolve(GG,psf, normalize_kernel = False, boundary = 'fill') #1

    GGx = np.multiply(bz,bzx)
    Gx = convolve(GGx,psf, normalize_kernel = False, boundary = 'fill') #2
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/Gx.csv', Gx, delimiter = ',')
    
    xGx = np.multiply(-1, convolve(GGx,psfx, normalize_kernel = False, boundary = 'fill')) #3
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xGx.csv', xGx, delimiter = ',')
    
    yGx = np.multiply(-1, convolve(GGx,psfy, normalize_kernel = False, boundary = 'fill')) #4
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yGx.csv', yGx, delimiter = ',')
    
    GGy = np.multiply(bz,bzy)
    Gy = convolve(GGy,psf, normalize_kernel = False, boundary = 'fill') #5
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/Gy.csv', Gy, delimiter = ',')
    
    xGy = np.multiply(-1, convolve(GGy,psfx, normalize_kernel = False, boundary = 'fill')) #6
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xGy.csv', xGy, delimiter = ',')
    
    yGy = np.multiply(-1, convolve(GGy,psfy, normalize_kernel = False, boundary = 'fill')) #7
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yGy.csv', yGy, delimiter = ',')
    
    GGt = np.multiply(bzt,bz)
    Ht = convolve(GGt,psf, normalize_kernel = False, boundary = 'fill') #8
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/Ht.csv', Ht, delimiter = ',')
    
    GGxx = np.multiply(bzx,bzx)
    Gxx = convolve(GGxx,psf, normalize_kernel = False, boundary = 'fill') #9
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/Gxx.csv', Gxx, delimiter = ',')
    
    GGyy = np.multiply(bzy,bzy)
    Gyy = convolve(GGyy,psf, normalize_kernel = False, boundary = 'fill') #10
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/Gyy.csv', Gyy, delimiter = ',')
    
    GGxy = np.multiply(bzx,bzy)
    Gxy = convolve(GGxy,psf, normalize_kernel = False, boundary = 'fill') #11
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/Gxy.csv', Gxy, delimiter = ',')
    
    GGtx = np.multiply(bzt,bzx)
    Gtx = convolve(GGtx,psf, normalize_kernel = False, boundary = 'fill') #12
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/Gtx.csv', Gtx, delimiter = ',')
    
    GGty = np.multiply(bzt,bzy) 
    Gty = convolve(GGty, psf, normalize_kernel = False, boundary = 'fill') #13
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/Gty.csv', Gty, delimiter = ',')
    
    ##########################
    
    ##########################
    xGxx = np.multiply(-1, convolve(GGxx,psfx, normalize_kernel = False, boundary = 'fill')) #14
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xGxx.csv', xGxx, delimiter = ',')
    
    xGyy = np.multiply(-1, convolve(GGyy,psfx, normalize_kernel = False, boundary = 'fill')) #15
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xGyy.csv', xGyy, delimiter = ',')
    
    xGxy = np.multiply(-1, convolve(GGxy,psfx, normalize_kernel = False, boundary = 'fill')) #16
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xGxy.csv', xGxy, delimiter = ',')
    
    xGtx = np.multiply(-1, convolve(GGtx,psfx, normalize_kernel = False, boundary = 'fill')) #17
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xGtx.csv', xGtx, delimiter = ',')
    
    xGty = np.multiply(-1, convolve(GGty,psfx, normalize_kernel = False, boundary = 'fill')) #18
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xGty.csv', xGty, delimiter = ',')
    
    ##########################
    
    ##########################
    yGxx = np.multiply(-1, convolve(GGxx,psfy, normalize_kernel = False, boundary = 'fill')) #19
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yGxx.csv', yGxx, delimiter = ',')
    
    yGyy = np.multiply(-1, convolve(GGyy,psfy, normalize_kernel = False, boundary = 'fill')) #20
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yGyy.csv', yGyy, delimiter = ',')
    
    yGxy = np.multiply(-1, convolve(GGxy,psfy, normalize_kernel = False, boundary = 'fill')) #21
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yGxy.csv', yGxy, delimiter = ',')
    
    yGtx = np.multiply(-1, convolve(GGtx,psfy, normalize_kernel = False, boundary = 'fill')) #22
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yGtx.csv', yGtx, delimiter = ',')
    
    yGty = np.multiply(-1, convolve(GGty,psfy, normalize_kernel = False, boundary = 'fill')) #23
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yGty.csv', yGty, delimiter = ',')
    
    ##########################
    
    ##########################
    xxGxx = convolve(GGxx,psfxx, normalize_kernel = False, boundary = 'fill') #24
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xxGxx.csv', xxGxx, delimiter = ',')
    
    xxGxy = convolve(GGxy,psfxx, normalize_kernel = False, boundary = 'fill') #25
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xxGxy.csv', xxGxy, delimiter = ',')
    
    xxGyy = convolve(GGyy,psfxx, normalize_kernel = False, boundary = 'fill') #26  
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xxGyy.csv', xxGyy, delimiter = ',')
    
    ##########################
    
    ##########################
    xyGxx = convolve(GGxx,psfxy, normalize_kernel = False, boundary = 'fill') #27
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xyGxx.csv', xyGxx, delimiter = ',')
    
    xyGxy = convolve(GGxy,psfxy, normalize_kernel = False, boundary = 'fill') #28
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xyGxy.csv', xyGxy, delimiter = ',')
    
    xyGyy = convolve(GGyy,psfxy, normalize_kernel = False, boundary = 'fill') #29
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xyGyy.csv', xyGyy, delimiter = ',')
    
    ##########################
    
    ##########################
    yyGxx = convolve(GGxx,psfyy, normalize_kernel = False, boundary = 'fill') #30
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yyGxx.csv', yyGxx, delimiter = ',')
    
    yyGxy = convolve(GGxy,psfyy, normalize_kernel = False, boundary = 'fill') #31
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yyGxy.csv', yyGxy, delimiter = ',')
    
    yyGyy = convolve(GGyy,psfyy, normalize_kernel = False, boundary = 'fill') #32
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yyGyy.csv', yyGyy, delimiter = ',')
    
    ##########################
    
    GGtt = np.multiply(bzt,bzt)
    Gtt = convolve(GGtt,psf)  #33
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/Gtt.csv', Gtt, delimiter = ',')
    
    ##########################
    #End of the original dave matrices
    #For now on there are represented the extra vector magnetogram terms
    ##########################
    
    BxBx = convolve(np.multiply(bx,bx),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BxBx.csv', BxBx, delimiter = ',')
    
    ByBy = convolve(np.multiply(by,by),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/ByBy.csv', ByBy, delimiter = ',')
    
    BxBy = convolve(np.multiply(bx,by),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BxBy.csv', BxBy, delimiter = ',')
    
    BzBx = convolve(np.multiply(bz,bx),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BzBx.csv', BzBx, delimiter = ',')
    
    BzBy = convolve(np.multiply(bz,by),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BzBy.csv', BzBy, delimiter = ',')
    
    BxBxx = convolve(np.multiply(bx,bxx),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BxBxx.csv', BxBxx, delimiter = ',')
    
    BxByy = convolve(np.multiply(bx,byy),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BxByy.csv', BxByy, delimiter = ',')
    
    BxxBxx = convolve(np.multiply(bxx,bxx),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BxxBxx.csv', BxxBxx, delimiter = ',')
    
    ByyByy = convolve(np.multiply(byy,byy),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/ByyByy.csv', ByyByy, delimiter = ',')
    
    BxxByy = convolve(np.multiply(bxx,byy),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BxxByy.csv', BxxByy, delimiter = ',')
    
    ByBxx = convolve(np.multiply(by,bxx),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/ByBxx.csv', ByBxx, delimiter = ',')
    
    ByByy = convolve(np.multiply(by,byy),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/ByByy.csv', ByByy, delimiter = ',')
    
    BzBxx = convolve(np.multiply(bz,bxx),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BzBxx.csv', BzBxx, delimiter = ',')
    
    BzByy = convolve(np.multiply(bz,byy),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BzByy.csv', BzByy, delimiter = ',')
    
    BztBxx = convolve(np.multiply(bzt,bxx),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BztBxx.csv', BztBxx, delimiter = ',')
    
    BztByy = convolve(np.multiply(bzt,byy),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BztByy.csv', BztByy, delimiter = ',')
    
    BzxBx = convolve(np.multiply(bzx,bx),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BzxBx.csv', BzxBx, delimiter = ',')
    
    BzxBy = convolve(np.multiply(bzx,by),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BzxBy.csv', BzxBy, delimiter = ',')
    
    BzxBxx = convolve(np.multiply(bzx,bxx),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BzxBxx.csv', BzxBxx, delimiter = ',')
    
    BzxByy = convolve(np.multiply(bzx,byy),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BzxByy.csv', BzxByy, delimiter = ',')
    
    BzyBx = convolve(np.multiply(bzy,bx),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BzyBx.csv', BzyBx, delimiter = ',')
    
    BzyBy = convolve(np.multiply(bzy,by),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BzyBy.csv', BzyBy, delimiter = ',')
    
    BzyBxx = convolve(np.multiply(bzy,bxx),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BzyBxx.csv', BzyBxx, delimiter = ',')
    
    BzyByy = convolve(np.multiply(bzy,byy),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BzyByy.csv', BzyByy, delimiter = ',')
    
    BztBx = convolve(np.multiply(bzt,bx),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BztBx.csv', BztBx, delimiter = ',')
    
    BztBy = convolve(np.multiply(bzt,by),psf, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/BztBy.csv', BztBy, delimiter = ',')
    
    ##########################
    #End of the psf dependent convolutions
    ##########################
    
    
    xBzxBx = np.multiply(-1, convolve(np.multiply(bzx,bx),psfx, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xBzxBx.csv', xBzxBx, delimiter = ',')
    
    xBzxBy = np.multiply(-1, convolve(np.multiply(bzx,by),psfx, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xBzxBy.csv', xBzxBy, delimiter = ',')
    
    xBzyBx = np.multiply(-1, convolve(np.multiply(bzy,bx),psfx, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xBzyBx.csv', xBzyBx, delimiter = ',')
    
    xBzyBy = np.multiply(-1, convolve(np.multiply(bzy,by),psfx, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xBzyBy.csv', xBzyBy, delimiter = ',')
    
    yBzyBx = np.multiply(-1, convolve(np.multiply(bzy,bx),psfy, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yBzyBx.csv', yBzyBx, delimiter = ',')
    
    yBzyBy = np.multiply(-1, convolve(np.multiply(bzy,by),psfy, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yBzyBy.csv', yBzyBy, delimiter = ',')
    
    yBzxBx = np.multiply(-1, convolve(np.multiply(bzx,bx),psfy, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yBzxBx.csv', yBzxBx, delimiter = ',')
    
    yBzxBy = np.multiply(-1, convolve(np.multiply(bzx,by),psfy, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yBzxBy.csv', yBzxBy, delimiter = ',')
    
    yBxBxx = np.multiply(-1, convolve(np.multiply(bx,bxx),psfy, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yBxBxx.csv', yBxBxx, delimiter = ',')
    
    yBxByy = np.multiply(-1, convolve(np.multiply(bx,byy),psfy, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yBxByy.csv', yBxByy, delimiter = ',')
    
    yByBxx = np.multiply(-1, convolve(np.multiply(by,bxx),psfy, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yByBxx.csv', yByBxx, delimiter = ',')
    
    yByByy = np.multiply(-1, convolve(np.multiply(by,byy),psfy, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yByByy.csv', yByByy, delimiter = ',')
    
    xByBxx = np.multiply(-1, convolve(np.multiply(by,bxx),psfx, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xByBxx.csv', xByBxx, delimiter = ',')
    
    xByByy = np.multiply(-1, convolve(np.multiply(by,byy),psfx, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xByByy.csv', xByByy, delimiter = ',')
    
    xBzxBxx = np.multiply(-1, convolve(np.multiply(bzx,bxx),psfx, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xBzxBxx.csv', xBzxBxx, delimiter = ',')
    
    xBzxByy = np.multiply(-1, convolve(np.multiply(bzx,byy),psfx, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xBzxByy.csv', xBzxByy, delimiter = ',')
    
    yBzxBxx = np.multiply(-1, convolve(np.multiply(bzx,bxx),psfy, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yBzxBxx.csv', yBzxBxx, delimiter = ',')
    
    yBzxByy = np.multiply(-1, convolve(np.multiply(bzx,byy),psfy, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yBzxByy.csv', yBzxByy, delimiter = ',')
    
    xBxxBxx = np.multiply(-1, convolve(np.multiply(bxx,bxx),psfx, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xBxxBxx.csv', xBxxBxx, delimiter = ',')
    
    xBxxByy = np.multiply(-1, convolve(np.multiply(bxx,byy),psfx, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xBxxByy.csv', xBxxByy, delimiter = ',')
    
    xByyByy = np.multiply(-1, convolve(np.multiply(byy,byy),psfx, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xByyByy.csv', xByyByy, delimiter = ',')
    
    yBxxBxx = np.multiply(-1, convolve(np.multiply(bxx,bxx),psfy, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yBxxBxx.csv', yBxxBxx, delimiter = ',')
    
    yBxxByy = np.multiply(-1, convolve(np.multiply(bxx,byy),psfy, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yBxxByy.csv', yBxxByy, delimiter = ',')
    
    yByyByy = np.multiply(-1, convolve(np.multiply(byy,byy),psfy, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yByyByy.csv', yByyByy, delimiter = ',')
    
    xBxBxx = np.multiply(-1, convolve(np.multiply(bx,bxx),psfx, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xBxBxx.csv', xBxBxx, delimiter = ',')
    
    xBxByy = np.multiply(-1, convolve(np.multiply(bx,byy),psfx, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xBxByy.csv', xBxByy, delimiter = ',')
    
    xBzBxx = np.multiply(-1, convolve(np.multiply(bz,bxx),psfx, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xBzBxx.csv', xBzBxx, delimiter = ',')
    
    xBzByy = np.multiply(-1, convolve(np.multiply(bz,byy),psfx, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xBzByy.csv', xBzByy, delimiter = ',')
    
    xBztBxx = np.multiply(-1, convolve(np.multiply(bzt,bxx),psfx, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xBztBxx.csv', xBztBxx, delimiter = ',')
    
    xBztByy = np.multiply(-1, convolve(np.multiply(bzt,byy),psfx, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xBztByy.csv', xBztByy, delimiter = ',')
    
    yBztBxx = np.multiply(-1, convolve(np.multiply(bzt,bxx),psfy, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yBztBxx.csv', yBztBxx, delimiter = ',')
    
    yBztByy = np.multiply(-1, convolve(np.multiply(bzt,byy),psfy, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yBztByy.csv', yBztByy, delimiter = ',')
    
    xyBxxBxx = convolve(np.multiply(bxx,bxx),psfxy, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xyBxxBxx.csv', xyBxxBxx, delimiter = ',')
    
    xyBxxByy = convolve(np.multiply(bxx,byy),psfxy, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xyBxxByy.csv', xyBxxByy, delimiter = ',')
    
    xyByyByy = convolve(np.multiply(byy,byy),psfxy, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xyByyByy.csv', xyByyByy, delimiter = ',')
    
    xyBzxBxx = convolve(np.multiply(bzx,bxx),psfxy, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xyBzxBxx.csv', xyBzxBxx, delimiter = ',')
    
    xyBzxByy = convolve(np.multiply(bzx,byy),psfxy, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xyBzxByy.csv', xyBzxByy, delimiter = ',')
    
    xyBzyBxx = convolve(np.multiply(bzy,bxx),psfxy, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xyBzyBxx.csv', xyBzyBxx, delimiter = ',')
    
    xyBzyByy = convolve(np.multiply(bzy,byy),psfxy, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xyBzyByy.csv', xyBzyByy, delimiter = ',')
    
    yBzBxx = np.multiply(-1, convolve(np.multiply(bz,bxx),psfy, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yBzBxx.csv', yBzBxx, delimiter = ',')
    
    yBzByy = np.multiply(-1, convolve(np.multiply(bz,byy),psfy, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yBzByy.csv', yBzByy, delimiter = ',')
    
    xBzyBxx = np.multiply(-1, convolve(np.multiply(bzy,bxx),psfx, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xBzyBxx.csv', xBzyBxx, delimiter = ',')
    
    xBzyByy = np.multiply(-1, convolve(np.multiply(bzy,byy),psfx, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xBzyByy.csv', xBzyByy, delimiter = ',')
    
    yBzyBxx = np.multiply(-1, convolve(np.multiply(bzy,bxx),psfy, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yBzyBxx.csv', yBzyBxx, delimiter = ',')
    
    yBzyByy = np.multiply(-1, convolve(np.multiply(bzy,byy),psfy, normalize_kernel = False, boundary = 'fill'))
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yBzyByy.csv', yBzyByy, delimiter = ',')
    
    xxBxxBxx = convolve(np.multiply(bxx,bxx),psfxx, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xxBxxBxx.csv', xxBxxBxx, delimiter = ',')
    
    xxBxxByy = convolve(np.multiply(bxx,byy),psfxx, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xxBxxByy.csv', xxBxxByy, delimiter = ',')
    
    xxByyByy = convolve(np.multiply(byy,byy),psfxx, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xxByyByy.csv', xxByyByy, delimiter = ',')
    
    xxBzxBxx = convolve(np.multiply(bzx,bxx),psfxx, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xxBzxBxx.csv', xxBzxBxx, delimiter = ',')
    
    xxBzyBxx = convolve(np.multiply(bzy,bxx),psfxx, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xxBzyBxx.csv', xxBzyBxx, delimiter = ',')
    
    xxBzxByy = convolve(np.multiply(bzx,byy),psfxx, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xxBzxByy.csv', xxBzxByy, delimiter = ',')
    
    xxBzyByy = convolve(np.multiply(bzy,byy),psfxx, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/xxBzyByy.csv', xxBzyByy, delimiter = ',')
    
    yyBxxBxx = convolve(np.multiply(bxx,bxx),psfyy, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yyBxxBxx.csv', yyBxxBxx, delimiter = ',')
    
    yyBxxByy = convolve(np.multiply(bxx,byy),psfyy, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yyBxxByy.csv', yyBxxByy, delimiter = ',')
    
    yyByyByy = convolve(np.multiply(byy,byy),psfyy, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yyByyByy.csv', yyByyByy, delimiter = ',')
    
    yyBzyBxx = convolve(np.multiply(bzy,bxx),psfyy, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yyBzyBxx.csv', yyBzyBxx, delimiter = ',')
    
    yyBzyByy = convolve(np.multiply(bzy,byy),psfyy, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yyBzyByy.csv', yyBzyByy, delimiter = ',')
    
    yyBzxBxx = convolve(np.multiply(bzx,bxx),psfyy, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yyBzxBxx.csv', yyBzxBxx, delimiter = ',')
    
    yyBzxByy = convolve(np.multiply(bzx,byy),psfyy, normalize_kernel = False, boundary = 'fill')
    
    #np.savetxt('/Users/andrechicrala/Downloads/extended/AMcomponents_py/yyBzxByy.csv', yyBzxByy, delimiter = ',')
    
    #############End of individual terms calculation################## 

    #stacking terms
    A = np.stack((Gxx, Gxy, Gx + xGxx, Gx + yGxy, yGxx, xGxy, -BzxBxx - BzxByy, 
  -BzxBx - xBzxBxx - xBzxByy, -BzxBy - yBzxBxx - yBzxByy, Gtx,
 Gxy, Gyy, Gy + xGxy, Gy + yGyy, yGxy, xGyy, -BzyBxx - BzyByy,
  -BzyBx - xBzyBxx - xBzyByy, -BzyBy - yBzyBxx - yBzyByy, Gty, 
 Gx + xGxx, Gy + xGxy, G + 2*xGx + xxGxx, G + xGx + xyGxy + yGy, 
  xyGxx + yGx, xGy + xxGxy, -BzBxx - BzByy - xBzxBxx - xBzxByy, 
  -BzBx - xBzBxx - xBzByy - xBzxBx - xxBzxBxx - xxBzxByy, 
  -BzBy - xBzxBy - xyBzxBxx - xyBzxByy - yBzBxx - yBzByy, Ht + xGtx, 
 Gx + yGxy, Gy + yGyy, G + xGx + xyGxy + yGy, G + 2*yGy + yyGyy, 
  yGx + yyGxy, xGy + xyGyy, -BzBxx - BzByy - yBzyBxx - yBzyByy, 
  -BzBx - xBzBxx - xBzByy - xyBzyBxx - xyBzyByy - yBzyBx, 
  -BzBy - yBzBxx - yBzByy - yBzyBy - yyBzyBxx - yyBzyByy, Ht + yGty, 
 yGxx, yGxy, xyGxx + yGx, yGx + yyGxy, yyGxx, xyGxy, -yBzxBxx - yBzxByy, 
  -xyBzxBxx - xyBzxByy - yBzxBx, -yBzxBy - yyBzxBxx - yyBzxByy, yGtx, 
 xGxy, xGyy, xGy + xxGxy, xGy + xyGyy, xyGxy, xxGyy, -xBzyBxx - xBzyByy, 
  -xBzyBx - xxBzyBxx - xxBzyByy, -xBzyBy - xyBzyBxx - xyBzyByy, xGty, 
 -BzxBxx - BzxByy, -BzyBxx - BzyByy, -BzBxx - BzByy - xBzxBxx - xBzxByy, 
  -BzBxx - BzByy - yBzyBxx - yBzyByy, -yBzxBxx - yBzxByy, -xBzyBxx - xBzyByy, 
  BxxBxx + 2*BxxByy + ByyByy, BxBxx + BxByy + xBxxBxx + 2*xBxxByy + xByyByy, 
  ByBxx + ByByy + yBxxBxx + 2*yBxxByy + yByyByy, -BztBxx - BztByy, 
 -BzxBx - xBzxBxx - xBzxByy, -BzyBx - xBzyBxx - xBzyByy, 
  -BzBx - xBzBxx - xBzByy - xBzxBx - xxBzxBxx - xxBzxByy, 
  -BzBx - xBzBxx - xBzByy - xyBzyBxx - xyBzyByy - yBzyBx, 
  -xyBzxBxx - xyBzxByy - yBzxBx, -xBzyBx - xxBzyBxx - xxBzyByy, 
  BxBxx + BxByy + xBxxBxx + 2*xBxxByy + xByyByy, 
  BxBx + 2*xBxBxx + 2*xBxByy + xxBxxBxx + 2*xxBxxByy + xxByyByy, 
  BxBy + xByBxx + xByByy + xyBxxBxx + 2*xyBxxByy + xyByyByy + yBxBxx + yBxByy,
 -BztBx - xBztBxx - xBztByy, -BzxBy - yBzxBxx - yBzxByy, 
  -BzyBy - yBzyBxx - yBzyByy, -BzBy - xBzxBy - xyBzxBxx - xyBzxByy - yBzBxx - yBzByy,
 -BzBy - yBzBxx - yBzByy - yBzyBy - yyBzyBxx - yyBzyByy, 
  -yBzxBy - yyBzxBxx - yyBzxByy, -xBzyBy - xyBzyBxx - xyBzyByy, 
  ByBxx + ByByy + yBxxBxx + 2*yBxxByy + yByyByy, 
  BxBy + xByBxx + xByByy + xyBxxBxx + 2*xyBxxByy + xyByyByy + yBxBxx + yBxByy,
  ByBy + 2*yByBxx + 2*yByByy + yyBxxBxx + 2*yyBxxByy + yyByyByy, 
  -BztBy - yBztBxx - yBztByy, Gtx, Gty, Ht + xGtx, Ht + yGty, yGtx, xGty, 
  -BztBxx - BztByy, -BztBx - xBztBxx - xBztByy, -BztBy - yBztBxx - yBztByy, 
  Gtt), axis = 0)
    
    return(A)


# In[ ]:

regionName = sys.argv[1]
#start = int(sys.argv[2])
#end=int(sys.argv[3])
inputdir = sys.argv[10]
outputdir= sys.argv[11]
smoothParam = int(sys.argv[12])
num_steps = int(sys.argv[13])

#dir_= inputdir+"/AR_"+regionName
dir_= inputdir

file_length=len(os.listdir(dir_))
#print(int(file_length/3)-1)

c=0              # number of calculated files
if (num_steps>2):
    for t in range(num_steps):
        ch_bx1, ch_bx2 = dir_+"/bx_"+regionName+"_{:d}.txt".format(t) , dir_+"/bx_"+regionName+"_{:d}.txt".format(t+1)
        ch_by1, ch_by2 = dir_+"/by_"+regionName+"_{:d}.txt".format(t) , dir_+"/by_"+regionName+"_{:d}.txt".format(t+1)
        ch_bz1, ch_bz2 = dir_+"/bz_"+regionName+"_{:d}.txt".format(t) , dir_+"/bz_"+regionName+"_{:d}.txt".format(t+1)
        if os.path.isfile(ch_bx1)==True and os.path.isfile(ch_bx2)==True:
            if (os.path.isfile(ch_by1)==True and os.path.isfile(ch_by2)==True):
                if (os.path.isfile(ch_bz1)==True and os.path.isfile(ch_bz2)==True):
                    print('file number ',t)
                    bx_start = np.loadtxt(dir_+"/bx_"+regionName+"_{:d}.txt".format(t))
                    by_start = np.loadtxt(dir_+"/by_"+regionName+"_{:d}.txt".format(t))
                    bz_start = np.loadtxt(dir_+"/bz_"+regionName+"_{:d}.txt".format(t))
                    bx_stop = np.loadtxt(dir_+"/bx_"+regionName+"_{:d}.txt".format(t+1))
                    by_stop = np.loadtxt(dir_+"/by_"+regionName+"_{:d}.txt".format(t+1))
                    bz_stop = np.loadtxt(dir_+"/bz_"+regionName+"_{:d}.txt".format(t+1))
                    #defining a dx and dy in km based on HMI resolution
                    #used 1000 before!
                    dx = 360
                    dy = dx
                
                    #defining the window size
                    window_size = smoothParam
                    
                    #Defining the time interval between two consecutive images in seconds
                    dt = 720
                    #taking the average change on bz over the time interval dt
                    #taking the average change on bz over the time interval dt
                    
                    
                    bzt = (bz_stop - bz_start)/dt
                    #Taking the average value of the images
                    
                    #Those average values will be entries for the odiffxy5 function
                    bx = (bx_stop + bx_start)/2
                    by = (by_stop + by_start)/2
                    bz = (bz_stop + bz_start)/2
                    
                    
                    #calculating the differentials
                    
                    bxx,bxy = odiff(bx)
                    byx,byy = odiff(by)
                    bzx,bzy = odiff(bz)
                    
                    
                    #the dictionary to hold the information
                    
                    #note that window_size is not within the magvm dictionary (as it should in a module)
                    
                    magvm = {'bzt': bzt, 'bx': bx, 'bxx': np.divide(bxx,dx), 
                             'bxy': np.divide(bxy,dy), 'by': by, 'byx': np.divide(byx,dx),
                             'byy': np.divide(byy,dy), 'bz': bz, 'bzx': np.divide(bzx,dx), 
                             'bzy': np.divide(bzy,dy), 'dx': dx, 'dy': dy, 'dt': dt}
                    
                    #Copying the dictionary to a variable    
                    mag_dic = magvm.copy()
                    
                    #Defining arrays
                    #Taking the shape of bz to later create arrays with the same shape.
                    sz = mag_dic['bz'].shape
                    
                    #creating lists to receive the data
                    #creating "dummy" variables
                    U0 = np.zeros((sz[0],sz[1]))
                    V0 = np.zeros((sz[0],sz[1]))
                    UX = np.zeros((sz[0],sz[1]))
                    VY = np.zeros((sz[0],sz[1]))
                    UY = np.zeros((sz[0],sz[1]))
                    VX = np.zeros((sz[0],sz[1]))
                    W0 = np.zeros((sz[0],sz[1]))
                    WX = np.zeros((sz[0],sz[1]))
                    WY = np.zeros((sz[0],sz[1]))
                    
                    #Constructing the weighting functions.
                    nw = int(2*int(window_size/2)+1)
                    
                    #Creating a numpy array based on the windowsize
                    nw2 = np.subtract(np.array(range(0,nw)),10) #check if nw should have an index associated to it
                    
                    #Creating the weighting functions
                    x = np.array([nw2,]*nw)*mag_dic['dx']
                    y = np.matrix.transpose(np.array([nw2,]*nw))*mag_dic['dy']
                    
                    #Creating another array
                    #Use double or float? initially I went with double
                    psf = np.full((nw,nw), 1, dtype = 'float64')
                    
                    #Normalizing this array
                    psf = np.divide(psf, np.sum(psf)) 
                    
                    #Making futher operations
                    psfx = np.multiply(psf,x)
                    psfy = np.multiply(psf,y)
                    psfxx = np.multiply(psf,np.multiply(x,x))
                    psfyy = np.multiply(psf,np.multiply(y,y))
                    psfxy = np.multiply(np.multiply(psf,x),y)
                    
                    #defining the kernel as a dictionary
                    kernel = {'psf': psf, 'psfx': psfx, 
                              'psfy': psfy, 'psfxx': psfxx,
                              'psfyy': psfyy, 'psfxy': psfxy}
                    
                    
                    AM = the_matrix(mag_dic['bx'], mag_dic['bxx'], 
                                    mag_dic['bxy'], mag_dic['by'], 
                                    mag_dic['byx'], mag_dic['byy'],
                                    mag_dic['bz'], mag_dic['bzx'],
                                    mag_dic['bzy'], mag_dic['bzt'],
                                    psf, psfx, psfy, psfxx, psfyy, psfxy)
                    AM = np.reshape(AM,(10,10,sz[0],sz[1]))
                    
                    #computing the trace
                    trc = np.trace(AM, axis1 = 0, axis2 = 1)
                    
                    #indexing points where the trace is bigger than 1
                    index = np.where(trc > 1)
                    for i in range(0,len(index[0])):    
                        #taking a chunk of AM at the specified point
                        AA = AM[:,:,index[0][i],index[1][i]]
                        
                        #taking the first 9 columns to build ''ax''
                        GA = AA[0:9,0:9]
                        
                        #taking the last column to build ''b''
                        FA = -1*np.reshape(AA[9,0:9],9)
                        
                        #defining a vector to receive the answers
                        vector = lstsq(GA, FA, lapack_driver = 'gelss')
                        
                        U0[index[0][i],index[1][i]] = vector[0][0]
                        V0[index[0][i],index[1][i]] = vector[0][1]
                        UX[index[0][i],index[1][i]] = vector[0][2]
                        VY[index[0][i],index[1][i]] = vector[0][3]
                        UY[index[0][i],index[1][i]] = vector[0][4]
                        VX[index[0][i],index[1][i]] = vector[0][5]
                        W0[index[0][i],index[1][i]] = vector[0][6]
                        WX[index[0][i],index[1][i]] = vector[0][7]
                        WY[index[0][i],index[1][i]] = vector[0][8]    
                        
                    del AM, GA, FA, vector
                    gc.collect()    
                        #Organize this as dic?
                    vel4vm = {'U0': U0, 'UX': UX, 'UY': UY, 'V0': V0, 'VX': VX, 'VY': VY,'W0': W0, 'WX': WX, 'WY': WY}
                    Vx= U0.flatten()
                    # a_file = (outputdir+"/AR_"+regionName+"/Ux_"+regionName+"_"+str(smoothParam)+"_{:d}.txt".format(t))
                    # np.savetxt(a_file, Vx[:])
                    # Vy= V0.flatten()
                    # a_file = (outputdir+"/AR_"+regionName+"/Uy_"+regionName+"_"+str(smoothParam)+"_{:d}.txt".format(t))
                    # np.savetxt(a_file, Vy[:])
                    # Vz= W0.flatten()
                    # a_file = (outputdir+"/AR_"+regionName+"/Uz_"+regionName+"_"+str(smoothParam)+"_{:d}.txt".format(t))
                    # np.savetxt(a_file, Vz[:])
                    # a_file = (outputdir+"/AR_"+regionName+"/bx_"+regionName+"_{:d}.txt".format(t))
                    # np.savetxt(a_file, bx_start[:,:].flatten())
                    # a_file = (outputdir+"/AR_"+regionName+"/by_"+regionName+"_{:d}.txt".format(t))
                    # np.savetxt(a_file, by_start[:,:].flatten())
                    # a_file = (outputdir+"/AR_"+regionName+"/bz_"+regionName+"_{:d}.txt".format(t))
                    # np.savetxt(a_file, bz_start[:,:].flatten())

                    a_file = (outputdir+"/Ux_"+regionName+"_"+str(smoothParam)+"_{:d}.txt".format(t))
                    np.savetxt(a_file, Vx[:])
                    Vy= V0.flatten()
                    a_file = (outputdir+"/Uy_"+regionName+"_"+str(smoothParam)+"_{:d}.txt".format(t))
                    np.savetxt(a_file, Vy[:])
                    Vz= W0.flatten()
                    a_file = (outputdir+"/Uz_"+regionName+"_"+str(smoothParam)+"_{:d}.txt".format(t))
                    np.savetxt(a_file, Vz[:])
                    a_file = (outputdir+"/bx_"+regionName+"_{:d}.txt".format(t))
                    np.savetxt(a_file, bx_start[:,:].flatten())
                    a_file = (outputdir+"/by_"+regionName+"_{:d}.txt".format(t))
                    np.savetxt(a_file, by_start[:,:].flatten())
                    a_file = (outputdir+"/bz_"+regionName+"_{:d}.txt".format(t))
                    np.savetxt(a_file, bz_start[:,:].flatten())


                    c+=1
                else:
                    print('The file '+ str("bz_"+regionName+"_{:d}.txt".format(t))+' does not exist')
                    continue

            else:
                print('The file '+ str("by_"+regionName+"_{:d}.txt".format(t))+' does not exist')
                continue

        else:
            print('The file '+ str("bx_"+regionName+"_{:d}.txt".format(t))+' does not exist')
            continue

    print('number of calculated files: ', 3*c)


    ## remove the data in the input file
    # import os
    # import glob
    
    # files = glob.glob(dir_+'/*.txt', recursive=True)
    
    # for f in files:
    #     try:
    #         os.remove(f)
    #     except OSError as e:
    #         print("Error: %s : %s" % (f, e.strerror))    
            
else:
    print('There are not enough data.')

