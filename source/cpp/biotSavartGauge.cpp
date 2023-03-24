#include "biotSavartGauge.h"

biotSavartGauge::biotSavartGauge(double &x0,double &y0,double &lenx,double &leny,int &nxIn,int &nyIn){
  d1= lenx;d2=leny;leftx=x0;lefty=y0;
  nx = nxIn;ny=nyIn;
  dx = d1/double(nx-1);
  dy = d2/double(ny-1);
  windfrac = 0.15915494309;
}

double biotSavartGauge::weightTrap(int &i,int &j){
  if((i==0 && j==0) || (i==0 && j== ny-1) || (i== nx-1 && j==ny-1) || (i==nx-1 && j== 0)){
    return 0.25;
  }else if(i==0 || j==0 || j== ny-1 || i==nx-1){
    return 0.5;
  }else{
    return 1.0;
  }
}



std::vector<double> biotSavartGauge::getWindingObsPotFast(int &xi,int &xj,std::vector<std::vector<point> > &vecFCur,std::vector<std::vector<point> > &vecFPot,std::vector<std::vector<point> > &bfield,std::vector<std::vector<point> > &vfield,std::vector<std::vector<point> > &RfieldCur,std::vector<std::vector<point> > &RfieldPot,double &magCutOff,int &downsampleFAC){
  double windRateCur = 0.0;double helRateCur = 0.0;double velOnlyRate = 0.0;
  double windRatePot = 0.0;double helRatePot = 0.0;
  double windRateMix = 0.0;double helRateMix = 0.0;
  double velOnlyRateHel = 0.0;
  double revealOnlyRateCur = 0.0;double revealOnlyRateHelCur = 0.0;
  double revealOnlyRatePot = 0.0;double revealOnlyRateHelPot = 0.0;
  double revealOnlyRateMix = 0.0;double revealOnlyRateHelMix = 0.0;
  double x1,x2,y1,y2,rsq,denomCur,denomPot,denomCurPot,denomPotCur,denomVel,denomRevealCur,denomRevealPot,denomRevealPotCur,denomRevealCurPot;
  x1 = leftx + xi*dx;
  x2 = lefty + xj*dy;
  double bxz = bfield[xi][xj].getZ();
  double sigma1,sigma2;
  double bmagX = std::sqrt(bfield[xi][xj].getX()*bfield[xi][xj].getX()+bfield[xi][xj].getY()*bfield[xi][xj].getY()+bfield[xi][xj].getZ()*bfield[xi][xj].getZ());
  if(bxz >0.0){
    sigma1=1.0;
  }else if(bxz<0.0){
    sigma1=-1.0;
  }else{
    sigma1=0.0;
  }
  //std::cout<<"co is "<<magCutOff<<"\n";
  //std::cout<<"in  here "<<nx<<" "<<ny<<" break\n";
  int nosample =0;
  for(int i=0;i<nx;i=i+downsampleFAC){
    for(int j=0;j<ny;j=j+downsampleFAC){
      nosample++;
      // handle the x=y case, currently just set its denisty to zero [hmmm];
      if(i != xi && j != xj){
      	y1 = leftx + i*dx;
       	y2 = lefty + j*dy;
	double bmagY = std::sqrt(bfield[i][j].getX()*bfield[i][j].getX()+bfield[i][j].getY()*bfield[i][j].getY()+bfield[i][j].getZ()*bfield[i][j].getZ());
	//std::cout<<i<<" "<<j<<" "<<bfield[i][j].getZ()<<" "<<bmagY<<" "<<bmagX<<" "<<magCutOff<<"\n";
	if(bmagY>magCutOff && bmagX>magCutOff){
	  rsq = (y1-x1)*(y1-x1)+ (y2-x2)*(y2-x2);
	  //std::cout<<rsq<<"\n";
	  if(rsq>0.000000000001){
	    denomCur =(vecFCur[i][j].getY()-vecFCur[xi][xj].getY())*(y1-x1) -(vecFCur[i][j].getX()-vecFCur[xi][xj].getX())*(y2-x2);
	    denomPot =(vecFPot[i][j].getY()-vecFPot[xi][xj].getY())*(y1-x1) -(vecFPot[i][j].getX()-vecFPot[xi][xj].getX())*(y2-x2);
	    denomVel =(vfield[i][j].getY()-vfield[xi][xj].getY())*(y1-x1) -(vfield[i][j].getX()-vfield[xi][xj].getX())*(y2-x2);
	    double byz =bfield[i][j].getZ();
	    if(byz >0.0){
	      sigma2=1.0;
	    }else if(byz<0.0){
	      sigma2=-1.0;
	    }else{
	      sigma2=0.0;
	    }
	    double sigProd = dx*dy*sigma1*sigma2/rsq;
	    double fluxmag=bfield[i][j].getZ()*bfield[xi][xj].getZ()*dx*dy/rsq;
	      // full wind/hel
	    //std::cout<<"dnom reveal pot is "<<denomRevealPot<<" "<<RfieldPot[i][j].getY()<<" "<<RfieldPot[xi][xj].getY()<<" "<<RfieldPot[i][j].getX()<<" "<<RfieldPot[xi][xj].getX()<<"\n";
	    windRateCur = windRateCur + sigProd*denomCur;
	    windRatePot = windRatePot + sigProd*denomPot;
	    helRateCur = helRateCur + denomCur*fluxmag;
	    helRatePot = helRatePot + denomPot*fluxmag;
	    // vel only  wind/hel
	    velOnlyRate = velOnlyRate + sigProd*denomVel;
	    velOnlyRateHel = velOnlyRateHel +denomVel*fluxmag;
	    // reveal only wind/he
	  }
	} 
      }
    }
  }
  std::vector<double> output;
  output.push_back(x1);
  output.push_back(x2);
  output.push_back(bfield[xi][xj].getZ());
  output.push_back(vfield[xi][xj].getZ());
  output.push_back(windfrac*windRateCur*nx*ny/nosample);
  output.push_back(windfrac*helRateCur*nx*ny/nosample);
  output.push_back(windfrac*windRatePot*nx*ny/nosample);
  output.push_back(windfrac*helRatePot*nx*ny/nosample);
  output.push_back(windfrac*velOnlyRate*nx*ny/nosample);
  output.push_back(windfrac*velOnlyRateHel*nx*ny/nosample);
  return output;
}

std::vector<double> biotSavartGauge::getWindingObsPotFastPreCalc(int &xi,int &xj,std::vector<std::vector<std::vector<double> > > coordDifs, std::vector<std::vector<double> > bmagCVec,std::vector<std::vector<point> > &vecFCur,std::vector<std::vector<point> > &vecFPot,std::vector<std::vector<point> > &bfield,std::vector<std::vector<point> > &vfield,std::vector<std::vector<point> > &RfieldCur,std::vector<std::vector<point> > &RfieldPot,double &magCutOff,int &downsampleFAC){
  double windRateCur = 0.0;double helRateCur = 0.0;double velOnlyRate = 0.0;
  double windRatePot = 0.0;double helRatePot = 0.0;
  double windRateMix = 0.0;double helRateMix = 0.0;
  double velOnlyRateHel = 0.0;
  double revealOnlyRateCur = 0.0;double revealOnlyRateHelCur = 0.0;
  double revealOnlyRatePot = 0.0;double revealOnlyRateHelPot = 0.0;
  double revealOnlyRateMix = 0.0;double revealOnlyRateHelMix = 0.0;
  double x1,x2,denomCur,denomPot,denomCurPot,denomPotCur,denomVel,denomRevealCur,denomRevealPot,denomRevealPotCur,denomRevealCurPot;
  int di,dj;
  double direcFaci=1.0;
  double direcFacj=1.0;
  x1 = leftx + xi*dx;
  x2 = lefty + xj*dy;
  double bxz = bfield[xi][xj].getZ();
  double sigma1,sigma2;
  double bmagX = bmagCVec[xi][xj];
  if(bxz >0.0){
    sigma1=1.0;
  }else if(bxz<0.0){
    sigma1=-1.0;
  }else{
    sigma1=0.0;
  }
  //std::cout<<"co is "<<magCutOff<<"\n";
  //std::cout<<"in  here "<<nx<<" "<<ny<<" break\n";
  int nosample =0;
  for(int i=0;i<nx;i=i+downsampleFAC){
    for(int j=0;j<ny;j=j+downsampleFAC){
      nosample++;
      // handle the x=y case, currently just set its denisty to zero [hmmm];
      if(i != xi && j != xj){
	double bmagY =  bmagCVec[i][j];
	int di = xi-i;
	int dj = xj-j;
	if(di<0){
	  direcFaci=-1.0;
	  di= i-xi;
	}else{
	  direcFacj=1.0;
	}
	if(dj<0){
	  direcFacj=-1.0;
	  dj = j-xj;
	}else{
	  direcFacj=1.0;
	}
	if(bmagY>magCutOff && bmagX>magCutOff && coordDifs[di][dj][2]>0.000000000001){
	  denomCur =(vecFCur[i][j].getY()-vecFCur[xi][xj].getY())*direcFaci*coordDifs[di][dj][0] -(vecFCur[i][j].getX()-vecFCur[xi][xj].getX())*direcFacj*coordDifs[di][dj][1];
	  denomPot =(vecFPot[i][j].getY()-vecFPot[xi][xj].getY())*direcFaci*coordDifs[i][j][0] -(vecFPot[i][j].getX()-vecFPot[xi][xj].getX())*direcFacj*coordDifs[i][j][1];
	  denomVel =(vfield[i][j].getY()-vfield[xi][xj].getY())*direcFaci*coordDifs[i][j][0] -(vfield[i][j].getX()-vfield[xi][xj].getX())*direcFacj*coordDifs[i][j][1];
	  double byz =bfield[i][j].getZ();
	    if(byz >0.0){
	      sigma2=1.0;
	    }else if(byz<0.0){
	      sigma2=-1.0;
	    }else{
	      sigma2=0.0;
	    }
	    double sigProd = sigma1*sigma2*dx*dy;
	    double fluxmag=bfield[i][j].getZ()*bfield[xi][xj].getZ()*dx*dy;
	    // full wind/hel
	    //std::cout<<"dnom reveal pot is "<<denomRevealPot<<" "<<RfieldPot[i][j].getY()<<" "<<RfieldPot[xi][xj].getY()<<" "<<RfieldPot[i][j].getX()<<" "<<RfieldPot[xi][xj].getX()<<"\n";
	    windRateCur = windRateCur + sigProd*denomCur;
	    windRatePot = windRatePot + sigProd*denomPot;
	    helRateCur = helRateCur + denomCur*fluxmag;
	    helRatePot = helRatePot + denomPot*fluxmag;
	  // vel only  wind/hel
	    velOnlyRate = velOnlyRate + sigProd*denomVel;
	  velOnlyRateHel = velOnlyRateHel +denomVel*fluxmag;
	  // reveal only wind/he
	}
      }
    }
  }
  std::vector<double> output;
  output.push_back(x1);
  output.push_back(x2);
  output.push_back(bfield[xi][xj].getZ());
  output.push_back(vfield[xi][xj].getZ());
  output.push_back((vfield[xi][xj].getZ()*bfield[xi][xj].getY()-vfield[xi][xj].getY()*bfield[xi][xj].getZ())*bfield[xi][xj].getY()-(vfield[xi][xj].getX()*bfield[xi][xj].getZ()-vfield[xi][xj].getZ()*bfield[xi][xj].getX())*bfield[xi][xj].getX()); // sz
  output.push_back(windfrac*windRateCur*nx*ny/nosample);
  output.push_back(windfrac*helRateCur*nx*ny/nosample);
  output.push_back(windfrac*windRatePot*nx*ny/nosample);
  output.push_back(windfrac*helRatePot*nx*ny/nosample);
  output.push_back(windfrac*velOnlyRate*nx*ny/nosample);
  output.push_back(windfrac*velOnlyRateHel*nx*ny/nosample);
  return output;
}

point biotSavartGauge::getBiotSavartGauge(int &xi,int &xj,std::vector<std::vector<point> > vecF){
  double Ax=0.0;double Ay=0.0;double Az=0.0;
  double x1,x2,y1,y2,rsq,denom;
  x1 = leftx + xi*dx;
  x2 = lefty + xj*dy;
  for(int i=0;i<nx;i++){
    for(int j=0;j<ny;j++){
      // handle the x=y case, currently just set its denisty to zero [hmmm];
      if(i != xi && j != xj){
      	y1 = leftx + i*dx;
       	y2 = lefty + j*dy;
	rsq = (y1-x1)*(y1-x1)+ (y2-x2)*(y2-x2);
	if(std::abs(rsq)>0.00000001){
	  Ax =Ax - dx*dy*weightTrap(i,j)*(vecF[i][j].getZ()*(x2-y2))/rsq;
	  Ay =Ay + dx*dy*weightTrap(i,j)*(vecF[i][j].getZ()*(x1-y1))/rsq;
	  Az =Az + dx*dy*weightTrap(i,j)*(vecF[i][j].getX()*(x2-y2)-vecF[i][j].getY()*(x1-y1))/rsq;
	}

      }
    }
  }
  point pout(Ax,Ay,Az);
  return pout;
}

  
 
