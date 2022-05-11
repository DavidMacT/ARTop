#ifndef BS_GAUGE
#define BS_GAUGE

#include "point.h"

class biotSavartGauge{
public:
   biotSavartGauge(double &x0,double &y0,double &lenx,double &leny,int &nxIn,int &nyIn);
  double weightTrap(int &i,int &j);
  double getWindingJac(int &xi,int &xj,std::vector<std::vector<point> > vecF,std::vector<std::vector<std::pair<double,double> > > &grid, std::vector<std::vector<double> > &jacmat);
  std::pair<double,double> getWindingFollow(point &p,int cvIndex,int zindex,std::vector<std::vector<std::pair<point,point> > > &vecF);
  std::vector<std::pair<double,double> > getWindingFollowFullZ(std::vector<std::pair<point,point> > &curve,int cvIndex,std::vector<std::vector<std::pair<point,point> > > &vecF);
  std::vector<std::pair<double,double> > getTestVector(double val,int sz);
  std::pair<double,double> getWinding(int &xi,int &xj,std::vector<std::vector<point> > vecF);
  double sgn(double &vl);
  std::pair<double,double> getWindingHI(int &xi,int &xj,std::vector<std::vector<point> > vecF);
  point getWindingNonEuclid(int &xi,int &xj,std::vector<std::vector<point> > &vecF,std::vector<std::vector<double> > &jacList);
  std::vector<double> getWindingNonEuclidCutOff(int &xi,int &xj,std::vector<std::vector<point> > &vecF,std::vector<std::vector<point> > &bfield,std::vector<std::vector<point> > &vfield,std::vector<std::vector<point> > &Rfield,double &cut3);
  std::vector<double> getWindingObs(int &xi,int &xj,std::vector<std::vector<point> > &vecF,std::vector<std::vector<point> > &bfield,std::vector<std::vector<point> > &vfield,std::vector<std::vector<point> > &Rfield,double &magCutOff);
  std::vector<double> getWindingObsPot(int &xi,int &xj,std::vector<std::vector<point> > &vecFCur,std::vector<std::vector<point> > &vecFPot,std::vector<std::vector<point> > &bfield,std::vector<std::vector<point> > &vfield,std::vector<std::vector<point> > &RfieldCur,std::vector<std::vector<point> > &RfieldPot,double &magCutOff);
  std::vector<double> getWindingObsPotFast(int &xi,int &xj,std::vector<std::vector<point> > &vecFCur,std::vector<std::vector<point> > &vecFPot,std::vector<std::vector<point> > &bfield,std::vector<std::vector<point> > &vfield,std::vector<std::vector<point> > &RfieldCur,std::vector<std::vector<point> > &RfieldPot,double &magCutOff,int &downsampleFAC);
  std::pair<double,double> getWindingPoint(point &p,point &f,std::vector<std::vector<point> > vecF);
  point getBiotSavartGauge(int &xi,int &xj,std::vector<std::vector<point> > vecF);
private:
  int nx,ny;
  double leftx,lefty,d1,d2,dx,dy;
  double windfrac;
};

#endif
