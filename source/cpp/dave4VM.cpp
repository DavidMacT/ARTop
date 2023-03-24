#include "dave4VM.h"

  
dave4VM::dave4VM(double &dxIn,double &dyIn,int &nxIn,int &nyIn){
  dx=dxIn;dy=dyIn;nx=nxIn;ny=nyIn;
  Bxdx.resize(nx, std::vector<double>(ny));
  Bydx.resize(nx, std::vector<double>(ny));
  Bzdx.resize(nx, std::vector<double>(ny));
  Bxdx.resize(nx, std::vector<double>(ny));
  Bydx.resize(nx, std::vector<double>(ny));
  Bzdx.resize(nx, std::vector<double>(ny));
  Bxdx.resize(nx, std::vector<double>(ny));
  Bydx.resize(nx, std::vector<double>(ny));
  Bzdx.resize(nx, std::vector<double>(ny));
  Bxdy.resize(nx, std::vector<double>(ny));
  Bydy.resize(nx, std::vector<double>(ny));
  Bzdy.resize(nx, std::vector<double>(ny));
  Bzsq.resize(nx, std::vector<double>(ny));
  BzBzdx.resize(nx, std::vector<double>(ny));
  BzBxdx.resize(nx, std::vector<double>(ny));
  BzBzdy.resize(nx, std::vector<double>(ny));
  BzdxBzdy.resize(nx, std::vector<double>(ny));
  Bzdxsq.resize(nx, std::vector<double>(ny));
  Bzdysq.resize(nx, std::vector<double>(ny));
  BxBzdx.resize(nx, std::vector<double>(ny));
  BxBzdy.resize(nx, std::vector<double>(ny));
  ByBzdx.resize(nx, std::vector<double>(ny));
  ByBzdy.resize(nx, std::vector<double>(ny));
  BxBz.resize(nx, std::vector<double>(ny));
  ByBz.resize(nx, std::vector<double>(ny));
  Bysq.resize(nx, std::vector<double>(ny));
  Bxsq.resize(nx, std::vector<double>(ny));
  BxdxBzdx.resize(nx, std::vector<double>(ny));
  BxdxBzdy.resize(nx, std::vector<double>(ny));
  BzBxdy.resize(nx, std::vector<double>(ny));
  BydxBzdx.resize(nx, std::vector<double>(ny));
  BydxBzdy.resize(nx, std::vector<double>(ny));
  BzBydy.resize(nx, std::vector<double>(ny));
  BxBxdx.resize(nx, std::vector<double>(ny));
  BxBxdy.resize(nx, std::vector<double>(ny));
  ByBydx.resize(nx, std::vector<double>(ny));
  Bxdxsq.resize(nx, std::vector<double>(ny));
  BzdtBzdx.resize(nx, std::vector<double>(ny));
  BzdtBzdy.resize(nx, std::vector<double>(ny));
  BzBzdt.resize(nx, std::vector<double>(ny));
  BzdtBxdx.resize(nx, std::vector<double>(ny));
  Bzdtsq.resize(nx, std::vector<double>(ny));
  BxBy.resize(nx, std::vector<double>(ny));
  ByBxdx.resize(nx, std::vector<double>(ny));
  BxBzdt.resize(nx, std::vector<double>(ny));
  ByBzdt.resize(nx, std::vector<double>(ny));
  BzdxBzdy.resize(nx, std::vector<double>(ny));
  BzdxBzdy.resize(nx, std::vector<double>(ny));
  BydyBzdx.resize(nx, std::vector<double>(ny));
  BydyBzdy.resize(nx, std::vector<double>(ny));
  BxdxBydy.resize(nx, std::vector<double>(ny));
  Bydysq.resize(nx, std::vector<double>(ny));
  BzBy.resize(nx, std::vector<double>(ny));
  BzBydz.resize(nx, std::vector<double>(ny));
  BxBydy.resize(nx, std::vector<double>(ny));
  ByBydy.resize(nx, std::vector<double>(ny));
  BzdtBydy.resize(nx, std::vector<double>(ny));
  BzdxBydy.resize(nx, std::vector<double>(ny));
  BzdyBydy.resize(nx, std::vector<double>(ny));
  BydyBzdx.resize(nx, std::vector<double>(ny));
  BydyBzdy.resize(nx, std::vector<double>(ny));
  BxdxBydy.resize(nx, std::vector<double>(ny));
  Bydysq.resize(nx, std::vector<double>(ny));
  BzBydz.resize(nx, std::vector<double>(ny));
  BxBydy.resize(nx, std::vector<double>(ny));
  ByBydy.resize(nx, std::vector<double>(ny));
  BzdtBydy.resize(nx, std::vector<double>(ny));
  BzdyBydydyd.resize(nx, std::vector<double>(ny));
}
  

void dave4VM::getDerivatives(std::vector<std::vector<double> > &BxIn,std::vector<std::vector<double> > &ByIn,std::vector<std::vector<double> > &BzIn,double &dx,double &dy){
  Bx=BxIn;
  By=ByIn;
  Bz=BzIn;
  //double C1=2.0/3.0;
  //double C2=1.0/12.0;
  double C2 = 0.12019;
  double C1 = 0.74038;
  for(int i=0;i<Bx.size();i++){
    for(int j=0;j<Bx[0].size();j++){
      if(i==0||i==1){
	//use forward deriv
	Bxdx[i][j] =(-(25.0/12.0)*Bx[i][j]+4.0*Bx[i+1][j]-3.0*Bx[i+2][j]+(4.0/3.0)*Bx[i+3][j]-0.25*Bx[i+4][j])/dx;
	Bydx[i][j] =(-(25.0/12.0)*By[i][j]+4.0*By[i+1][j]-3.0*By[i+2][j]+(4.0/3.0)*By[i+3][j]-0.25*By[i+4][j])/dx;
	Bzdx[i][j] =(-(25.0/12.0)*Bz[i][j]+4.0*Bz[i+1][j]-3.0*Bz[i+2][j]+(4.0/3.0)*Bz[i+3][j]-0.25*Bz[i+4][j])/dx;
      }else if(i==nx-1 || i==nx-2){
	//use backward deriv
	Bxdx[i][j] =((25.0/12.0)*Bx[i][j]-4.0*Bx[i-1][j]+3.0*Bx[i-2][j]-(4.0/3.0)*Bx[i-3][j]+0.25*Bx[i-4][j])/dx;
	Bydx[i][j] =((25.0/12.0)*By[i][j]-4.0*By[i-1][j]+3.0*By[i-2][j]-(4.0/3.0)*By[i-3][j]+0.25*By[i-4][j])/dx;
	Bzdx[i][j] =((25.0/12.0)*Bz[i][j]-4.0*Bz[i-1][j]+3.0*Bz[i-2][j]-(4.0/3.0)*Bz[i-3][j]+0.25*Bz[i-4][j])/dx;
      }else{
	//central diff
	//std::cout<<"here?\n "<<Bz[i+1][j]<<" "<<Bz[i-1][j]<<"\n";
	Bxdx[i][j] =(C1*(Bx[i+1][j]-Bx[i-1][j]) -C2*(Bx[i+2][j]-Bx[i-2][j]))/dx;
	Bydx[i][j] =(C1*(By[i+1][j]-By[i-1][j]) -C2*(By[i+2][j]-By[i-2][j]))/dx;
	Bzdx[i][j] =(C1*(Bz[i+1][j]-Bz[i-1][j]) -C2*(Bz[i+2][j]-Bz[i-2][j]))/dx;
      }
      if(j==0||j==1){
	//use forward deriv
	Bxdy[i][j] =(-(25.0/12.0)*Bx[i][j]+4.0*Bx[i][j+1]-3.0*Bx[i][j+2]+(4.0/3.0)*Bx[i][j+3]-0.25*Bx[i][j+4])/dy;
	Bydy[i][j] =(-(25.0/12.0)*By[i][j]+4.0*By[i][j+1]-3.0*By[i][j+2]+(4.0/3.0)*By[i][j+3]-0.25*By[i][j+4])/dy;
	Bzdy[i][j] =(-(25.0/12.0)*Bz[i][j]+4.0*Bz[i][j+1]-3.0*Bz[i][j+2]+(4.0/3.0)*Bz[i][j+3]-0.25*Bz[i][j+4])/dy;
      }else if(j==ny-1 || j==ny-2){
	//use backward deriv
	Bxdy[i][j] =((25.0/12.0)*Bx[i][j]-4.0*Bx[i][j-1]+3.0*Bx[i][j-2]-(4.0/3.0)*Bx[i][j-3]+0.25*Bx[i][j-4])/dy;
	Bydy[i][j] =((25.0/12.0)*By[i][j]-4.0*By[i][j-1]+3.0*By[i][j-2]-(4.0/3.0)*By[i][j-3]+0.25*By[i][j-4])/dy;
	Bzdy[i][j] =((25.0/12.0)*Bz[i][j]-4.0*Bz[i][j-1]+3.0*Bz[i][j-2]-(4.0/3.0)*Bz[i][j-3]+0.25*Bz[i][j-4])/dy;
      }else{
	Bxdy[i][j] =(C1*(Bx[i][j+1]-Bx[i][j-1]) -C2*(Bx[i][j+2]-Bx[i][j-2]))/dy;
	Bydy[i][j] =(C1*(By[i][j+1]-By[i][j-1]) -C2*(By[i][j+2]-By[i][j-2]))/dy;
	Bzdy[i][j] =(C1*(Bz[i][j+1]-Bz[i][j-1]) -C2*(Bz[i][j+2]-Bz[i][j-2]))/dy;
      }
    }
  }
}

void dave4VM::productTerms(std::vector<std::vector<double> > &B1,std::vector<std::vector<double> > &B2,std::vector<std::vector<double> > &output){
  for(int i=0;i<B1.size();i++){
    for(int j=0;j<B1[0].size();j++){
      output[i][j]=B1[i][j]*B2[i][j];
    }
  }
}

void dave4VM::sumTerms(std::vector<std::vector<double> > &B1,std::vector<std::vector<double> > &output){
  for(int i=0;i<B1.size();i++){
    for(int j=0;j<B1[0].size();j++){
      output[i][j]=output[i][j]+B1[i][j];
    }
  }
}


void dave4VM::sumTerms(double multiplyingFactor,std::vector<std::vector<double> > &B1,std::vector<std::vector<double> > &output){
  for(int i=0;i<B1.size();i++){
    for(int j=0;j<B1[0].size();j++){
      output[i][j]=multiplyingFactor*B1[i][j] + output[i][j];
    }
  }
}

void dave4VM::scalarMult(double multiplyingFactor,std::vector<std::vector<double> > &output){
  for(int i=0;i<output.size();i++){
    for(int j=0;j<output[0].size();j++){
      output[i][j]=multiplyingFactor*output[i][j];
    }
  }
}


void dave4VM::weightFunctionsOld(int &windowSize){
  //turn the window size into an odd number
  nw = int(2*int(windowSize/2))+1;
  // midpoint for calculating the relative positions
  midpoint = (nw-1)/2;
  // resize the matricies
  filterMat.resize(nw, std::vector<double>(nw));
  filterMatX.resize(nw, std::vector<double>(nw));
  filterMatXSq.resize(nw, std::vector<double>(nw));
  filterMatY.resize(nw, std::vector<double>(nw));
  filterMatYSq.resize(nw, std::vector<double>(nw));
  filterMatXY.resize(nw, std::vector<double>(nw));
  // now create the filers.
  for(int i=0;i<nw;i++){
    for(int j=0;j<nw;j++){
      filterMat[i][j] = 1.0/(nw*nw);
      filterMatX[i][j] = dx*(i-(midpoint-1))/double(nw*nw);
      filterMatXSq[i][j] = dx*dx*(i-(midpoint-1))*(i-(midpoint-1))/double(nw*nw);
      filterMatY[i][j] = dy*(j-(midpoint-1))/double((nw*nw));
      filterMatYSq[i][j] = dy*dy*(j-(midpoint-1))*(j-(midpoint-1))/double(nw*nw);
      filterMatXY[i][j] = dx*dy*(i-(midpoint-1))*(j-(midpoint-1))/double(nw*nw);
    }
  }
}

void dave4VM::weightFunctions(int &windowSize){
  //turn the window size into an odd number
  nw = int(2*int(windowSize/2))+1;
  // midpoint for calculating the relative positions
  midpoint = ((nw-1)/2)+1;
    // resize the matricies
  filterMat1D.resize(nw);
  filterMatX1D.resize(nw);
  filterMatXSq1D.resize(nw);
  filterMatY1D.resize(nw);
  filterMatYSq1D.resize(nw);
  // now create the filers.
  for(int i=0;i<nw;i++){
    filterMat1D[i] = 1.0/double(nw);
    filterMatX1D[i] = dx*(i-(midpoint-1))/double(nw);
    filterMatXSq1D[i] = dx*dx*(i-(midpoint-1))*(i-(midpoint-1))/double(nw);
    filterMatY1D[i] = dy*(i-(midpoint-1))/double(nw);
    filterMatYSq1D[i] = dy*dy*(i-(midpoint-1))*(i-(midpoint-1))/double(nw);
  }
}

double dave4VM::applyFilterOnceOld(std::vector<std::vector<double> > &filter,std::vector<std::vector<double> > &quantity,int &Chix,int &ChiY){
  double output=0.0;
  for(int i=0;i<filter.size();i++){
    for(int j=0;j<filter.size();j++){
      if(((Chix+i-midpoint-1)>-1 &&(Chix+i-midpoint-1)<nx) && ((ChiY+j-midpoint-1) >-1 && (ChiY+j-midpoint-1)<ny)){
	output=output + filter[i][j]*quantity[Chix+i-midpoint-1][ChiY+j-midpoint-1];
      }
    }
  }
  return output;
}

double dave4VM::applyFilterOnceX(std::vector<double> &filter,std::vector<std::vector<double> > &quantity,int &Chix,int &ChiY){
  double output=0.0;
  for(int i=0;i<filter.size();i++){
    if((Chix+i-midpoint+1 >-1) &&(Chix+i-midpoint+1 <nx)){
	output=output + filter[i]*quantity[Chix+i-midpoint+1][ChiY];
    }
  }
  return output;
}

double dave4VM::applyFilterOnceY(std::vector<double> &filter,std::vector<std::vector<double> > &quantity,int &Chix,int &ChiY){
  double output=0.0;
  for(int i=0;i<filter.size();i++){
    if((ChiY+i-midpoint+1 >-1) &&(ChiY+i-midpoint+1 <ny)){
	output=output + filter[i]*quantity[Chix][ChiY+i-midpoint+1];
    }
  }
  return output;
}


std::vector<std::vector<double> > dave4VM::applyFilterGloballyOld(std::vector<std::vector<double> > &filter,std::vector<std::vector<double> > &quantity){
  std::vector<std::vector<double> > outputQuantity(quantity.size(),std::vector<double>(quantity[0].size()));
  double output=0.0;
  for(int i=0;i<quantity.size();i++){
    for(int j=0;j<quantity[0].size();j++){
      outputQuantity[i][j] = applyFilterOnceOld(filter,quantity,i,j);
    }
  }
  return outputQuantity;
}

std::vector<std::vector<double> > dave4VM::applyFilterGlobally(std::vector<double> &Xfilter,std::vector<double> &Yfilter,std::vector<std::vector<double> > &quantity){
  std::vector<std::vector<double> > outputQuantityXFiltered(quantity.size(),std::vector<double>(quantity[0].size()));
   std::vector<std::vector<double> > outputQuantity(quantity.size(),std::vector<double>(quantity[0].size()));
  double output=0.0;
  for(int i=0;i<quantity.size();i++){
    for(int j=0;j<quantity[0].size();j++){
      outputQuantityXFiltered[i][j] = applyFilterOnceX(Xfilter,quantity,i,j);
    }
  }
  for(int i=0;i<quantity.size();i++){
    for(int j=0;j<quantity[0].size();j++){
      outputQuantity[i][j] = applyFilterOnceY(Yfilter,outputQuantityXFiltered,i,j);
    }
  }
  return outputQuantity;
}

void dave4VM::setMatrixTermsPreFilter(std::vector<std::vector<double> > &Bxdt,std::vector<std::vector<double> > &Bydt,std::vector<std::vector<double> > &Bzdt){
  //pre calc common terms Bz^2 Bz(dxBz) Bz(dyBz)#
  productTerms(Bz,Bz,Bzsq);productTerms(Bz,Bzdx,BzBzdx);productTerms(Bz,Bzdy,BzBzdy);
   //(dxBz)(dyBz),(dxBz)^2,(dyBz)^2
  productTerms(Bzdx,Bzdy,BzdxBzdy);productTerms(Bzdx,Bzdx,Bzdxsq);productTerms(Bzdy,Bzdy,Bzdysq);
  //Bz(dxBz) Bz(dyBz) Bx(dxBz)
  productTerms(Bzdx,Bz,BzBzdx);productTerms(Bz,Bzdy,BzBzdy);productTerms(Bx,Bzdx,BxBzdx);
  //Bx(dyBz) By(dyBz) By(dxBz)
  productTerms(Bx,Bzdy,BxBzdy);productTerms(By,Bzdx,ByBzdx);productTerms(By,Bzdy,ByBzdy);
  // S product terms
  // BxBz ByBz By^2 Bx^2
  productTerms(Bx,Bz,BxBz);productTerms(By,Bz,ByBz);productTerms(By,By,Bysq);productTerms(Bx,Bx,Bxsq);
  // (dxBx)(dxBz), (dxBx)(dyBz) Bz(dxBx) Bz(dyBx)
  productTerms(Bxdx,Bzdx,BxdxBzdx);productTerms(Bxdx,Bzdy,BxdxBzdy);productTerms(Bz,Bxdy,BzBxdy);productTerms(Bz,Bxdy,BzBxdy);
   //(dxBy)(dxBz), (dxBy)(dyBz) Bz(dxBy) Bz(dyBy)
  productTerms(Bydx,Bzdx,BydxBzdx);productTerms(Bydx,Bzdy,BydxBzdy);productTerms(Bz,Bydy,BzBydy);productTerms(Bz,Bydy,BzBydy);
  // Bx(dxBx) Bx(dyBx) By(dxBy) By(dyBy) 
  productTerms(Bx,Bxdx,BxBxdx);productTerms(Bx,Bxdy,BxBxdy);productTerms(By,Bydx,ByBydx);productTerms(By,Bydy,ByBydy);
  // (dxBx)^2
  productTerms(Bxdx,Bxdx,Bxdxsq);
  // with t terms
  // dtBzdxBz dtBzdyBz BzdtBz
  productTerms(Bzdt,Bzdx,BzdtBzdx);productTerms(Bzdt,Bzdy,BzdtBzdy);productTerms(Bz,Bzdt,BzBzdt);
  // dtBzdxBx, (dtBz)^2 BzdxBz
  productTerms(Bzdt,Bxdx,BzdtBxdx);productTerms(Bzdt,Bzdt,Bzdtsq);productTerms(Bz,Bxdx,BzBxdx);
  //BxBy BydxBx
  productTerms(Bx,By,BxBy); productTerms(By,Bxdx,ByBxdx);
  //BxdtBz BydtBz
   productTerms(Bx,Bzdt,BxBzdt); productTerms(By,Bzdt,ByBzdt);
   // dxBzdyBy dyBzdyBy dyBydxBz
   productTerms(Bzdx,Bydy,BzdxBydy);productTerms(Bzdy,Bydy,BzdyBydy);BydyBzdx=BzdxBydy;
   BydyBzdy=BzdyBydy; productTerms(Bxdx,Bydy,BxdxBydy); productTerms(Bydy,Bydy,Bydysq);
   productTerms(Bz,BzBy,BzBydz);productTerms(Bx,Bydy,BxBydy);productTerms(By,Bydy,ByBydy);
   productTerms(Bzdt,Bydy,BzdtBydy);
}

std::vector<std::vector<point> >  dave4VM::composeMatrixAndInvert(){
  /*****************************************************************

       Smoothed Bz derivs

   *****************************************************************/
  std::vector<std::vector<double> > BzdxsqSm = applyFilterGlobally(filterMat1D,filterMat1D,Bzdxsq);
  std::vector<std::vector<double> > BzdysqSm = applyFilterGlobally(filterMat1D,filterMat1D,Bzdysq);
  std::vector<std::vector<double> > BzdxBzdySm = applyFilterGlobally(filterMat1D,filterMat1D,BzdxBzdy);
  std::vector<std::vector<double> > Bzdxsqxd = applyFilterGlobally(filterMatX1D,filterMat1D,Bzdxsq);
  std::vector<std::vector<double> > Bzdxsqyd = applyFilterGlobally(filterMat1D,filterMatY1D,Bzdxsq);
  std::vector<std::vector<double> > Bzdysqxd = applyFilterGlobally(filterMatX1D,filterMat1D,Bzdysq);
  std::vector<std::vector<double> > Bzdysqyd = applyFilterGlobally(filterMat1D,filterMatY1D,Bzdysq);
  std::vector<std::vector<double> > BzBzdxSm = applyFilterGlobally(filterMat1D,filterMat1D,BzBzdx);
  std::vector<std::vector<double> > BzBzdySm = applyFilterGlobally(filterMat1D,filterMat1D,BzBzdy);
  std::vector<std::vector<double> > BzBzdxxd = applyFilterGlobally(filterMatX1D,filterMat1D,BzBzdx);
  std::vector<std::vector<double> > BzBzdyxd = applyFilterGlobally(filterMatX1D,filterMat1D,BzBzdy);
  std::vector<std::vector<double> > BzBzdxyd = applyFilterGlobally(filterMat1D,filterMatY1D,BzBzdx);
  std::vector<std::vector<double> > BzBzdyyd = applyFilterGlobally(filterMat1D,filterMatY1D,BzBzdy);
  std::vector<std::vector<double> > BzdxBzdyxd = applyFilterGlobally(filterMatX1D,filterMat1D,BzdxBzdy);
  std::vector<std::vector<double> > BzdxBzdyyd = applyFilterGlobally(filterMat1D,filterMatY1D,BzdxBzdy);
  std::vector<std::vector<double> > BzsqSm = applyFilterGlobally(filterMat1D,filterMat1D,Bzsq);
  std::vector<std::vector<double> > Bzdxsqxdsq = applyFilterGlobally(filterMatXSq1D,filterMat1D,Bzdxsq);
  std::vector<std::vector<double> > BzdxBzdyxdyd = applyFilterGlobally(filterMatX1D,filterMatY1D,BzdxBzdy);
  std::vector<std::vector<double> > BzdxBzdyydSq = applyFilterGlobally(filterMat1D,filterMatYSq1D,BzdxBzdy);
  std::vector<std::vector<double> > BzdxBzdyxdSq = applyFilterGlobally(filterMatXSq1D,filterMat1D,BzdxBzdy);
  std::vector<std::vector<double> > Bzdxsqydsq = applyFilterGlobally(filterMat1D,filterMatYSq1D,Bzdxsq);
  std::vector<std::vector<double> > Bzdysqydsq = applyFilterGlobally(filterMat1D,filterMatYSq1D,Bzdysq);
  std::vector<std::vector<double> > Bzdxsqxdyd = applyFilterGlobally(filterMatX1D,filterMatY1D,Bzdxsq);
  std::vector<std::vector<double> > Bzdysqxdyd = applyFilterGlobally(filterMatX1D,filterMatY1D,Bzdysq);
  std::vector<std::vector<double> > BzBzdxxdsq = applyFilterGlobally(filterMatXSq1D,filterMat1D,BzBzdx);
  std::vector<std::vector<double> > BzBzdyxdsq = applyFilterGlobally(filterMatXSq1D,filterMat1D,BzBzdy);
  std::vector<std::vector<double> > BzBzdxydsq = applyFilterGlobally(filterMat1D,filterMatYSq1D,BzBzdx);
  std::vector<std::vector<double> > BzBzdyydsq = applyFilterGlobally(filterMat1D,filterMatYSq1D,BzBzdy);
  std::vector<std::vector<double> > BzdxBzdyydsq = applyFilterGlobally(filterMat1D,filterMatYSq1D,BzdxBzdy);
  std::vector<std::vector<double> > BzdxBzdyxdsq = applyFilterGlobally(filterMatXSq1D,filterMat1D,BzdxBzdy);
  std::vector<std::vector<double> > Bzdysqxdsq = applyFilterGlobally(filterMatXSq1D,filterMat1D,Bzdysq);
  std::vector<std::vector<double> > ByBzdxSm = applyFilterGlobally(filterMat1D,filterMat1D,ByBzdx);
  std::vector<std::vector<double> > ByBzdySm = applyFilterGlobally(filterMat1D,filterMat1D,ByBzdy);
  /*****************************************************************

      Matrix terms fro Bz derivs

   ****************************************************************/

  // G00
  std::vector<std::vector<double> > G00 =BzdxsqSm;
  // G10
  std::vector<std::vector<double> > G10 = BzdxBzdySm;
  // G11
  std::vector<std::vector<double> > G11 = BzdysqSm;
  // G20
  std::vector<std::vector<double> > G20= BzBzdxSm;sumTerms(Bzdxsqxd,G20);
  // G21
  std::vector<std::vector<double> >  G21 = BzBzdySm;sumTerms(BzdxBzdyxd,G21);
  // G22
  std::vector<std::vector<double> > G22 = BzsqSm;sumTerms(2.0,BzBzdxxd,G22);sumTerms(Bzdxsqxdsq,G22);
  // G30
  std::vector<std::vector<double> > G30 = BzBzdxSm;sumTerms(BzdxBzdyyd,G30);
  // G31
  std::vector<std::vector<double> > G31 = BzBzdySm;sumTerms(Bzdysqyd,G31);
  // G32
  std::vector<std::vector<double> > G32 = BzsqSm;sumTerms(BzBzdxxd,G32);sumTerms(BzBzdyyd,G32);sumTerms(BzdxBzdyxdyd,G32);
  // G33
  std::vector<std::vector<double> > G33 = BzsqSm;sumTerms(2.0,BzBzdyyd,G33);sumTerms(Bzdysqydsq,G33);
  // G40
  std::vector<std::vector<double> > G40 = Bzdxsqyd;
  // G41
  std::vector<std::vector<double> > G41 = BzdxBzdyyd;
  // G42
  std::vector<std::vector<double> > G42 = BzBzdxyd;sumTerms(Bzdxsqxdyd,G42);
  // G43
  std::vector<std::vector<double> > G43 = BzBzdxyd;sumTerms(BzdxBzdyydsq,G43);
  // G44
  std::vector<std::vector<double> > G44 = Bzdxsqydsq;
  // G50
  std::vector<std::vector<double> > G50 = BzdxBzdyxd;
  // G51
  std::vector<std::vector<double> > G51 = Bzdysqxd;
  // G52
  std::vector<std::vector<double> > G52 = BzBzdyxd;sumTerms(BzdxBzdyxdsq,G52);
  // G53
  std::vector<std::vector<double> > G53 = BzBzdyxd;sumTerms(Bzdysqxdyd,G53);
  // G54
  std::vector<std::vector<double> > G54 = BzdxBzdyxdyd;
  // G55
  std::vector<std::vector<double> > G55 = Bzdysqxdsq;
  /*************************************************************************

   S term smoothings

   *************************************************************************/
  std::vector<std::vector<double> > BxdxBzdxSm= applyFilterGlobally(filterMat1D,filterMat1D,BxdxBzdx);
  std::vector<std::vector<double> > BxdxBzdySm= applyFilterGlobally(filterMat1D,filterMat1D,BxdxBzdy);
  std::vector<std::vector<double> > BzBxdxSm= applyFilterGlobally(filterMat1D,filterMat1D,BzBxdx);
  std::vector<std::vector<double> > BzBxdySm= applyFilterGlobally(filterMat1D,filterMat1D,BzBxdy);
  std::vector<std::vector<double> > BxdxBzdxxd = applyFilterGlobally(filterMatX1D,filterMat1D,BxdxBzdx);
  std::vector<std::vector<double> > BxdxBzdyxd = applyFilterGlobally(filterMatX1D,filterMat1D,BxdxBzdy);
  std::vector<std::vector<double> > BxdxBzdxyd = applyFilterGlobally(filterMat1D,filterMatY1D,BxdxBzdx);
  std::vector<std::vector<double> > BxdxBzdyyd = applyFilterGlobally(filterMat1D,filterMatY1D,BxdxBzdy);
  std::vector<std::vector<double> > BxdxsqSm = applyFilterGlobally(filterMat1D,filterMat1D,Bxdxsq);
  std::vector<std::vector<double> > BxBzdxSm = applyFilterGlobally(filterMat1D,filterMat1D,BxBzdx);
  std::vector<std::vector<double> > BxBzdySm = applyFilterGlobally(filterMat1D,filterMat1D,BxBzdy);
  std::vector<std::vector<double> > BxBzSm = applyFilterGlobally(filterMat1D,filterMat1D,BxBz);
  std::vector<std::vector<double> > ByBzSm = applyFilterGlobally(filterMat1D,filterMat1D,ByBz);
  std::vector<std::vector<double> > BxBySm = applyFilterGlobally(filterMat1D,filterMat1D,BxBy);
  std::vector<std::vector<double> > BxBzdyyd = applyFilterGlobally(filterMat1D,filterMatY1D,BxBzdy);
  std::vector<std::vector<double> > BxBzdyxd = applyFilterGlobally(filterMatX1D,filterMat1D,BxBzdy);
  std::vector<std::vector<double> > BxBzdxyd = applyFilterGlobally(filterMat1D,filterMatY1D,BxBzdx);
  std::vector<std::vector<double> > BxBzdxxd = applyFilterGlobally(filterMatX1D,filterMat1D,BxBzdx);
  std::vector<std::vector<double> > BxdxBzdxxdyd = applyFilterGlobally(filterMatX1D,filterMatY1D,BxdxBzdx);
  std::vector<std::vector<double> > BxdxBzdyxdyd = applyFilterGlobally(filterMatX1D,filterMatY1D,BxdxBzdy);
  std::vector<std::vector<double> > BzBxdxxd = applyFilterGlobally(filterMatX1D,filterMat1D,BzBxdx);
  std::vector<std::vector<double> > BzBxdxyd = applyFilterGlobally(filterMat1D,filterMatY1D,BzBxdx);
  std::vector<std::vector<double> > BxdxBzdxxdsq = applyFilterGlobally(filterMatXSq1D,filterMat1D,BxdxBzdx);
  std::vector<std::vector<double> > BxdxBzdyxdsq = applyFilterGlobally(filterMatXSq1D,filterMat1D,BxdxBzdy);
  std::vector<std::vector<double> > BxdxBzdxydsq = applyFilterGlobally(filterMat1D,filterMatYSq1D,BxdxBzdx);
  std::vector<std::vector<double> > BxdxBzdyydsq = applyFilterGlobally(filterMat1D,filterMatYSq1D,BxdxBzdy);
  std::vector<std::vector<double> > BxBxdxSm  = applyFilterGlobally(filterMat1D,filterMat1D,BxBxdx);
  std::vector<std::vector<double> > BxBxdxxd  = applyFilterGlobally(filterMatX1D,filterMat1D,BxBxdx);
  std::vector<std::vector<double> > BxBxdxyd  = applyFilterGlobally(filterMat1D,filterMatY1D,BxBxdx);
  std::vector<std::vector<double> > BxBxdyyd  = applyFilterGlobally(filterMat1D,filterMatY1D,BxBxdy);
  std::vector<std::vector<double> > Bxdxsqxd   = applyFilterGlobally(filterMatX1D,filterMat1D,Bxdxsq);
  std::vector<std::vector<double> > Bxdxsqxdyd   = applyFilterGlobally(filterMatX1D,filterMatY1D,Bxdxsq);
  std::vector<std::vector<double> > Bxdxsqxdsq   = applyFilterGlobally(filterMatXSq1D,filterMat1D,Bxdxsq);
  std::vector<std::vector<double> > Bxdxsqydsq   = applyFilterGlobally(filterMat1D,filterMatYSq1D,Bxdxsq);
  std::vector<std::vector<double> > BxsqSm   = applyFilterGlobally(filterMat1D,filterMat1D,Bxsq);
  std::vector<std::vector<double> > BysqSm   = applyFilterGlobally(filterMat1D,filterMat1D,Bysq);
  std::vector<std::vector<double> > BxdxSqSm   = applyFilterGlobally(filterMat1D,filterMat1D,Bxdxsq);
  std::vector<std::vector<double> > ByBxdxSm  = applyFilterGlobally(filterMat1D,filterMat1D,ByBxdx);
  std::vector<std::vector<double> > ByBxdxxd  = applyFilterGlobally(filterMatX1D,filterMat1D,ByBxdx);
  std::vector<std::vector<double> > ByBxdxyd  = applyFilterGlobally(filterMat1D,filterMatY1D,ByBxdx);
  std::vector<std::vector<double> > ByBzdxxd = applyFilterGlobally(filterMatX1D,filterMat1D,ByBzdx);
  std::vector<std::vector<double> > ByBzdyyd = applyFilterGlobally(filterMat1D,filterMatY1D,ByBzdy);
  std::vector<std::vector<double> > ByBzdxyd = applyFilterGlobally(filterMat1D,filterMatY1D,ByBzdx);
  std::vector<std::vector<double> > ByBzdyxd = applyFilterGlobally(filterMatX1D,filterMat1D,ByBzdy);
  std::vector<std::vector<double> > Bxdxsqyd = applyFilterGlobally(filterMat1D,filterMatY1D,Bxdxsq);
  std::vector<std::vector<double> > BzdxBydySm =applyFilterGlobally(filterMat1D,filterMat1D,BzdxBydy);
  std::vector<std::vector<double> > BydyBzdySm =applyFilterGlobally(filterMat1D,filterMat1D,BzdyBydy);
  std::vector<std::vector<double> > BzBydySm =applyFilterGlobally(filterMat1D,filterMat1D,BzBydy);
  std::vector<std::vector<double> > BzdxBydyxd=applyFilterGlobally(filterMatX1D,filterMat1D,BzdxBydy);
  std::vector<std::vector<double> > BzdyBydyyd=applyFilterGlobally(filterMat1D,filterMatY1D,BzdyBydy);
  std::vector<std::vector<double> > BydyBzdxyd=applyFilterGlobally(filterMat1D,filterMatY1D,BydyBzdx);;
  std::vector<std::vector<double> > BydyBzdyxd=applyFilterGlobally(filterMatX1D,filterMat1D,BydyBzdy);
  std::vector<std::vector<double> > BxdxBydySm= applyFilterGlobally(filterMat1D,filterMat1D,BxdxBydy);
  std::vector<std::vector<double> > BydysqSm = applyFilterGlobally(filterMat1D,filterMat1D,Bydysq);
  std::vector<std::vector<double> > Bydysqyd = applyFilterGlobally(filterMat1D,filterMatY1D,Bydysq);
  std::vector<std::vector<double> > Bydysqydsq = applyFilterGlobally(filterMat1D,filterMatYSq1D,Bydysq);
  std::vector<std::vector<double> > BzdyBydyxd=applyFilterGlobally(filterMatX1D,filterMat1D,BzdyBydy);
  std::vector<std::vector<double> > BzBydyxd=applyFilterGlobally(filterMatX1D,filterMat1D,BzBydy);
  std::vector<std::vector<double> > BzdxBydyxdsq =applyFilterGlobally(filterMatXSq1D,filterMat1D,BzdxBydy);
  std::vector<std::vector<double> > BzBydzyd=applyFilterGlobally(filterMat1D,filterMatY1D,BzBydz);
  std::vector<std::vector<double> > BzdyBydyxdyd=applyFilterGlobally(filterMatX1D,filterMatY1D,BzdyBydy);
  std::vector<std::vector<double> > BzdxBydyxdyd =applyFilterGlobally(filterMatX1D,filterMatY1D,BzdxBydy);
  std::vector<std::vector<double> > BzdyBydyxdsq=applyFilterGlobally(filterMatXSq1D,filterMat1D,BzdyBydy);
  std::vector<std::vector<double> > BxBydySm = applyFilterGlobally(filterMat1D,filterMat1D,BxBydy);
  std::vector<std::vector<double> > BxdxBydyxd=applyFilterGlobally(filterMatX1D,filterMat1D,BxdxBydy);
  std::vector<std::vector<double> > Bydysqxd=applyFilterGlobally(filterMatX1D,filterMat1D,Bydysq);
  std::vector<std::vector<double> > BxBydyxd =applyFilterGlobally(filterMatX1D,filterMat1D,BxBydy);
  std::vector<std::vector<double> > BxdxBydyxdsq=applyFilterGlobally(filterMatXSq1D,filterMat1D,BxdxBydy);
  std::vector<std::vector<double> > Bydysqxdsq=applyFilterGlobally(filterMatXSq1D,filterMat1D,Bydysq);
  std::vector<std::vector<double> > BzdxBydyyd = applyFilterGlobally(filterMat1D,filterMatY1D,BzdxBydy);
  std::vector<std::vector<double> > BydyBzdyyd = applyFilterGlobally(filterMat1D,filterMatY1D,BydyBzdy);
  std::vector<std::vector<double> > BzBydyyd = applyFilterGlobally(filterMat1D,filterMatY1D,BzBydy);
  std::vector<std::vector<double> > BydyBzdyydsq = applyFilterGlobally(filterMat1D,filterMatYSq1D,BydyBzdy);
  std::vector<std::vector<double> > BzdxBydyydsq = applyFilterGlobally(filterMat1D,filterMatYSq1D,BzdxBydy);
  std::vector<std::vector<double> > ByBydySm = applyFilterGlobally(filterMat1D,filterMat1D,ByBydy);
  std::vector<std::vector<double> > BxdxBydyyd  = applyFilterGlobally(filterMat1D,filterMatY1D,BxdxBydy);
  std::vector<std::vector<double> > ByBydyxd  = applyFilterGlobally(filterMatX1D,filterMat1D,ByBydy);
  std::vector<std::vector<double> > BxBydyyd  = applyFilterGlobally(filterMat1D,filterMatY1D, BxBydy);
  std::vector<std::vector<double> > BxdxBydyxdyd  = applyFilterGlobally(filterMatX1D,filterMatY1D,BxdxBydy);
  std::vector<std::vector<double> > Bydysqxdyd  = applyFilterGlobally(filterMatX1D,filterMatY1D, Bydysq);
  std::vector<std::vector<double> > ByBydyyd  = applyFilterGlobally(filterMat1D,filterMatY1D,ByBydy);
  std::vector<std::vector<double> > BxdxBydyydsq = applyFilterGlobally(filterMat1D,filterMatYSq1D,BxdxBydy);
  std::vector<std::vector<double> > BzdtBydySm  = applyFilterGlobally(filterMat1D,filterMat1D,BzdtBydy);             
  std::vector<std::vector<double> > BzdtBydyxd = applyFilterGlobally(filterMatX1D,filterMat1D,BzdtBydy);             

  std::vector<std::vector<double> > BzdtBydyyd  = applyFilterGlobally(filterMat1D,filterMatY1D,BzdtBydy);
  

  /*************************************************************

   S matrix terms

   *************************************************************************/
  // S60
  std::vector<std::vector<double> > S60 = BxdxBzdxSm;sumTerms(BzdxBydySm,S60);scalarMult(-1.0,S60);
  // S61
  std::vector<std::vector<double> > S61 = BxdxBzdySm;sumTerms(BydyBzdySm,S61);scalarMult(-1.0,S61);
  // S62
  std::vector<std::vector<double> > S62 = BzBxdxSm;sumTerms(BzBydySm,S62);sumTerms(BxdxBzdxxd,S62);sumTerms(BzdxBydyxd,S62);scalarMult(-1.0,S62);
  // S63
  std::vector<std::vector<double> >  S63 = BzBxdxSm;sumTerms(BzBydySm,S63);sumTerms(BxdxBzdyyd,S63);sumTerms(BzdyBydyyd,S63);scalarMult(-1.0,S63);
  // S64
  std::vector<std::vector<double> > S64 = BxdxBzdxyd;sumTerms(BydyBzdxyd,S64);scalarMult(-1.0,S64);
  // S65
  std::vector<std::vector<double> > S65 = BxdxBzdyxd;sumTerms(BydyBzdyxd,S65);scalarMult(-1.0,S65);
  // S66
  std::vector<std::vector<double> > S66 = BxdxsqSm;sumTerms(2.0,BxdxBydySm,S66);sumTerms(BydysqSm,S66);
  // S70
  std::vector<std::vector<double> > S70 = BxBzdxSm;sumTerms(BxdxBzdxxd,S70);sumTerms(BzdxBydyxd,S70);scalarMult(-1.0,S70);
  // S71
  std::vector<std::vector<double> > S71 = BxBzdySm;sumTerms(BxdxBzdyxd,S71);sumTerms(BzdyBydyxd,S71);scalarMult(-1.0,S71);
  // S72
  std::vector<std::vector<double> > S72 = BxBzSm;sumTerms(BzBxdxxd,S72);sumTerms(BzBydyxd,S72);sumTerms(BxBzdxxd,S72);sumTerms(BxdxBzdxxdsq,S72);sumTerms(BzdxBydyxdsq,S72);scalarMult(-1.0,S72);
  // S73
  std::vector<std::vector<double> > S73 = BxBzSm;sumTerms(BzBxdxxd,S73);sumTerms(BzBydyxd,S73);sumTerms(BxBzdyyd,S73);sumTerms(BxdxBzdyxdyd,S73);sumTerms(BzdyBydyxdyd,S73);scalarMult(-1.0,S73);
  // S74
  std::vector<std::vector<double> > S74 = BxBzdxyd;sumTerms(BxdxBzdxxdyd,S74);sumTerms(BzdxBydyxdyd,S74);scalarMult(-1.0,S74);
  // S75
  std::vector<std::vector<double> > S75 = BxBzdyxd;sumTerms(BxdxBzdyxdsq,S75);sumTerms(BzdyBydyxdsq,S75);scalarMult(-1.0,S75);
  // S76
  std::vector<std::vector<double> > S76 = BxBxdxSm;sumTerms(BxBydySm,S76);sumTerms(Bxdxsqxd,S76);sumTerms(2.0,BxdxBydyxd,S76);sumTerms(Bydysqxd,S76);
  // S77
  std::vector<std::vector<double> > S77 = BxsqSm;sumTerms(2.0,BxBxdxxd,S77);sumTerms(2.0,BxBydyxd,S77);sumTerms(Bxdxsqxdsq,S77);sumTerms(2.0,BxdxBydyxdsq,S77);sumTerms(Bydysqxdsq,S77);
  // S80
  std::vector<std::vector<double> > S80 = ByBzdxSm;sumTerms(BxdxBzdxyd,S80);sumTerms(BzdxBydyyd,S80);scalarMult(-1.0,S80);
  // S81
  std::vector<std::vector<double> > S81 = ByBzdySm;sumTerms(BxdxBzdyyd,S81);sumTerms(BydyBzdyyd,S81);scalarMult(-1.0,S81);
  // S82
  std::vector<std::vector<double> > S82 = ByBzSm;sumTerms(ByBzdxxd,S82);sumTerms(BzBxdxyd,S82);sumTerms(BzBydyyd,S82);sumTerms(BxdxBzdxxdyd,S82);sumTerms(BzdxBydyxdyd,S82);scalarMult(-1.0,S82);
  // S83
  std::vector<std::vector<double> > S83 = ByBzSm;sumTerms(BzBxdxyd,S83);sumTerms(BzBydyyd,S83);sumTerms(ByBzdyyd,S83);sumTerms(BxdxBzdyydsq,S83);sumTerms(BydyBzdyydsq,S83);scalarMult(-1.0,S83);
  // S84
  std::vector<std::vector<double> > S84 = ByBzdxyd;sumTerms(BxdxBzdxydsq,S84);sumTerms(BzdxBydyydsq,S84);scalarMult(-1.0,S84);
  // S85
  std::vector<std::vector<double> > S85 = ByBzdyxd;sumTerms(BxdxBzdyxdyd,S85);sumTerms(BzdyBydyxdyd,S85);scalarMult(-1.0,S85);
  // S86
  std::vector<std::vector<double> > S86 = ByBxdxSm;sumTerms(ByBydySm,S86);sumTerms(Bxdxsqyd,S86);sumTerms(2.0,BxdxBydyyd,S86);sumTerms(Bydysqyd,S86);
  // S87
  std::vector<std::vector<double> > S87 = BxBySm;sumTerms(ByBxdxxd,S87);sumTerms(ByBydyxd,S87);sumTerms(BxBxdxyd,S87);sumTerms(BxBydyyd,S87);sumTerms(Bxdxsqxdyd,S87);sumTerms(2.0,BxdxBydyxdyd,S87);sumTerms(Bydysqxdyd,S87);
  //S88
  std::vector<std::vector<double> > S88 = BysqSm;sumTerms(2.0,ByBxdxyd,S88);sumTerms(2.0,ByBydyyd,S88);sumTerms(Bxdxsqydsq,S88); sumTerms(2.0,BxdxBydyydsq,S88);sumTerms(Bydysqydsq,S88);

  /*****************************************************************
    t term smoothings
  ******************************************************************/
  std::vector<std::vector<double> > BzdtBxdxSm = applyFilterGlobally(filterMat1D,filterMat1D,BzdtBxdx);
  std::vector<std::vector<double> > BzdtBzdxSm = applyFilterGlobally(filterMat1D,filterMat1D,BzdtBzdx);
  std::vector<std::vector<double> > BzdtBzdySm = applyFilterGlobally(filterMat1D,filterMat1D, BzdtBzdy);
  std::vector<std::vector<double> > BzBzdtSm = applyFilterGlobally(filterMat1D,filterMat1D,BzBzdt);
  std::vector<std::vector<double> > BzdtBzdxxd =  applyFilterGlobally(filterMatX1D,filterMat1D,BzdtBzdx);
  std::vector<std::vector<double> > BzdtBzdyyd = applyFilterGlobally(filterMat1D,filterMatY1D,BzdtBzdy);
  std::vector<std::vector<double> > BzdtBzdyxd =  applyFilterGlobally(filterMatX1D,filterMat1D,BzdtBzdy);
  std::vector<std::vector<double> > BzdtBzdxyd = applyFilterGlobally(filterMat1D,filterMatY1D,BzdtBzdx);
  std::vector<std::vector<double> > BxBzdtSm = applyFilterGlobally(filterMat1D,filterMat1D,BxBzdt);
  std::vector<std::vector<double> > BzdtBxdxxd = applyFilterGlobally(filterMatX1D,filterMat1D,BzdtBxdx);
  std::vector<std::vector<double> > BzdtBxdxyd = applyFilterGlobally(filterMat1D,filterMatY1D,BzdtBxdx);
  std::vector<std::vector<double> > ByBzdtSm = applyFilterGlobally(filterMat1D,filterMat1D,ByBzdt);
  std::vector<std::vector<double> > BzdtsqSm =  applyFilterGlobally(filterMat1D,filterMat1D,Bzdtsq);
     //G90
  std::vector<std::vector<double> > G90=BzdtBzdxSm;
  // G91
  std::vector<std::vector<double> >G91=BzdtBzdySm;
  // G92
  std::vector<std::vector<double> > G92=BzBzdtSm;sumTerms(BzdtBzdxxd,G92);
  // G93
  std::vector<std::vector<double> > G93=BzBzdtSm;sumTerms(BzdtBzdyyd,G93);
  // G94
  std::vector<std::vector<double> > G94=BzdtBzdxyd;
  // G95
  std::vector<std::vector<double> > G95=BzdtBzdyxd;
  //S96
  std::vector<std::vector<double> > S96=BzdtBxdxSm;sumTerms(BzdtBydySm,S96);scalarMult(-1.0,S96);
  //S97
  std::vector<std::vector<double> > S97=BxBzdtSm;sumTerms(BzdtBxdxxd,S97);sumTerms(BzdtBydyxd,S97);scalarMult(-1.0,S97);
   //S98
  std::vector<std::vector<double> > S98=ByBzdtSm;sumTerms(BzdtBxdxyd,S98);sumTerms(BzdtBydyyd,S98);scalarMult(-1.0,S98);
  // G99
  std::vector<std::vector<double> > G99 = Bzdtsq;
  

  /************************************************************

    now perform the inversion

   ************************************************************/
  std::vector<std::vector<point> > velOut(nx,std::vector<point>(ny));
  for(int i=0;i<nx;i++){
    for(int j=0;j<ny;j++){
      //the A  matrix 
      std::vector<double> amat{G00[i][j],G10[i][j],G20[i][j],G30[i][j],G40[i][j],G50[i][j],S60[i][j],S70[i][j],S80[i][j],G10[i][j],G11[i][j],G21[i][j],G31[i][j],G41[i][j],G51[i][j],S61[i][j],S71[i][j],S81[i][j],G20[i][j],G21[i][j],G22[i][j],G32[i][j],G42[i][j],G52[i][j],S62[i][j],S72[i][j],S82[i][j],G30[i][j],G31[i][j],G32[i][j],G33[i][j],G43[i][j],G53[i][j],S63[i][j],S73[i][j],S83[i][j],G40[i][j],G41[i][j],G42[i][j],G43[i][j],G44[i][j],G54[i][j],S64[i][j],S74[i][j],S84[i][j],G50[i][j],G51[i][j],G52[i][j],G53[i][j],G54[i][j],G55[i][j],S65[i][j],S75[i][j],S85[i][j],S60[i][j],S61[i][j],S62[i][j],S63[i][j],S64[i][j],S65[i][j],S66[i][j],S76[i][j],S86[i][j],S70[i][j],S71[i][j],S72[i][j],S73[i][j],S74[i][j],S75[i][j],S76[i][j],S77[i][j],S87[i][j],S80[i][j],S81[i][j],S82[i][j],S83[i][j],S84[i][j],S85[i][j],S86[i][j],S87[i][j],S88[i][j]};
      // b vector
      std::vector<double> bmat{-G90[i][j],-G91[i][j],-G92[i][j],-G93[i][j],-G94[i][j],-G95[i][j],-S96[i][j],-S97[i][j],-S98[i][j]};
      // solve Ax =b with x the velocity components
      double det = G00[i][j]+G11[i][j]+G22[i][j]+G33[i][j]+G44[i][j]+G55[i][j]+S66[i][j]+S77[i][j]+S88[i][j];
      if(det>1.0){
	int dim = 9;
	// set up params 
	int info=0;
       // one vector b here
        int nrhs=1;
	int ipiv[dim];
	//rcond recommended to use machine precision
	dgesv_(&dim,&nrhs,amat.data(),&dim,ipiv, bmat.data(),&dim,&info);
	point v(bmat[0],bmat[1],bmat[6]);
	velOut[i][j]=v;
      }else{
	point v(0.0,0.0,0.0);
	velOut[i][j]=v;
      }
    }
  }
  return velOut;
}
