#include "interpolation2Large.h"

vecFieldPoint::vecFieldPoint(double &xin,double &yin,double &zin,point &bin){
  x = xin;
  y = yin;
  z = zin;
  b = bin;
}

double vecFieldPoint::getX(){
  return x;
}

double vecFieldPoint::getY(){
  return y;
}

double vecFieldPoint::getZ(){
  return z;
}

void vecFieldPoint::setX(double &xv){
  x =xv;
}

void vecFieldPoint::setY(double &yv){
  y= yv;
}

void vecFieldPoint::setZ(double &zv){
  z =zv;
}

void vecFieldPoint::setP(point &p){
  b=p;
}

void vecFieldPoint::setXComp(double &val){
  b.setX(val);
}

void vecFieldPoint::setYComp(double &val){
  b.setY(val);
}

void vecFieldPoint::setZComp(double &val){
  b.setZ(val);
}


point vecFieldPoint::getPos(){
  point p;
  p.setX(x);
  p.setY(y);
  p.setZ(z);
  return p;
}

point vecFieldPoint::getPoint(){
  return b; 
}

vecField::vecField(std::vector<vecFieldPoint> &fieldDatIn1,std::vector<vecFieldPoint> &fieldDatIn2,int &xdimenIn,int &ydimenIn,int &zdimenIn){
  fieldDat1 = fieldDatIn1;
  fieldDat2 = fieldDatIn2;
  xDimen = xdimenIn;
  yDimen = ydimenIn;
  zDimen = zdimenIn;
  dx = (fieldDat1[xDimen-1].getX()-fieldDat1[0].getX())/double(xDimen-1);
  dy = (fieldDat1[yDimen*xDimen-1].getY()-fieldDat1[0].getY())/double(yDimen-1);
  dz = (fieldDat2[fieldDat2.size()-1].getZ()-fieldDat1[0].getZ())/double(zDimen-1);
  xmin = fieldDat1[0].getX();
  xmax = fieldDat1[xDimen-1].getX();
  ymin = fieldDat1[0].getY();
  ymax = fieldDat1[xDimen*yDimen-1].getY();
  zmin = fieldDat1[0].getZ();
  zmax = fieldDat2[fieldDat2.size()-1].getZ();
  
}

vecField::vecField(int &xdimenIn,int &ydimenIn,int &zdimenIn){
  xDimen = xdimenIn;
  yDimen = ydimenIn;
  zDimen = zdimenIn;
}


void vecField::setField(){
  std::cout<<"in  ?\n";
  dx = (fieldDat1[xDimen-1].getX()-fieldDat1[0].getX())/double(xDimen-1);
  dy = (fieldDat1[yDimen*xDimen-1].getY()-fieldDat1[0].getY())/double(yDimen-1);
  dz = (fieldDat8[fieldDat8.size()-1].getZ()-fieldDat1[0].getZ())/double(zDimen-1);
  xmin = fieldDat1[0].getX();
  xmax = fieldDat1[xDimen-1].getX();
  ymin = fieldDat1[0].getY();
  ymax = fieldDat1[xDimen*yDimen-1].getY();
  zmin = fieldDat1[0].getZ();
  zmax = fieldDat8[fieldDat8.size()-1].getZ();
  maxVecSize = fieldDat1.size();
  /*
  int sec2Size = fieldDat2.size();
  std::cout<<"filling  a? "<<maxVecSize<<" "<<"\n";
  xderivatives1.resize(maxVecSize);
   std::cout<<"filling  b? "<<maxVecSize<<" "<<"\n";
  yderivatives1.resize(maxVecSize);
   std::cout<<"filling  c? "<<maxVecSize<<" "<<"\n";
  zderivatives1.resize(maxVecSize);
   std::cout<<"filling  d? "<<maxVecSize<<" "<<"\n";
  xyderivatives1.resize(maxVecSize);
   std::cout<<"filling  e? "<<maxVecSize<<" "<<"\n";
  xzderivatives1.resize(maxVecSize);
   std::cout<<"filling  f? "<<maxVecSize<<" "<<"\n";
  yzderivatives1.resize(maxVecSize);
   std::cout<<"filling  g? "<<maxVecSize<<" "<<"\n";
  xyzderivatives1.resize(maxVecSize);
  std::cout<<"filling  2?\n";
  xderivatives2.resize(sec2Size);
  yderivatives2.resize(sec2Size);
  zderivatives2.resize(sec2Size);
  xyderivatives2.resize(sec2Size);
  xzderivatives2.resize(sec2Size);
  yzderivatives2.resize(sec2Size);
  xyzderivatives2.resize(sec2Size);
  std::cout<<"rarrr  ?\n";*/
}

void vecField::reSizeVecs(int &size1,int &size2){
  fieldDat1.resize(size1);
  fieldDat2.resize(size1);
  fieldDat3.resize(size1);
  fieldDat4.resize(size1);
  fieldDat5.resize(size1);
  fieldDat6.resize(size1);
  fieldDat7.resize(size1);
  fieldDat8.resize(size2);
  maxVecSize = size1;
}

void vecField::setFieldVal(int &m,int &maxSize,vecFieldPoint &vfp){
  if(m<maxSize){
    fieldDat1[m]=vfp;
  }else if(m< 2*maxSize && m>=maxSize){
    fieldDat2[m-maxSize]= vfp;
  }else if(m< 3*maxSize && m>=2*maxSize){
    fieldDat3[m-2*maxSize]= vfp;
  }else if(m< 4*maxSize && m>=3*maxSize){
    fieldDat4[m-3*maxSize]= vfp;
  }else if(m< 5*maxSize && m>=4*maxSize){
    fieldDat5[m-4*maxSize]= vfp;
  }else if(m< 6*maxSize && m>=5*maxSize){
    fieldDat6[m-5*maxSize]= vfp;
  }else if(m< 7*maxSize && m>=6*maxSize){
    fieldDat7[m-6*maxSize]= vfp;
  }else if(m >= 7*maxSize){
    fieldDat8[m-7*maxSize]= vfp;
  }   
}


double vecField::xinc(){
  return dx;
}

double vecField::yinc(){
  return dy;
}

double vecField::zinc(){
  return dz;
}

double vecField::getXmin(){
  return xmin;
};

double vecField::getXmax(){
  return xmax;
};

double vecField::getYmin(){
  return ymin;
};

double vecField::getYmax(){
  return ymax;
};

double vecField::getZmin(){
  return zmin;
};

double vecField::getZmax(){
  return zmax;
};

void vecField::setZmax(double &zmIN){
  zmax = zmIN;
}



void vecField::getIndicies(double &xv,double &yv,double &zv,int &currX,int &currY,int &currZ){
  double iv = (xv - xmin)/dx;
  double jv = (yv - ymin)/dy;
  double kv = (zv - zmin)/dz;
  currX = floor(iv);
  currY = floor(jv);
  currZ = floor(kv);
  if(std::abs(currX+1-iv)<0.00001){
    currX=currX+1;
  }
  if(std::abs(currY+1-jv)<0.00001){
    currY=currY+1;
  }  
  if(std::abs(currZ+1-kv)<0.00001){
    currZ=currZ+1;
  } 
}

void vecField::getIndicies(point &p,int &currX,int &currY,int &currZ){  
  double iv = (p.getX() - xmin)/dx;
  double jv = (p.getY() - ymin)/dy;
  double kv = (p.getZ() - zmin)/dz;
  currX = floor(iv);
  currY = floor(jv);
  currZ = floor(kv); 
  if(std::abs(currX+1-iv)<0.00001){
    currX=currX+1;
  }
  if(std::abs(currY+1-jv)<0.00001){
    currY=currY+1;
  }  
  if(std::abs(currZ+1-kv)<0.00001){
    currZ=currZ+1;
  }  
}

bool vecField::checkIn(point &p){
  bool isIn = true;
  if(p.getX()< xmin ||p.getX()>xmax){
    isIn = false;
  }else if(p.getY()< ymin ||p.getY()>ymax){
    isIn = false;
  }else if(p.getZ()< zmin ||p.getZ()>zmax){
    isIn = false;
  }
  return isIn;
}

point vecField::getPos(int xIn,int yIn,int zIn){
  point pp;
  int yInt = yIn*xDimen;
  int zInt = zIn*xDimen*yDimen;
  int m = xIn + yInt + zInt;
  if(m<maxVecSize){
    return fieldDat1[m].getPos();
  }else if(m< 2*maxVecSize && m>=maxVecSize){
    return fieldDat2[m-maxVecSize].getPos();
  }else if(m< 3*maxVecSize && m>=2*maxVecSize){
    return fieldDat3[m-2*maxVecSize].getPos();
  }else if(m< 4*maxVecSize && m>=3*maxVecSize){
    return fieldDat4[m-3*maxVecSize].getPos();
  }else if(m< 5*maxVecSize && m>=4*maxVecSize){
    return fieldDat5[m-4*maxVecSize].getPos();
  }else if(m< 6*maxVecSize && m>=5*maxVecSize){
    return fieldDat6[m-5*maxVecSize].getPos();
  }else if(m< 7*maxVecSize && m>=6*maxVecSize){
    return fieldDat7[m-6*maxVecSize].getPos();
  }else{
    return fieldDat8[m-7*maxVecSize].getPos();
  }   
}

point vecField::getField(int xIn,int yIn,int zIn){
  int yInt = yIn*xDimen;
  int zInt = zIn*xDimen*yDimen;
  int m = xIn + yInt + zInt;
  if(m<maxVecSize){
    return fieldDat1[m].getPos();
  }else if(m< 2*maxVecSize && m>=maxVecSize){
    return fieldDat2[m-maxVecSize].getPoint();
  }else if(m< 3*maxVecSize && m>=2*maxVecSize){
    return fieldDat3[m-2*maxVecSize].getPoint();
  }else if(m< 4*maxVecSize && m>=3*maxVecSize){
    return fieldDat4[m-3*maxVecSize].getPoint();
  }else if(m< 5*maxVecSize && m>=4*maxVecSize){
    return fieldDat5[m-4*maxVecSize].getPoint();
  }else if(m< 6*maxVecSize && m>=5*maxVecSize){
    return fieldDat6[m-5*maxVecSize].getPoint();
  }else if(m< 7*maxVecSize && m>=6*maxVecSize){
    return fieldDat7[m-6*maxVecSize].getPoint();
  }else{
    return fieldDat8[m-7*maxVecSize].getPoint();
  }   
} 

point vecField::getXderiv(int xIn,int yIn,int zIn){
  int yInt = yIn*xDimen;
  int zInt = zIn*xDimen*yDimen;
  int finalInt = xIn + yInt + zInt;
  if(finalInt>=maxVecSize){
    return xderivatives1[finalInt-maxVecSize];
  }else{
    return xderivatives2[finalInt];
  }
}

point vecField::getYderiv(int xIn,int yIn,int zIn){
  int yInt = yIn*xDimen;
  int zInt = zIn*xDimen*yDimen;
  int finalInt = xIn + yInt + zInt;
  if(finalInt>= maxVecSize){
    return yderivatives1[finalInt-maxVecSize];
  }else{
    return yderivatives2[finalInt];
  }
}

point vecField::getZderiv(int xIn,int yIn,int zIn){
  int yInt = yIn*xDimen;
  int zInt = zIn*xDimen*yDimen;
  int finalInt = xIn + yInt + zInt;
  if(finalInt>= maxVecSize){
    return zderivatives1[finalInt-maxVecSize];
  }else{
    return zderivatives2[finalInt];
  }
}

point vecField::getXYderiv(int xIn,int yIn,int zIn){
  int yInt = yIn*xDimen;
  int zInt = zIn*xDimen*yDimen;
  int finalInt = xIn + yInt + zInt;
  if(finalInt>= maxVecSize){
    return xyderivatives1[finalInt-maxVecSize];
  }else{
    return xyderivatives2[finalInt];
  }
}

point vecField::getYZderiv(int xIn,int yIn,int zIn){
  int yInt = yIn*xDimen;
  int zInt = zIn*xDimen*yDimen;
  int finalInt = xIn + yInt + zInt;
  if(finalInt>= maxVecSize){
    return yzderivatives1[finalInt-maxVecSize];
  }else{
    return yzderivatives2[finalInt];
  }
}

point vecField::getXZderiv(int xIn,int yIn,int zIn){
  int yInt = yIn*xDimen;
  int zInt = zIn*xDimen*yDimen;
  int finalInt = xIn + yInt + zInt;
  if(finalInt>= maxVecSize){
    return xzderivatives1[finalInt-maxVecSize];
  }else{
    return xzderivatives2[finalInt];
  }
}

point vecField::getXYZderiv(int xIn,int yIn,int zIn){
  int yInt = yIn*xDimen;
  int zInt = zIn*xDimen*yDimen;
  int finalInt = xIn + yInt + zInt;
  if(finalInt>= maxVecSize){
    return xyzderivatives1[finalInt-maxVecSize];
  }else{
    return xyzderivatives2[finalInt];
  }
}

point vecField::getCurrent(int xIn,int yIn,int zIn){
  int yInt = yIn*xDimen;
  int zInt = zIn*xDimen*yDimen;
  int finalInt = xIn + yInt + zInt;
  if(finalInt>= maxVecSize){
    return currentDat1[finalInt-maxVecSize].getPoint();
  }else{
    return currentDat2[finalInt].getPoint();
  }
}

double vecField::getInvmatCoeff(int i,int j){
  return invmat[i][j];
}

int vecField::getXDimen(){
  return xDimen;
};

int vecField::getYDimen(){
  return yDimen;
}

int vecField::getZDimen(){
  return zDimen;
}


void vecField::setXDimen(int &xdIn){
  xDimen = xdIn;
};

void vecField::setYDimen(int &ydIn){
  yDimen = ydIn;
}

void vecField::setZDimen(int &zdIn){
  zDimen = zdIn;
}





void vecField::readInFileWvels(const char* filename,vecField &velField,vecField &rhoField){
  vecFieldPoint vfp;
  vecFieldPoint vfpVel;
  vecFieldPoint vfpRho;
  std::ifstream myfile;
  std::string str;
  double val,xin,yin,zin,rho;
  point bf,vf,rhop;
  int maxSize,maxSize2;
  myfile.open(filename);
  int k =0; int m=0;
   if(myfile.is_open()){
     while(!myfile.eof()){
       k++;
       std::getline(myfile,str);
       std::stringstream ss(str);
       if(k== 1){
	 ss>>val;
	 xDimen = val;
	 velField.setXDimen(xDimen);
	 rhoField.setXDimen(xDimen);
       }else if(k==2){
	 ss>>val;
	 yDimen = val;
	 velField.setYDimen(yDimen);
	 rhoField.setYDimen(yDimen);
       }else if(k==3){
	 ss>>val;
	 zDimen = val;
	 velField.setZDimen(zDimen);
	 rhoField.setZDimen(zDimen);
       }
  else{
	 //get max Dimension and split two ways
	 int l=0;
	 if(m==0){
	   maxSize = int(std::round(xDimen*yDimen*zDimen/8));
	   maxSize2= xDimen*yDimen*zDimen-7*maxSize;
	   fieldDat1.resize(maxSize);
	   fieldDat2.resize(maxSize);
	   fieldDat3.resize(maxSize);
	   fieldDat4.resize(maxSize); 
	   fieldDat5.resize(maxSize);
	   fieldDat6.resize(maxSize);
	   fieldDat7.resize(maxSize); 
	   fieldDat8.resize(maxSize2);     
	   velField.reSizeVecs(maxSize,maxSize2);
	   rhoField.reSizeVecs(maxSize,maxSize2);
	 }
	 //while(l<= 11){
	   ss>>val;
	   xin =val;
	   l++;
	   ss>>val;
	   yin =val;
	   l++;
	   ss>>val;
	   zin = val;
	   l++;
	   ss>>val;
	   bf.setX(val);
	   l++;
	   ss>>val;
	   bf.setY(val);
	   l++;
	   ss>>val;
	   bf.setZ(val);
	   l++;  
	   ss>>val;
	   vf.setX(val);
	   l++;
	   ss>>val; 
	   vf.setZ(val);
	   l++;
	   ss>>val;
	   vf.setZ(val);
	   l++;
	   ss>>rho;  
	   vfp.setX(xin);
	   vfp.setY(yin);
	   vfp.setZ(zin);
	   vfp.setP(bf);
	   vfpVel.setX(xin);
	   vfpVel.setY(yin);
	   vfpVel.setZ(zin);
	   vfpVel.setP(vf);
	   vfpRho.setX(xin);
	   vfpRho.setY(yin);
	   vfpRho.setZ(zin);
	   rhop.setX(rho);
	   rhop.setY(rho);
	   rhop.setZ(rho);
	   vfpRho.setP(rhop);
	   //std::cout<<m<<" "<<xin<<" "<<yin<<" "<<zin<<" "<<rhop.getX()<<"\n";
	   if(m<maxSize){
	     fieldDat1[m]=vfp;
	     l++;
	   }else if(m< 2*maxSize && m>=maxSize){
	     fieldDat2[m-maxSize]= vfp;
	     l++;
	   }else if(m< 3*maxSize && m>=2*maxSize){
	     fieldDat3[m-2*maxSize]= vfp;
	     l++;
	   }else if(m< 4*maxSize && m>=3*maxSize){
	     fieldDat4[m-3*maxSize]= vfp;
	     l++;
	   }else if(m< 5*maxSize && m>=4*maxSize){
	     fieldDat5[m-4*maxSize]= vfp;
	     l++;
	   }else if(m< 6*maxSize && m>=5*maxSize){
	     fieldDat6[m-5*maxSize]= vfp;
	     l++;
	   }else if(m< 7*maxSize && m>=6*maxSize){
	     fieldDat7[m-6*maxSize]= vfp;
	     l++;
	   }else if(m >= 7*maxSize){
	     fieldDat8[m-7*maxSize]= vfp;
	     l++;
	   }
	   velField.setFieldVal(m,maxSize,vfpVel);
	   rhoField.setFieldVal(m,maxSize,vfpRho);  
	   m++;
	   //}   
       }   
     }
     myfile.close();
   }else{
     std::cout<<"file failed to open";
   }
   std::cout<<"post loop\n";
   setField();
   std::cout<<"post loop\n";
   velField.setField();
   std::cout<<"set field 2\n";
   rhoField.setField();
}

void vecField::readInFileJustField(const char* filename){
  vecFieldPoint vfp;
  std::ifstream myfile;
  std::string str;
  double val,xin,yin,zin;
  point bf;
  int maxSize,maxSize2;
  myfile.open(filename);
  int k =0; int m=0;
   if(myfile.is_open()){
     while(!myfile.eof()){
       k++;
       std::getline(myfile,str);
       std::stringstream ss(str);
       if(k== 1){
	 ss>>val;
	 xDimen = val;
       }else if(k==2){
	 ss>>val;
	 yDimen = val;
       }else if(k==3){
	 ss>>val;
	 zDimen = val;
       }
  else{
	 //get max Dimension and split two ways
	 int l=0;
	 if(m==0){
	   maxSize = int(std::round(xDimen*yDimen*zDimen/8));
	   maxSize2= xDimen*yDimen*zDimen-7*maxSize;
	   fieldDat1.resize(maxSize);
	   fieldDat2.resize(maxSize);
	   fieldDat3.resize(maxSize);
	   fieldDat4.resize(maxSize); 
	   fieldDat5.resize(maxSize);
	   fieldDat6.resize(maxSize);
	   fieldDat7.resize(maxSize); 
	   fieldDat8.resize(maxSize2);     
	 }
	 //while(l<= 11){
	   ss>>val;
	   xin =val;
	   l++;
	   ss>>val;
	   yin =val;
	   l++;
	   ss>>val;
	   zin = val;
	   l++;
	   ss>>val;
	   bf.setX(val);
	   l++;
	   ss>>val;
	   bf.setY(val);
	   l++;
	   ss>>val;
	   bf.setZ(val);
	   l++;  
	   vfp.setX(xin);
	   vfp.setY(yin);
	   vfp.setZ(zin);
	   vfp.setP(bf);
	   if(m<maxSize){
	     fieldDat1[m]=vfp;
	     l++;
	   }else if(m< 2*maxSize && m>=maxSize){
	     fieldDat2[m-maxSize]= vfp;
	     l++;
	   }else if(m< 3*maxSize && m>=2*maxSize){
	     fieldDat3[m-2*maxSize]= vfp;
	     l++;
	   }else if(m< 4*maxSize && m>=3*maxSize){
	     fieldDat4[m-3*maxSize]= vfp;
	     l++;
	   }else if(m< 5*maxSize && m>=4*maxSize){
	     fieldDat5[m-4*maxSize]= vfp;
	     l++;
	   }else if(m< 6*maxSize && m>=5*maxSize){
	     fieldDat6[m-5*maxSize]= vfp;
	     l++;
	   }else if(m< 7*maxSize && m>=6*maxSize){
	     fieldDat7[m-6*maxSize]= vfp;
	     l++;
	   }else if(m >= 7*maxSize){
	     fieldDat8[m-7*maxSize]= vfp;
	     l++;
	   }
	   m++;
	   //}   
       }   
     }
     myfile.close();
   }else{
     std::cout<<"file failed to open";
   }
   std::cout<<"post loop\n";
   setField();
}


void vecField::getCurl(int xindex,int yindex,int zindex)
{
  // first get the x variations 
  point xminus,xplus,xcurr,xpartial;
  point yminus,yplus,ycurr,ypartial;
  point zminus,zplus,zcurr,zpartial;
  if(xindex ==0){
    xcurr = getField(xindex,yindex,zindex);
    xplus = getField(xindex+1,yindex,zindex);
    xpartial = (xplus -xcurr)/dx;
  }else if(xindex ==xDimen-1){
    xcurr = getField(xindex-1,yindex,zindex);
    xplus = getField(xindex,yindex,zindex);
    xpartial = (xplus -xcurr)/dx;
  }else{
    xplus = getField(xindex+1,yindex,zindex);
    xminus = getField(xindex-1,yindex,zindex);
    xpartial = (xplus -xminus)/(2.0*dx);
  }
  if(yindex ==0){
    ycurr = getField(xindex,yindex,zindex);
    yplus = getField(xindex,yindex+1,zindex);
    ypartial = (yplus -ycurr)/dy;
  }else if(yindex ==yDimen-1){
    ycurr = getField(xindex,yindex-1,zindex);
    yplus = getField(xindex,yindex,zindex);
    ypartial = (yplus -ycurr)/dy;
  }else{
    yplus = getField(xindex,yindex+1,zindex);
    yminus = getField(xindex,yindex-1,zindex);
    ypartial = (yplus -yminus)/(2.0*dy);
  }
  if(zindex ==0){
    zcurr = getField(xindex,yindex,zindex);
    zplus = getField(xindex,yindex,zindex+1);
    zpartial = (zplus -zcurr)/dz;
  }else if(zindex ==zDimen-1){
    zcurr = getField(xindex,yindex,zindex-1);
    zplus = getField(xindex,yindex,zindex);
    zpartial = (zplus -zcurr)/dz;
  }else{
    zplus = getField(xindex,yindex,zindex+1);
    zminus = getField(xindex,yindex,zindex-1);
    zpartial = (zplus -zminus)/(2.0*dz);
  }
  point curval(-zpartial.getY()+ypartial.getZ(),-xpartial.getZ()+zpartial.getX(),-ypartial.getX()+xpartial.getY());
  point position = getPos(xindex,yindex,zindex);
  double xpos,ypos,zpos;
  xpos = position.getX();
  ypos = position.getY();
  zpos = position.getZ();
  vecFieldPoint vfp(xpos,ypos,zpos,curval);
  // now add to the current vector 
  int yInt = yindex*xDimen;
  int zInt = zindex*xDimen*yDimen;
  int finalInt = xindex + yInt + zInt;
  if(finalInt>= maxVecSize){
    currentDat2[finalInt-maxVecSize]=vfp;
  }else{
    currentDat1[finalInt] = vfp;
  }
}


void  vecField::computeFirstDeriv(int &xindex, int &yindex, int &zindex){
  point xcurr,xplus,xminus;
  point ycurr,yplus,yminus;  
  point zcurr,zplus,zminus;
  point xpartial,ypartial,zpartial;  
  if(xindex ==0){
    xcurr = getField(xindex,yindex,zindex);
    xplus = getField(xindex+1,yindex,zindex);
    xpartial = (xplus -xcurr)/dx;
  }else if(xindex ==xDimen-1){
    xcurr = getField(xindex-1,yindex,zindex);
    xplus = getField(xindex,yindex,zindex);
    xpartial = (xplus -xcurr)/dx;
  }else{
    xplus = getField(xindex+1,yindex,zindex);
    xminus = getField(xindex-1,yindex,zindex);
    xpartial = (xplus -xminus)/(2.0*dx);
	}
  if(yindex ==0){
    ycurr = getField(xindex,yindex,zindex);
    yplus = getField(xindex,yindex+1,zindex);
    ypartial = (yplus -ycurr)/dy;
  }else if(yindex ==yDimen-1){
    ycurr = getField(xindex,yindex-1,zindex);
    yplus = getField(xindex,yindex,zindex);
    ypartial = (yplus -ycurr)/dy;
  }else{
    yplus = getField(xindex,yindex+1,zindex);
    yminus = getField(xindex,yindex-1,zindex);
    ypartial = (yplus -yminus)/(2.0*dy);
  }
  if(zindex ==0){
    zcurr = getField(xindex,yindex,zindex);
    zplus = getField(xindex,yindex,zindex+1);
    zpartial = (zplus -zcurr)/dz;
  }else if(zindex ==zDimen-1){
    zcurr = getField(xindex,yindex,zindex-1);
    zplus = getField(xindex,yindex,zindex);
    zpartial = (zplus -zcurr)/dz;
  }else{
    zplus = getField(xindex,yindex,zindex+1);
    zminus = getField(xindex,yindex,zindex-1);
    zpartial = (zplus -zminus)/(2.0*dz);
  }
  int yInt = yindex*xDimen;
  int zInt = zindex*xDimen*yDimen;
  int finalInt = xindex + yInt + zInt;
  if(finalInt>= maxVecSize){
    xderivatives2[finalInt-maxVecSize] = xpartial;
    yderivatives2[finalInt-maxVecSize] = ypartial;
    zderivatives2[finalInt-maxVecSize] = zpartial;
  }else{
    xderivatives1[finalInt] = xpartial;
    yderivatives1[finalInt] = ypartial;
    zderivatives1[finalInt] = zpartial;
  }
}

void  vecField::computeSecondDeriv(int &xindex, int &yindex, int &zindex){
  point xcurr,xplus,xminus;
  point ycurr,yplus,yminus;  
  point zcurr,zplus,zminus;
  point xypartial,yzpartial,xzpartial;  
  // first the xy derivative
  if(yindex == 0){
    ycurr = getXderiv(xindex,yindex,zindex);
    yplus = getXderiv(xindex,yindex+1,zindex);
    xypartial = (yplus -ycurr)/dy;
  }else if(yindex ==yDimen-1){
    ycurr = getXderiv(xindex,yindex-1,zindex);
    yplus = getXderiv(xindex,yindex,zindex);
	  xypartial = (yplus -ycurr)/dy;
  }else{
    yplus = getXderiv(xindex,yindex+1,zindex);
    yminus = getXderiv(xindex,yindex-1,zindex);
    xypartial = (yplus -yminus)/(2.0*dy);
	}
  // then the yz derivative
  if(yindex ==0){
    ycurr = getZderiv(xindex,yindex,zindex);
    yplus = getZderiv(xindex,yindex+1,zindex);
    yzpartial = (yplus -ycurr)/dy;
  }else if(yindex ==yDimen-1){
    ycurr = getZderiv(xindex,yindex-1,zindex);
    yplus = getZderiv(xindex,yindex,zindex);
    yzpartial = (yplus -ycurr)/dy;
  }else{
    yplus = getZderiv(xindex,yindex+1,zindex);
    yminus = getZderiv(xindex,yindex-1,zindex);
    yzpartial = (yplus -yminus)/(2.0*dy);
  }
  //  the xz derivative
  if(zindex ==0){
    zcurr = getXderiv(xindex,yindex,zindex);
    zplus = getXderiv(xindex,yindex,zindex+1);
    xzpartial = (zplus -zcurr)/dz;
  }else if(zindex ==zDimen-1){
    zcurr = getXderiv(xindex,yindex,zindex-1);
    zplus = getXderiv(xindex,yindex,zindex);
    xzpartial = (zplus -zcurr)/dz;
  }else{
    zplus = getXderiv(xindex,yindex,zindex+1);
    zminus = getXderiv(xindex,yindex,zindex-1);
    xzpartial = (zplus -zminus)/(2.0*dz);
  }
  int yInt = yindex*xDimen;
  int zInt = zindex*xDimen*yDimen;
  int finalInt = xindex + yInt + zInt;
  if(finalInt>= maxVecSize){
    xyderivatives2[finalInt-maxVecSize] = xypartial;
    yzderivatives2[finalInt-maxVecSize] = yzpartial;
    xzderivatives2[finalInt-maxVecSize] = xzpartial;
  }else{
     xyderivatives1[finalInt] = xypartial;
     yzderivatives1[finalInt] = yzpartial;
     xzderivatives1[finalInt] = xzpartial;
  }
}

void  vecField::computeThirdDerivXYZ(int &xindex, int &yindex, int &zindex){
  point zcurr,zplus,zminus;
  point xyzpartial;
  if(zindex ==0){
    zcurr = getXYderiv(xindex,yindex,zindex);
    zplus = getXYderiv(xindex,yindex,zindex+1);
    xyzpartial = (zplus -zcurr)/dz;
  }else if(zindex ==zDimen-1){
    zcurr = getXYderiv(xindex,yindex,zindex-1);
    zplus = getXYderiv(xindex,yindex,zindex);
    xyzpartial = (zplus -zcurr)/dz;
  }else{
    zplus = getXYderiv(xindex,yindex,zindex+1);
    zminus = getXYderiv(xindex,yindex,zindex-1);
    xyzpartial = (zplus -zminus)/(2.0*dz);
  }
  int yInt = yindex*xDimen;
  int zInt = zindex*xDimen*yDimen;
  int finalInt = xindex + yInt + zInt;
  if(finalInt>= maxVecSize){
    xyzderivatives2[finalInt-maxVecSize] = xyzpartial;
  }else{
    xyzderivatives1[finalInt] = xyzpartial;
  }
}
void vecField::preComputeDerivatives()
{
  // read in the inverse matrix
  std::ifstream myfile;std::string str;double val;
  std::vector<double> invmat_line;
  myfile.open("inverse.txt");
  int k=0;int l=0;
   if(myfile.is_open()){
     while(!myfile.eof()&& l<64){
       k=0;
       l++;
       std::getline(myfile,str);
       std::stringstream ss(str);
       while(!ss.eof()&&k<64){
       k++;
       ss>>val;
	 invmat_line.push_back(val);
       }
       invmat.push_back(invmat_line);
       invmat_line.clear();
     }
     myfile.close();
   }else{
     std::cout<<"inverse interpolation file failed to open\n";
  }
   // first get the first and second order variations 
  for(int xindex =0;xindex<xDimen;xindex++){
    for(int yindex =0;yindex<yDimen;yindex++){
      for(int zindex=0;zindex<zDimen;zindex++){
	// compute the first derivatives
	      computeFirstDeriv(xindex,yindex,zindex); 
      }
    }
  }
  for(int xindex =0;xindex<xDimen;xindex++){
    for(int yindex =0;yindex<yDimen;yindex++){
      for(int zindex=0;zindex<zDimen;zindex++){
	// compute the first derivatives
	      computeSecondDeriv(xindex,yindex,zindex); 
      }
    }
  }
  for(int xindex =0;xindex<xDimen;xindex++){
    for(int yindex =0;yindex<yDimen;yindex++){
      for(int zindex=0;zindex<zDimen;zindex++){
	// compute the triple derivatives
	  computeThirdDerivXYZ(xindex,yindex,zindex);
      }
    }
  }
}



void vecField::calculateCurrent(){
  // first resize the current vector to be as big as the 
  currentDat1.resize(maxVecSize);
  currentDat2.resize(xDimen*yDimen*zDimen-maxVecSize);
  int xindex,yindex,zindex;
  int l=0;
  for(int i=0;i<zDimen;i++){
    for(int j=0;j<yDimen;j++){
      for(int k=0;k<xDimen;k++){
        getCurl(k,j,i);
      }
    }
  }
}

void vecField::deleteField(){
  fieldDat1.clear();
  currentDat1.clear();
  fieldDat2.clear();
  currentDat2.clear();
  std::cout<<"fields have been deleted ?"<<fieldDat1.size()<<" "<<currentDat1.size()<<"\n";
}


point interpolate::getField(point &pIn,bool &outDomain,vecField &field){
  // first check if the point is even in 
  bool inDomain = field.checkIn(pIn);
  if(inDomain){
    outDomain =false;
  }else{
    outDomain =true;
  }
  point pout;
  int xind=0;
  int yind=0;
  int zind=0;
  double x1,y1,z1;
  if(outDomain==false){
    // grab the indiciesd of one box edge
    //std::cout<<"pt "<<pIn.getX()<<" "<<pIn.getY()<<" "<<pIn.getZ()<<"\n";
    field.getIndicies(pIn,xind,yind,zind);
    // transform into positions on a 1X1X1 cube (including a check if we are on the grid boudnary)
    //std::cout<<xind<<" "<<yind<<" "<<zind<<" and pos\n ";
    point fieldPos = field.getPos(xind,yind,zind);
    if(xind == field.getXDimen()-1){
      xind = xind-1;
      x1 =1.0;
    }else{
      if(xind == 0){
	xind = xind+1;
	x1=-1.0;
      }else{
      x1 = (pIn.getX()-fieldPos.getX())/field.xinc();
      }
    }
    if(yind ==field.getYDimen()-1){
      yind = yind-1;
      y1 = 1.0;
    }else{
      if(yind == 0){
	 yind = yind+1;
	 y1 = -1.0;
      }
      else{
      y1 =  (pIn.getY()-fieldPos.getY())/field.yinc();
      }
    }
    if(zind == field.getZDimen()-1){
       zind = zind-1;
       z1 = 1.0;
    }else{
      if(zind ==0){
       zind = zind+1;
       z1 = -1.0;
      }else{
      z1 =  (pIn.getZ()-fieldPos.getZ())/field.zinc();
      }
    }
    //std::cout<<"pre \n ";
    v000 = field.getField(xind,yind,zind);
    //v000.printPoint();
    v010 = field.getField(xind,yind+1,zind);
    //v010.printPoint();
    v001 = field.getField(xind,yind,zind+1);
    //v001.printPoint();
    v011 = field.getField(xind,yind+1,zind+1);
    //v011.printPoint();
    v100 = field.getField(xind+1,yind,zind);
    //v100.printPoint();
    v110 = field.getField(xind+1,yind+1,zind);
    //v110.printPoint();
    v101 = field.getField(xind+1,yind,zind+1);
    //v101.printPoint();
    v111 = field.getField(xind+1,yind+1,zind+1);
    //v111.printPoint();
    //std::cout<<v000.getX()<<" "<<v000.getY()<<" "<<v000.getZ()<<"\n";
    //std::cout<<"here ? "<<xind<<" "<<yind<<" "<<zind<<"\n ";
    pout = v000 + (v100-v000)*x1 + (v010-v000)*y1 + (v110-v100-v010+v000)*x1*y1 + (v001-v000)*z1 + (v101-v100-v001+v000)*x1*z1 + (v011-v010-v001+v000)*y1*z1 + (v111-v110-v101-v011+v100+v010+v001-v000)*x1*y1*z1;
    //std::cout<<"main "<<pout.getX()<<" "<<pout.getY()<<" "<<pout.getZ()<<"\n";
  }else{
    double np=0.0;
    pout.setX(np);
    pout.setY(np);
    pout.setZ(np);
  }
  return pout;
}

point interpolate::getFieldTricubic(point &pIn,bool &outDomain,vecField &field){
  // first check if the point is even in 
  bool inDomain = field.checkIn(pIn);
  if(inDomain){
    outDomain =false;
  }else{
    outDomain =true;
  }
  point pout;
  int xind=0;
  int yind=0;
  int zind=0;
  double x1,y1,z1,dx,dy,dz;
  dx = field.xinc();
  dy=field.yinc();
  dz=field.zinc();
  if(inDomain){
    // grab the indiciesd of one box edge
    field.getIndicies(pIn,xind,yind,zind);
    // transform into positions on a 1X1X1 cube (including a check if we are on the grid boudnary)
    //std::cout<<xind<<" "<<yind<<" "<<zind<<" and pos\n ";
    point fieldPos = field.getPos(xind,yind,zind);
    if(xind == field.getXDimen()-1){
      xind = xind-1;
      x1 =1.0;
    }else{
      if(xind == 0){
	xind = xind+1;
	x1=-1.0;
      }else{
      x1 = (pIn.getX()-fieldPos.getX())/dx;
      }
    }
    if(yind ==field.getYDimen()-1){
      yind = yind-1;
      y1 = 1.0;
    }else{
      if(yind == 0){
	 yind = yind+1;
	 y1 = -1.0;
      }
      else{
      y1 =  (pIn.getY()-fieldPos.getY())/dy;
      }
    }
    if(zind == field.getZDimen()-1){
       zind = zind-1;
       z1 = 1.0;
    }else{
      if(zind ==0){
       zind = zind+1;
       z1 = -1.0;
      }else{
      z1 =  (pIn.getZ()-fieldPos.getZ())/dz;
      }
    }
    std::vector<point> alpha;
    point alphaval;
    for(int i=0;i<64;i++){
      alphaval = field.getField(xind,yind,zind)*field.getInvmatCoeff(0,i)+
	      field.getField(xind+1,yind,zind)*field.getInvmatCoeff(1,i)+  
	      field.getField(xind,yind+1,zind)*field.getInvmatCoeff(2,i)+
	      field.getField(xind+1,yind+1,zind)*field.getInvmatCoeff(3,i)+
	      field.getField(xind,yind,zind+1)*field.getInvmatCoeff(4,i)+
	      field.getField(xind+1,yind,zind+1)*field.getInvmatCoeff(5,i)+
	      field.getField(xind,yind+1,zind+1)*field.getInvmatCoeff(6,i)+
		  field.getField(xind+1,yind+1,zind+1)*field.getInvmatCoeff(7,i)+

         (field.getXderiv(xind,yind,zind)*field.getInvmatCoeff(8,i)+
	      field.getXderiv(xind+1,yind,zind)*field.getInvmatCoeff(9,i)+
	      field.getXderiv(xind,yind+1,zind)*field.getInvmatCoeff(10,i)+
              field.getXderiv(xind+1,yind+1,zind)*field.getInvmatCoeff(11,i)+
	      field.getXderiv(xind,yind,zind+1)*field.getInvmatCoeff(12,i)+
      	      field.getXderiv(xind+1,yind,zind+1)*field.getInvmatCoeff(13,i)+
	      field.getXderiv(xind,yind+1,zind+1)*field.getInvmatCoeff(14,i)+
	 field.getXderiv(xind+1,yind+1,zind+1)*field.getInvmatCoeff(15,i))*dx+

	(field.getYderiv(xind,yind,zind)*field.getInvmatCoeff(16,i)+
	      field.getYderiv(xind+1,yind,zind)*field.getInvmatCoeff(17,i)+
              field.getYderiv(xind,yind+1,zind)*field.getInvmatCoeff(18,i)+
	      field.getYderiv(xind+1,yind+1,zind)*field.getInvmatCoeff(19,i)+
	      field.getYderiv(xind,yind,zind+1)*field.getInvmatCoeff(20,i)+
	      field.getYderiv(xind+1,yind,zind+1)*field.getInvmatCoeff(21,i)+
	      field.getYderiv(xind,yind+1,zind+1)*field.getInvmatCoeff(22,i)+
	 field.getYderiv(xind+1,yind+1,zind+1)*field.getInvmatCoeff(23,i))*dy+

        (field.getZderiv(xind,yind,zind)*field.getInvmatCoeff(24,i)+
	      field.getZderiv(xind+1,yind,zind)*field.getInvmatCoeff(25,i)+
              field.getZderiv(xind,yind+1,zind)*field.getInvmatCoeff(26,i)+
	      field.getZderiv(xind+1,yind+1,zind)*field.getInvmatCoeff(27,i)+
	      field.getZderiv(xind,yind,zind+1)*field.getInvmatCoeff(28,i)+
	      field.getZderiv(xind+1,yind,zind+1)*field.getInvmatCoeff(29,i)+
	      field.getZderiv(xind,yind+1,zind+1)*field.getInvmatCoeff(30,i)+
	 field.getZderiv(xind+1,yind+1,zind+1)*field.getInvmatCoeff(31,i))*dz+
	
	(field.getXYderiv(xind,yind,zind)*field.getInvmatCoeff(32,i)+
	      field.getXYderiv(xind+1,yind,zind)*field.getInvmatCoeff(33,i)+
	      field.getXYderiv(xind,yind+1,zind)*field.getInvmatCoeff(34,i)+
	      field.getXYderiv(xind+1,yind+1,zind)*field.getInvmatCoeff(35,i)+
	      field.getXYderiv(xind,yind,zind+1)*field.getInvmatCoeff(36,i)+
	      field.getXYderiv(xind+1,yind,zind+1)*field.getInvmatCoeff(37,i)+
	      field.getXYderiv(xind,yind+1,zind+1)*field.getInvmatCoeff(38,i)+
	 field.getXYderiv(xind+1,yind+1,zind+1)*field.getInvmatCoeff(39,i))*dx*dy +
	
	(field.getXZderiv(xind,yind,zind)*field.getInvmatCoeff(40,i)+
	      field.getXZderiv(xind+1,yind,zind)*field.getInvmatCoeff(41,i)+
	      field.getXZderiv(xind,yind+1,zind)*field.getInvmatCoeff(42,i)+
              field.getXZderiv(xind+1,yind+1,zind)*field.getInvmatCoeff(43,i)+
	      field.getXZderiv(xind,yind,zind+1)*field.getInvmatCoeff(44,i)+
              field.getXZderiv(xind+1,yind,zind+1)*field.getInvmatCoeff(45,i)+
	      field.getXZderiv(xind,yind+1,zind+1)*field.getInvmatCoeff(46,i)+
	 field.getXZderiv(xind+1,yind+1,zind+1)*field.getInvmatCoeff(47,i))*dx*dz +

	(field.getYZderiv(xind,yind,zind)*field.getInvmatCoeff(48,i)+
              field.getYZderiv(xind+1,yind,zind)*field.getInvmatCoeff(49,i)+
	      field.getYZderiv(xind,yind+1,zind)*field.getInvmatCoeff(50,i)+
	      field.getYZderiv(xind+1,yind+1,zind)*field.getInvmatCoeff(51,i)+
              field.getYZderiv(xind,yind,zind+1)*field.getInvmatCoeff(52,i)+
	      field.getYZderiv(xind+1,yind,zind+1)*field.getInvmatCoeff(53,i)+
	      field.getYZderiv(xind,yind+1,zind+1)*field.getInvmatCoeff(54,i)+
	      field.getYZderiv(xind+1,yind+1,zind+1)*field.getInvmatCoeff(55,i))*dy*dz+
	
	(field.getXYZderiv(xind,yind,zind)*field.getInvmatCoeff(56,i)+
	      field.getXYZderiv(xind+1,yind,zind)*field.getInvmatCoeff(57,i)+
	      field.getXYZderiv(xind,yind+1,zind)*field.getInvmatCoeff(58,i)+
	      field.getXYZderiv(xind+1,yind+1,zind)*field.getInvmatCoeff(59,i)+
	      field.getXYZderiv(xind,yind,zind+1)*field.getInvmatCoeff(60,i)+
	      field.getXYZderiv(xind+1,yind,zind+1)*field.getInvmatCoeff(61,i)+
	      field.getXYZderiv(xind,yind+1,zind+1)*field.getInvmatCoeff(62,i)+
	 field.getXYZderiv(xind+1,yind+1,zind+1)*field.getInvmatCoeff(63,i))*dx*dy*dz;
	      alpha.push_back(alphaval); 
    }
    point temp(0.0,0.0,0.0);
    int zco,yco,xco;
    for(int i=0;i<64;i++){
      zco = floor(i/16);
      yco = int(floor(i/4))%4;
      xco = int(floor(i))%4;
      temp = temp+alpha[i]*std::pow(x1,xco)*std::pow(y1,yco)*std::pow(z1,zco);
    }
    pout=temp;
  }else{
    double np=0.0;
    pout.setX(np);
    pout.setY(np);
    pout.setZ(np);
  }
  return pout;
}

point interpolate::current(point &pIn,vecField &field){
  // first check if the point is even in 
  bool inDomain = field.checkIn(pIn);
  point pout;
  int xind=0;
  int yind=0;
  int zind=0;
  double x1,y1,z1;
  if(inDomain){
    // grab the indiciesd of one box edge 
    field.getIndicies(pIn,xind,yind,zind);
    // transform into positions on a 1X1X1 cube (including a check if we are on the grid boudnary)
    point fieldPos = field.getPos(xind,yind,zind);
    if(xind == field.getXDimen()){
      xind = xind-1;
      x1 =1.0;
    }else{
      if(xind == 0){
	xind = xind+1;
	x1=-1.0;
      }else{
      x1 = (pIn.getX()-fieldPos.getX())/field.xinc();
      }
    }
    if(yind ==field.getYDimen() ){
      yind = yind-1;
      y1 = 1.0;
    }else{
      if(yind == 0){
	 yind = yind+1;
	 y1 = -1.0;
      }
      else{
      y1 =  (pIn.getY()-fieldPos.getY())/field.yinc();
      }
    }
    if(zind == field.getZDimen()){
       zind = zind-1;
       z1 = 1.0;
    }else{
      if(zind ==0){
       zind = zind+1;
       z1 = -1.0;
      }else{
      z1 =  (pIn.getZ()-fieldPos.getZ())/field.zinc();
      }
    }
    v000 = field.getCurrent(xind,yind,zind);
    //v000.printPoint();
    v010 = field.getCurrent(xind,yind+1,zind);
    //v010.printPoint();
    v001 = field.getCurrent(xind,yind,zind+1);
    //v001.printPoint();
    v011 = field.getCurrent(xind,yind+1,zind+1);
    //v011.printPoint();
    v100 = field.getCurrent(xind+1,yind,zind);
    //v100.printPoint();
    v110 = field.getCurrent(xind+1,yind+1,zind);
    //v110.printPoint();
    v101 = field.getCurrent(xind+1,yind,zind+1);
    //v101.printPoint();
    v111 = field.getCurrent(xind+1,yind+1,zind+1);
    //v111.printPoint();
    pout = v000 + (v100-v000)*x1 + (v010-v000)*y1 + (v110-v100-v010+v000)*x1*y1 + (v001-v000)*z1 + (v101-v100-v001+v000)*x1*z1 + (v011-v010-v001+v000)*y1*z1 + (v111-v110-v101-v011+v100+v010+v001-v000)*x1*y1*z1; 
  }else{
    double np=0.0;
    pout.setX(np);
    pout.setY(np);
    pout.setZ(np);
  } 
  return pout;
}




