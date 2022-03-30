#ifndef INTERP_H
#define INTERP_H

#include "pointCheap.h"
#include "biotSavartGauge.h"
#include <iostream>
#include <string>
#include <string.h>
#include <tuple>
#include <ctime>
#include <algorithm>

class vecFieldPoint{
public:
  vecFieldPoint(double &xin,double &yin,double &zin,point &bin);
  vecFieldPoint(){};
  double getX();
  double getY();
  double getZ();
  void setX(double &xv);
  void setY(double &yv);
  void setZ(double &zv);
  void setXComp(double &val);
  void setYComp(double &val);
  void setZComp(double &val);
  void setP(point &p);
  point getPos();
  point getPoint();
private:
  double x,y,z;
  point b;
};


class vecField{
public:
  vecField(std::vector<vecFieldPoint> &fieldDatIn1,std::vector<vecFieldPoint> &fieldDatIn2,int &xdimenIn,int &ydimenIn,int &zdimenIn);
  vecField(int &xdimenIn,int &ydimenIn,int &zdimenIn);
  vecField(){};
  void setField();
  void setFieldVal(int &index,int &maxSize,vecFieldPoint &vfp);
  void reSizeVecs(int &size1,int &size2);
  void getIndicies(double &xv,double &yv,double &zv,int &currX,int &currY,int &currZ);
  void getIndicies(point &p,int &currX,int &currY,int &currZ);
  point getPos(int xIn,int yIn,int zIn);
  point getField(int xIn,int yIn,int zIn);
  point getXderiv(int xIn,int yIn,int zIn);
  point getYderiv(int xIn,int yIn,int zIn);
  point getZderiv(int xIn,int yIn,int zIn);
  point getXYderiv(int xIn,int yIn,int zIn);
  point getXZderiv(int xIn,int yIn,int zIn);
  point getYZderiv(int xIn,int yIn,int zIn);
  point getXYZderiv(int xIn,int yIn,int zIn);
  point getCurrent(int xIn,int yIn,int zIn);
  double getInvmatCoeff(int i,int j);
  void  computeFirstDeriv(int &xindex, int &yIndex, int &zindex);
  void  computeSecondDeriv(int &xindex, int &yindex, int &zindex);
  void  computeThirdDerivXYZ(int &xindex, int &yIndex, int &zindex);
  void  preComputeDerivatives();
  bool checkIn(point &p);
  double xinc();
  double yinc();
  double zinc();
  double getXmin();
  double getXmax();
  double getYmin();
  double getYmax();
  double getZmin();
  double getZmax();
  void setZmax(double &zmIN);
  int getXDimen();
  int getYDimen();
  int getZDimen();
  void setXDimen(int &xdIn);
  void setYDimen(int &ydIn);
  void setZDimen(int &zdIn);
  void readInFile(const char* filename);
  void readInFile2(const char* filename);
  void readInFileTwist(const char* filename,int nxIn,int nyIn,int nzIn);
  void readInFileDavid(const char* filename);
  void readInFileWvels(const char* filename,vecField &velField,vecField &rhoField);
  void readInFileJustField(const char* filename);
  void getCurl(int xindex,int yindex,int zindex);
  //double lorentzMag(int &i);
  //void getMaxLorentzInterior();
  void calculateCurrent();
  void deleteField();
private:
  std::vector<vecFieldPoint> fieldDat1;
  std::vector<vecFieldPoint> fieldDat2;
  std::vector<vecFieldPoint> fieldDat3;
  std::vector<vecFieldPoint> fieldDat4;
  std::vector<vecFieldPoint> fieldDat5;
  std::vector<vecFieldPoint> fieldDat6;
  std::vector<vecFieldPoint> fieldDat7;
  std::vector<vecFieldPoint> fieldDat8;
  std::vector<vecFieldPoint> currentDat1;
  std::vector<vecFieldPoint> currentDat2;
  std::vector<point> xderivatives1;
  std::vector<point> yderivatives1;
  std::vector<point> zderivatives1;
  std::vector<point> xyderivatives1;
  std::vector<point> xzderivatives1;
  std::vector<point> yzderivatives1;
  std::vector<point> xyzderivatives1;
  std::vector<point> xderivatives2;
  std::vector<point> yderivatives2;
  std::vector<point> zderivatives2;
  std::vector<point> xyderivatives2;
  std::vector<point> xzderivatives2;
  std::vector<point> yzderivatives2;
  std::vector<point> xyzderivatives2;
  std::vector<std::vector<double> > invmat;
  int xDimen,yDimen,zDimen,maxVecSize;
double dx,dy,dz,xmin,xmax,ymin,ymax,zmin,zmax;
};

class interpolate{
public:
  interpolate(){invmat = {{1.0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,1.0,0,0,0,0,0,0,0,0,0,0,0},{-3.0,3.0,0,0,-2.0,-1.0,0,0,0,0,0,0,0,0,0,0},{2.0,-2.0,0,0,1.0,1.0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,1.0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,1.0,0,0,0},{0,0,0,0,0,0,0,0,-3.0,3.0,0,0,-2.0,-1.0,0,0},{0,0,0,0,0,0,0,0,2.0,-2.0,0,0,1.0,1.0,0,0},{-3.0,0,3.0,0,0,0,0,0,-2.0,0,-1.0,0,0,0,0,0},{0,0,0,0,-3.0,0,3.0,0,0,0,0,0,-2.0,0,-1.0,0},{9.0,-9.0,-9.0,9.0,6.0,3.0,-6.0,-3.0,6.0,-6.0,3.0,-3.0,4.0,2.0,2.0,1.0},{-6.0,6.0,6.0,-6.0,-3.0,-3.0,3.0,3.0,-4.0,4.0,-2.0,2.0,-2.0,-2.0,-1.0,-1.0},{2.0,0,-2.0,0,0,0,0,0,1.0,0,1.0,0,0,0,0,0},{0,0,0,0,2.0,0,-2.0,0,0,0,0,0,1.0,0,1.0,0},{-6.0,6.0,6.0,-6.0,-4.0,-2.0,4.0,2.0,-3.0,3.0,-3.0,3.0,-2.0,-1.0,-2.0,-1.0},{4.0,-4.0,-4.0,4.0,2.0,2.0,-2.0,-2.0,2.0,-2.0,2.0,-2.0,1.0,1.0,1.0,1.0}};};
  point getField(point &pIn,bool &inDomain,vecField &fieldIn);
  point getFieldTricubic(point &pIn,bool &outDomain,vecField &field);
  point current(point &pin,vecField &fieldIn);
private:
  point v000;
  point v010;
  point v001;
  point v011;
  point v100;
  point v110;
  point v101;
  point v111;
  std::vector<std::vector<double> > invmat;
};


#endif
