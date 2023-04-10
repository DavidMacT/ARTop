#ifndef DAVE4VM
#define DAVE4VM
#include "point.h"
#include <iostream>

extern "C" {
  void daxpy_(int* n,double* alpha,double* dx,int* incx,double* dy,int* incy);
  double dnrm2_(int* n,double* x, int* incx);

  void dgetrf_(int* M, int *N, double* A, int* lda, int* IPIV, int* INFO);
  void dgetrs_(char* C, int* N, int* NRHS, double* A, int* LDA, int* IPIV, double* B, int* LDB, int* INFO);
  void dgesv_(int *n, int *nrhs, double *a, int *lda, int *ipiv, double *b, int *ldb, int *info);
  void dgelsd_( int* m, int* n, int* nrhs, double* a, int* lda,
                double* b, int* ldb, double* s, double* rcond, int* rank,
                double* work, int* lwork, int* iwork, int* info );
}

class dave4VM{
public:
  dave4VM(double &dxIn,double &dyIn,int &nxIn,int &nyIn);
  void getDerivativesEdge(std::vector<std::vector<double> > &Bx,std::vector<std::vector<double> > &By,std::vector<std::vector<double> > &Bz,double &dx,double &dy);
  void getDerivatives(std::vector<std::vector<double> > &Bx,std::vector<std::vector<double> > &By,std::vector<std::vector<double> > &Bz,double &dx,double &dy);
  void productTerms(std::vector<std::vector<double> > &B1,std::vector<std::vector<double> > &B2,std::vector<std::vector<double> > &output);
  void sumTerms(std::vector<std::vector<double> > &B1,std::vector<std::vector<double> > &output);
  void sumTerms(double multiplyingFactor,std::vector<std::vector<double> > &B1,std::vector<std::vector<double> > &output);
  void scalarMult(double multiplyingFactor,std::vector<std::vector<double> > &output);
  void weightFunctions(int &windowSize);
  void weightFunctionsOld(int &windowSize);
  double applyFilterOnceX(std::vector<double> &filter,std::vector<std::vector<double> > &quantity,int &Chix,int &ChiY);
  double applyFilterOnceY(std::vector<double> &filter,std::vector<std::vector<double> > &quantity,int &Chix,int &ChiY);
  double applyFilterOnceOld(std::vector<std::vector<double> > &filter,std::vector<std::vector<double> > &quantity,int &Chix,int &ChiY);
  std::vector<std::vector<double> > applyFilterGloballyOld(std::vector<std::vector<double> > &filter,std::vector<std::vector<double> > &quantity);
  std::vector<std::vector<double> > applyFilterGlobally(std::vector<double> &Xfilter,std::vector<double> &Yfilter,std::vector<std::vector<double> > &quantity);  
  void setMatrixTermsPreFilter(std::vector<std::vector<double> > &Bxdt,std::vector<std::vector<double> > &Bydt,std::vector<std::vector<double> > &Bzdt);
  std::vector<std::vector<point> >  composeMatrixAndInvert();
  private:
  std::vector<std::vector<double> > Bx;
  std::vector<std::vector<double> > By;
  std::vector<std::vector<double> > Bz;
  std::vector<std::vector<double> > Bxdx;
  std::vector<std::vector<double> > Bydx;
  std::vector<std::vector<double> > Bzdx;
  std::vector<std::vector<double> > Bxdy;
  std::vector<std::vector<double> > Bydy;
  std::vector<std::vector<double> > Bzdy;
  std::vector<std::vector<double> > Bzsq;
  std::vector<std::vector<double> > BzBzdx;
  std::vector<std::vector<double> > BzBzdy;
  std::vector<std::vector<double> > BzBxdx;
  std::vector<std::vector<double> > BzdxBzdy;
  std::vector<std::vector<double> > Bzdxsq;
  std::vector<std::vector<double> > Bzdysq;
  std::vector<std::vector<double> > BxBzdx;
  std::vector<std::vector<double> > BxBzdy;
  std::vector<std::vector<double> > ByBzdx;
  std::vector<std::vector<double> > ByBzdy;
  std::vector<std::vector<double> > BxBz;
  std::vector<std::vector<double> > ByBz;
  std::vector<std::vector<double> > Bysq;
  std::vector<std::vector<double> > Bxsq;
  std::vector<std::vector<double> > BxdxBzdx;
  std::vector<std::vector<double> > BxdxBzdy;
  std::vector<std::vector<double> > BzBxdy;
  std::vector<std::vector<double> > BydxBzdx;
  std::vector<std::vector<double> > BydxBzdy;
  std::vector<std::vector<double> > BzBydy;
  std::vector<std::vector<double> > BxBxdx;
  std::vector<std::vector<double> > BxBxdy;
  std::vector<std::vector<double> > ByBydx;
  std::vector<std::vector<double> > Bxdxsq;
  std::vector<std::vector<double> > BzdtBzdx;
  std::vector<std::vector<double> > BzdtBzdy;
  std::vector<std::vector<double> > BzBzdt;
  std::vector<std::vector<double> > BzdtBxdx;
  std::vector<std::vector<double> > Bzdtsq;
  std::vector<std::vector<double> > BxBy;
  std::vector<std::vector<double> > ByBxdx;
  std::vector<std::vector<double> > BxBzdt;
  std::vector<std::vector<double> > ByBzdt;
  std::vector<std::vector<double> > BydyBzdx;
  std::vector<std::vector<double> > BydyBzdy;
  std::vector<std::vector<double> > BxdxBydy;
  std::vector<std::vector<double> > Bydysq;
  std::vector<std::vector<double> > BzBy;
  std::vector<std::vector<double> > BzBydz;
  std::vector<std::vector<double> > BxBydy;
  std::vector<std::vector<double> > ByBydy;
  std::vector<std::vector<double> > BzdxBydy;
  std::vector<std::vector<double> > BzdyBydy;
  std::vector<std::vector<double> > BzdtBydy;
  std::vector<std::vector<double> > BzdyBydydyd;
  double dx,dy;int nw,midpoint,nx,ny;
  std::vector<std::vector<double> > filterMat;
  std::vector<std::vector<double> > filterMatX;
  std::vector<std::vector<double> > filterMatY;
  std::vector<std::vector<double> > filterMatXY;
  std::vector<std::vector<double> > filterMatXSq;
  std::vector<std::vector<double> > filterMatYSq;
  std::vector<double> filterMat1D;
  std::vector<double> filterMatX1D;
  std::vector<double> filterMatY1D;
  std::vector<double> filterMatXY1D;
  std::vector<double> filterMatXSq1D;
  std::vector<double> filterMatYSq1D;
};
#endif  

  
 
