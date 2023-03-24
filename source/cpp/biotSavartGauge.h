#ifndef BS_GAUGE
#define BS_GAUGE

#include "point.h"

class biotSavartGauge{
public:
  biotSavartGauge(double &x0,double &y0,double &lenx,double &leny,int &nxIn,int &nyIn);
  double weightTrap(int &i,int &j);
  std::vector<double> getWindingObsPotFast(int &xi,int &xj,std::vector<std::vector<point> > &vecFCur,std::vector<std::vector<point> > &vecFPot,std::vector<std::vector<point> > &bfield,std::vector<std::vector<point> > &vfield,std::vector<std::vector<point> > &RfieldCur,std::vector<std::vector<point> > &RfieldPot,double &magCutOff,int &downsampleFAC);
  std::vector<double> getWindingObsPotFastPreCalc(int &xi,int &xj,std::vector<std::vector<std::vector<double> > >  coordDifs, std::vector<std::vector<double> > bmagCVec,std::vector<std::vector<point> > &vecFCur,std::vector<std::vector<point> > &vecFPot,std::vector<std::vector<point> > &bfield,std::vector<std::vector<point> > &vfield,std::vector<std::vector<point> > &RfieldCur,std::vector<std::vector<point> > &RfieldPot,double &magCutOff,int &downsampleFAC);
  point getBiotSavartGauge(int &xi,int &xj,std::vector<std::vector<point> > vecF);
private:
  int nx,ny;
  double leftx,lefty,d1,d2,dx,dy;
  double windfrac;
};

#endif
