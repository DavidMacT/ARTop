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

double biotSavartGauge::getWindingJac(int &xi,int &xj,std::vector<std::vector<point> > vecF,std::vector<std::vector<std::pair<double,double> > > &grid, std::vector<std::vector<double> > &jacmat){
  double windRate = 0.0;
  double x1,x2,y1,y2,rsq,denom;
  x1 = grid[xi][xj].first;
  x2 = grid[xi][xj].second;
  if(std::abs(x1)<1000000.0){
    for(int i=0;i<nx;i++){
      for(int j=0;j<ny;j++){
	// in some simulations, such as the UCLA expriments we lose some of the edge points
	if(std::abs(jacmat[i][j])<1000000.0){
	  // handle the x=y case, currently just set its denisty to zero [hmmm];
	  y1 = grid[i][j].first;
	  y2 = grid[i][j].second;
	  if(i != xi && j != xj && std::abs(vecF[xi][xj].getZ())>0&& std::abs(vecF[i][j].getZ()) >0 && std::abs(y1)<1000000.0){
	    rsq = std::pow(y1-x1,2)+ std::pow(y2-x2,2);
	    if(std::abs(rsq)>0.00000001){
	      denom =((vecF[i][j].getY()/vecF[i][j].getZ())-(vecF[xi][xj].getY()/vecF[xi][xj].getZ()))*(y1-x1) -((vecF[i][j].getX()/vecF[i][j].getZ())-(vecF[xi][xj].getX()/vecF[xi][xj].getZ()))*(y2-x2);
	    }else{
	      denom = 0.0;
	    }
	    if(std::abs(denom)>0.0){
	      //std::cout<<" non zero vel ? "<< denom <<" vels "<<vecF[i][j].getX()<<" "<<vecF[i][j].getY()<<"\n";
	    }
	    windRate = windRate + dx*dy*(denom/rsq);
	  }
	}
      }
    }
    return windfrac*windRate;
  }else{
    return 0.0;    
  }
}

std::pair<double,double> biotSavartGauge::getWindingFollow(point &p,int cvIndex,int zindex,std::vector<std::vector<std::pair<point,point> > > &vecF){
  double windRate = 0.0;
  double windRateFlux = 0.0;
  double x1,x2,rsq,denom;
  // get the initial position
  x1 = p.getX();
  x2 = p.getY();
  for(int i=0;i<vecF.size();i++){
      double y1 = vecF[i][zindex].first.getX();
      double y2 = vecF[i][zindex].first.getY();
      // get y coordinates of curve, be careful to now
      if(i != cvIndex && std::abs(vecF[i][zindex].second.getZ())>0.0000000000001&& std::abs(vecF[cvIndex][zindex].second.getZ()) >0.000000000001){
	rsq = (y1-x1)*(y1-x1)+ (y2-x2)*(y2-x2);
	if(std::abs(rsq)>0.00000001){
	  denom =((vecF[i][zindex].second.getY()/vecF[i][zindex].second.getZ())-(vecF[cvIndex][zindex].second.getY()/vecF[cvIndex][zindex].second.getZ()))*(y1-x1) -((vecF[i][zindex].second.getX()/vecF[i][zindex].second.getZ())-(vecF[cvIndex][zindex].second.getX()/vecF[cvIndex][zindex].second.getZ()))*(y2-x2);
	}else{
	  denom = 0.0;
	}
	windRate = windRate + dx*dy*(denom/rsq);
	windRateFlux = windRateFlux + dx*dy*(denom/rsq)*vecF[i][0].second.getZ();
      }
  }
  std::pair<double,double> pwind;
  pwind.first = windfrac*windRate;
  pwind.second = windfrac*windRateFlux;
  return pwind;
}


std::vector<std::pair<double,double> > biotSavartGauge::getWindingFollowFullZ(std::vector<std::pair<point,point> > &curve,int cvIndex,std::vector<std::vector<std::pair<point,point> > > &vecF){
  std::vector<std::pair<double,double> > pwindVec;
  for(int l=0;l<curve.size();l++){
    double windRate = 0.0;
    double windRateFlux = 0.0;
    double x1,x2,rsq,denom;
    // get the initial position
    x1 = curve[l].first.getX();
    x2 = curve[l].first.getY();
    int zindex=l;
    for(int i=0;i<vecF.size();i++){
      double y1 = vecF[i][zindex].first.getX();
      double y2 = vecF[i][zindex].first.getY();
      // get y coordinates of curve, be careful to now
      if(i != cvIndex && std::abs(vecF[i][zindex].second.getZ())>0.0000000000001&& std::abs(vecF[cvIndex][zindex].second.getZ()) >0.000000000001){
	rsq = (y1-x1)*(y1-x1)+ (y2-x2)*(y2-x2);
	if(std::abs(rsq)>0.00000001){
	  denom =((vecF[i][zindex].second.getY()/vecF[i][zindex].second.getZ())-(vecF[cvIndex][zindex].second.getY()/vecF[cvIndex][zindex].second.getZ()))*(y1-x1) -((vecF[i][zindex].second.getX()/vecF[i][zindex].second.getZ())-(vecF[cvIndex][zindex].second.getX()/vecF[cvIndex][zindex].second.getZ()))*(y2-x2);
	}else{
	  denom = 0.0;
	}
	windRate = windRate + dx*dy*(denom/rsq);
	windRateFlux = windRateFlux + dx*dy*(denom/rsq)*vecF[i][0].second.getZ();
      }
    }
    std::pair<double,double> pwind;
    pwind.first = windfrac*windRate;
    pwind.second = windfrac*windRateFlux;
    pwindVec.push_back(pwind);
  }
  return pwindVec;
}



std::vector<std::pair<double,double> > biotSavartGauge::getTestVector(double val,int sz){
  std::vector<std::pair<double,double> > out;
  for(int i=0;i<sz;i++){
    std::pair<double,double> pr;
    pr.first=val;
    pr.second=val+1.0;
    out.push_back(pr);
  }
  return out;
};

std::pair<double,double> biotSavartGauge::getWinding(int &xi,int &xj,std::vector<std::vector<point> > vecF){
  double windRate = 0.0;double windRateFlux = 0.0;
  double x1,x2,y1,y2,rsq,denom;
  x1 = leftx + xi*dx;
  x2 = lefty + xj*dy;
  for(int i=0;i<nx;i++){
    for(int j=0;j<ny;j++){
      // handle the x=y case, currently just set its denisty to zero [hmmm];
      if(i != xi && j != xj && std::abs(vecF[i][j].getZ()>0.000000001) && std::abs(vecF[xi][xj].getZ())>0.0000000001){
      	y1 = leftx + i*dx;
       	y2 = lefty + j*dy;
        rsq = (y1-x1)*(y1-x1)+ (y2-x2)*(y2-x2);  
        if(rsq>0.000000000001){
	  denom =((vecF[i][j].getY()/vecF[i][j].getZ())-(vecF[xi][xj].getY()/vecF[xi][xj].getZ()))*(y1-x1) -((vecF[i][j].getX()/vecF[i][j].getZ())-(vecF[xi][xj].getX()/vecF[xi][xj].getZ()))*(y2-x2);
	  	windRate = windRate + dx*dy*(denom/rsq);
    	windRateFlux = windRateFlux + dx*dy*(denom/rsq)*vecF[i][j].getZ()*vecF[xi][xj].getZ();
       }
      }
    }
  }
  std::pair<double,double> pwind;
  pwind.first = windfrac*windRate;
  pwind.second = windfrac*windRateFlux;
  return pwind;
}

double biotSavartGauge::sgn(double &vl){
  if(vl<0.0){
    return -1.0;
  }else if(vl>0.0){
    return 1.0;
  }else{
    return 0.0;
  }
}

std::pair<double,double> biotSavartGauge::getWindingHI(int &xi,int &xj,std::vector<std::vector<point> > vecF){
  double windRate = 0.0;double windRateFlux = 0.0;
  double x1,x2,y1,y2,rsq,denom;
  x1 = leftx + xi*dx;
  x2 = lefty + xj*dy;
  double bxz = vecF[xi][xj].getZ();
  double sigma1,sigma2;
  if(bxz >0.0){
    sigma1=1.0;
  }else if(bxz<0.0){
    sigma1=-1.0;
  }else{
    sigma1=0.0;
  }
  //std::cout<<"in  here "<<nx<<" "<<ny<<" break\n";
  for(int i=0;i<nx;i++){
    for(int j=0;j<ny;j++){
      // handle the x=y case, currently just set its denisty to zero [hmmm];
      if(i != xi && j != xj){
      	y1 = leftx + i*dx;
       	y2 = lefty + j*dy;
        rsq = (y1-x1)*(y1-x1)+ (y2-x2)*(y2-x2);  
        if(rsq>0.000000000001){
	  denom =(vecF[i][j].getY()-vecF[xi][xj].getY())*(y1-x1) -(vecF[i][j].getX()-vecF[xi][xj].getX())*(y2-x2);
	  double byz =vecF[i][j].getZ();
	   if(byz >0.0){
	     sigma2=1.0;
	   }else if(byz<0.0){
	     sigma2=-1.0;
	   }else{
	     sigma2=0.0;
	   } 
	  windRate = windRate + dx*dy*(denom/rsq)*sigma1*sigma2;
	  windRateFlux = windRateFlux + dx*dy*(denom/rsq)*vecF[i][j].getZ()*vecF[xi][xj].getZ();
       }
      }
    }
  }
  std::pair<double,double> pwind;
  pwind.first = windfrac*windRate;
  pwind.second = windfrac*windRateFlux;
  return pwind;
}


point biotSavartGauge::getWindingNonEuclid(int &xi,int &xj,std::vector<std::vector<point> > &vecF,std::vector<std::vector<double> > &jacList){
  double windRate = 0.0;double windRateFlux = 0.0;double windRateFluxSmoo = 0.0;
  double x1,x2,y1,y2,rsq,denom;
  x1 = leftx + xi*dx;
  x2 = lefty + xj*dy;
  double bxz = vecF[xi][xj].getZ();
  double sigma1,sigma2;
  double bmagX = std::sqrt(vecF[xi][xj].getX()*vecF[xi][xj].getX()+vecF[xi][xj].getY()*vecF[xi][xj].getY()+vecF[xi][xj].getZ()*vecF[xi][xj].getZ());
  if(bxz >0.0){
    sigma1=1.0;
  }else if(bxz<0.0){
    sigma1=-1.0;
  }else{
    sigma1=0.0;
  }
  //std::cout<<"in  here "<<nx<<" "<<ny<<" break\n";
  for(int i=0;i<nx;i++){
    for(int j=0;j<ny;j++){
      // handle the x=y case, currently just set its denisty to zero [hmmm];
      if(i != xi && j != xj){
      	y1 = leftx + i*dx;
       	y2 = lefty + j*dy;
        rsq = (y1-x1)*(y1-x1)+ (y2-x2)*(y2-x2);  
        if(rsq>0.000000000001){
	  denom =(vecF[i][j].getY()-vecF[xi][xj].getY())*(y1-x1) -(vecF[i][j].getX()-vecF[xi][xj].getX())*(y2-x2);
	  double byz =vecF[i][j].getZ();
	   if(byz >0.0){
	     sigma2=1.0;
	   }else if(byz<0.0){
	     sigma2=-1.0;
	   }else{
	     sigma2=0.0;
	   } 
	  windRate = windRate + dx*dy*jacList[i][j]*(denom/rsq)*sigma1*sigma2;
	  windRateFlux = windRateFlux + dx*dy*jacList[i][j]*(denom/rsq)*vecF[i][j].getZ()*vecF[xi][xj].getZ();
    double bmagY = std::sqrt(vecF[i][j].getX()*vecF[i][j].getX()+vecF[i][j].getY()*vecF[i][j].getY()+vecF[i][j].getZ()*vecF[i][j].getZ());
     if(bmagY>0.0001 && bmagX>0.0001){
     windRateFluxSmoo = windRateFluxSmoo + dx*dy*jacList[i][j]*(denom/rsq)*vecF[i][j].getZ()*vecF[xi][xj].getZ();
      }
       }
      }
    }
  }
  point pwind;
  pwind.setX(windfrac*windRate);
  pwind.setY(windfrac*windRateFlux);
  pwind.setY(windfrac*windRateFluxSmoo);
  return pwind;
}

std::vector<double> biotSavartGauge::getWindingNonEuclidCutOff(int &xi,int &xj,std::vector<std::vector<point> > &vecF,std::vector<std::vector<point> > &bfield,std::vector<std::vector<point> > &vfield,std::vector<std::vector<point> > &Rfield,double &cut3){
  double windRate = 0.0;double windRateCut3 = 0.0;double helRate = 0.0;
  double helRateVel = 0.0;double helRateReveal = 0.0;double windRateVel=0.0;
  double windRateReveal=0.0;
  double x1,x2,y1,y2,rsq,denom,denomVel,denomReveal;
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
  //std::cout<<"in  here "<<nx<<" "<<ny<<" break\n";
  for(int i=0;i<nx;i++){
    for(int j=0;j<ny;j++){
      // handle the x=y case, currently just set its denisty to zero [hmmm];
      if(i != xi && j != xj){
      	y1 = leftx + i*dx;
       	y2 = lefty + j*dy;
        rsq = (y1-x1)*(y1-x1)+ (y2-x2)*(y2-x2);  
        if(rsq>0.000000000001){
	  denom =(vecF[i][j].getY()-vecF[xi][xj].getY())*(y1-x1) -(vecF[i][j].getX()-vecF[xi][xj].getX())*(y2-x2);
    denomVel =(vfield[i][j].getY()-vfield[xi][xj].getY())*(y1-x1) -(vfield[i][j].getX()-vfield[xi][xj].getX())*(y2-x2);
	    denomReveal =(Rfield[i][j].getY()-Rfield[xi][xj].getY())*(y1-x1) -(Rfield[i][j].getX()-Rfield[xi][xj].getX())*(y2-x2);
	  double byz =bfield[i][j].getZ();
	   if(byz >0.0){
	     sigma2=1.0;
	   }else if(byz<0.0){
	     sigma2=-1.0;
	   }else{
	     sigma2=0.0;
	   } 
	  windRate = windRate + dx*dy*(denom/rsq)*sigma1*sigma2;
    windRateVel = windRateVel + dx*dy*(denomVel/rsq)*sigma1*sigma2;
    windRateReveal = windRateReveal + dx*dy*(denomReveal/rsq)*sigma1*sigma2;
	  helRate = helRate + dx*dy*(denom/rsq)*vecF[i][j].getZ()*vecF[xi][xj].getZ();
	  helRateVel = helRateVel + dx*dy*(denomVel/rsq)*vecF[i][j].getZ()*vecF[xi][xj].getZ();
	    helRateReveal = helRateReveal + dx*dy*(denomReveal/rsq)*vecF[i][j].getZ()*vecF[xi][xj].getZ();
	  double bmagY = std::sqrt(bfield[i][j].getX()*bfield[i][j].getX()+bfield[i][j].getY()*bfield[i][j].getY()+bfield[i][j].getZ()*bfield[i][j].getZ());
     
	  if(bmagY>cut3 && bmagX>cut3){
	    windRateCut3 = windRateCut3 + dx*dy*(denom/rsq)*sigma1*sigma2;
	  }
       } 
      }
    }
  }
  std::vector<double> output;
  output.push_back(x1);
  output.push_back(x2);
  output.push_back(bfield[xi][xj].getX());
  output.push_back(bfield[xi][xj].getY());
  output.push_back(bfield[xi][xj].getZ());
  output.push_back(vfield[xi][xj].getX());
  output.push_back(vfield[xi][xj].getY());
  output.push_back(vfield[xi][xj].getZ());
  output.push_back(windfrac*windRate);
  output.push_back(windfrac*windRateCut3);
  output.push_back(windfrac*windRateVel);
  output.push_back(windfrac*windRateReveal);
  output.push_back(windfrac*helRate);
  output.push_back(windfrac*helRateVel);
  output.push_back(windfrac*helRateReveal);
  return output;
}

std::vector<double> biotSavartGauge::getWindingObsPot(int &xi,int &xj,std::vector<std::vector<point> > &vecFCur,std::vector<std::vector<point> > &vecFPot,std::vector<std::vector<point> > &bfield,std::vector<std::vector<point> > &vfield,std::vector<std::vector<point> > &RfieldCur,std::vector<std::vector<point> > &RfieldPot,double &magCutOff){
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
  for(int i=0;i<nx;i++){
    for(int j=0;j<ny;j++){
      // handle the x=y case, currently just set its denisty to zero;
      if(i != xi && j != xj){
      	y1 = leftx + i*dx;
       	y2 = lefty + j*dy;
	double bmagY = std::sqrt(bfield[i][j].getX()*bfield[i][j].getX()+bfield[i][j].getY()*bfield[i][j].getY()+bfield[i][j].getZ()*bfield[i][j].getZ());
	if(bmagY>magCutOff && bmagX>magCutOff){
	  rsq = (y1-x1)*(y1-x1)+ (y2-x2)*(y2-x2);  
	  if(rsq>0.000000000001){
	    denomCur =(vecFCur[i][j].getY()-vecFCur[xi][xj].getY())*(y1-x1) -(vecFCur[i][j].getX()-vecFCur[xi][xj].getX())*(y2-x2);
	    denomPot =(vecFPot[i][j].getY()-vecFPot[xi][xj].getY())*(y1-x1) -(vecFPot[i][j].getX()-vecFPot[xi][xj].getX())*(y2-x2);
	    denomVel =(vfield[i][j].getY()-vfield[xi][xj].getY())*(y1-x1) -(vfield[i][j].getX()-vfield[xi][xj].getX())*(y2-x2);
	    denomRevealCur=(RfieldCur[i][j].getY()-RfieldCur[xi][xj].getY())*(y1-x1) -(RfieldCur[i][j].getX()-RfieldCur[xi][xj].getX())*(y2-x2);
	    denomRevealPot=(RfieldPot[i][j].getY()-RfieldPot[xi][xj].getY())*(y1-x1) -(RfieldPot[i][j].getX()-RfieldPot[xi][xj].getX())*(y2-x2);
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
	    // reveal only wind/hel
	    revealOnlyRateCur = revealOnlyRateCur + sigProd*denomRevealCur;
	    revealOnlyRatePot = revealOnlyRatePot + sigProd*denomRevealPot;
	    revealOnlyRateHelCur = revealOnlyRateHelCur +denomRevealCur*fluxmag;
	    revealOnlyRateHelPot = revealOnlyRateHelPot +denomRevealPot*fluxmag;
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
  output.push_back(windfrac*windRateCur);
  output.push_back(windfrac*helRateCur);
  output.push_back(windfrac*windRatePot);
  output.push_back(windfrac*helRatePot);
  output.push_back(windfrac*velOnlyRate);
  output.push_back(windfrac*velOnlyRateHel);
  output.push_back(windfrac*revealOnlyRateCur);
  output.push_back(windfrac*revealOnlyRateHelCur);
  output.push_back(windfrac*revealOnlyRatePot);
  output.push_back(windfrac*revealOnlyRateHelPot);
  return output;
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
      // handle the x=y case, currently just set its denisty to zero
      if(i != xi && j != xj){
      	y1 = leftx + i*dx;
       	y2 = lefty + j*dy;
	double bmagY = std::sqrt(bfield[i][j].getX()*bfield[i][j].getX()+bfield[i][j].getY()*bfield[i][j].getY()+bfield[i][j].getZ()*bfield[i][j].getZ());
	if(bmagY>magCutOff && bmagX>magCutOff){
	  rsq = (y1-x1)*(y1-x1)+ (y2-x2)*(y2-x2);  
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
	    windRateCur = windRateCur + sigProd*denomCur;
	    windRatePot = windRatePot + sigProd*denomPot;
	    helRateCur = helRateCur + denomCur*fluxmag;
	    helRatePot = helRatePot + denomPot*fluxmag;
	    // vel only  wind/hel
	    velOnlyRate = velOnlyRate + sigProd*denomVel;
	    velOnlyRateHel = velOnlyRateHel +denomVel*fluxmag;
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
  output.push_back((vfield[xi][xj].getZ()*bfield[xi][xj].getY()-vfield[xi][xj].getY()*bfield[xi][xj].getZ())*bfield[xi][xj].getY()-(vfield[xi][xj].getX()*bfield[xi][xj].getZ()-vfield[xi][xj].getZ()*bfield[xi][xj].getX())*bfield[xi][xj].getX()); // sz
  output.push_back(windfrac*windRateCur*nx*ny/nosample);
  output.push_back(windfrac*helRateCur*nx*ny/nosample);
  output.push_back(windfrac*windRatePot*nx*ny/nosample);
  output.push_back(windfrac*helRatePot*nx*ny/nosample);
  output.push_back(windfrac*velOnlyRate*nx*ny/nosample);
  output.push_back(windfrac*velOnlyRateHel*nx*ny/nosample);
  return output;
}


std::vector<double> biotSavartGauge::getWindingObs(int &xi,int &xj,std::vector<std::vector<point> > &vecF,std::vector<std::vector<point> > &bfield,std::vector<std::vector<point> > &vfield,std::vector<std::vector<point> > &Rfield,double &magCutOff){
  double windRate = 0.0;double helRate = 0.0;double velOnlyRate = 0.0;
  double velOnlyRateHel = 0.0;double revealOnlyRate = 0.0;double revealOnlyRateHel = 0.0;
  double x1,x2,y1,y2,rsq,denom,denomVel,denomReveal;
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
  for(int i=0;i<nx;i++){
    for(int j=0;j<ny;j++){
      // handle the x=y case, currently just set its denisty to zero [hmmm];
      if(i != xi && j != xj){
      	y1 = leftx + i*dx;
       	y2 = lefty + j*dy;
	double bmagY = std::sqrt(bfield[i][j].getX()*bfield[i][j].getX()+bfield[i][j].getY()*bfield[i][j].getY()+bfield[i][j].getZ()*bfield[i][j].getZ());
	if(bmagY>magCutOff && bmagX>magCutOff){
	  rsq = (y1-x1)*(y1-x1)+ (y2-x2)*(y2-x2);  
	  if(rsq>0.000000000001){
	    denom =(vecF[i][j].getY()-vecF[xi][xj].getY())*(y1-x1) -(vecF[i][j].getX()-vecF[xi][xj].getX())*(y2-x2);
	    denomVel =(vfield[i][j].getY()-vfield[xi][xj].getY())*(y1-x1) -(vfield[i][j].getX()-vfield[xi][xj].getX())*(y2-x2);
	    denomReveal =(Rfield[i][j].getY()-Rfield[xi][xj].getY())*(y1-x1) -(Rfield[i][j].getX()-Rfield[xi][xj].getX())*(y2-x2);
	    double byz =bfield[i][j].getZ();
	    if(byz >0.0){
	      sigma2=1.0;
	    }else if(byz<0.0){
	      sigma2=-1.0;
	    }else{
	      sigma2=0.0;
	    }
	    //std::cout<<-denomReveal+denomVel-denom<<"\n"; 
	    double sigProd = dx*dy*sigma1*sigma2/rsq;
	    double fluxmag=bfield[i][j].getZ()*bfield[xi][xj].getZ()*dx*dy/rsq;
	      // full wind/hel
	    
	    windRate = windRate + sigProd*denom;
	    helRate = helRate + denom*fluxmag;
	    // vel only  wind/hel
	    velOnlyRate = velOnlyRate + sigProd*denomVel;
	    velOnlyRateHel = velOnlyRateHel +denomVel*fluxmag;
	    // reveal only wind/hel
	    revealOnlyRate = revealOnlyRate + sigProd*denomReveal;
	    revealOnlyRateHel = revealOnlyRateHel +denomReveal*fluxmag;
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
  output.push_back(windfrac*windRate);
  output.push_back(windfrac*helRate);
  output.push_back(windfrac*velOnlyRate);
  output.push_back(windfrac*velOnlyRateHel);
  output.push_back(windfrac*revealOnlyRate);
  output.push_back(windfrac*revealOnlyRateHel);
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

  
  
std::pair<double,double> biotSavartGauge::getWindingPoint(point &p,point &f,std::vector<std::vector<point> > vecF){
  double windRate = 0.0;
  double felicityRate = 0.0;
  double x1,x2,y1,y2,rsq,denom;
  x1 = p.getX();
  x2 = p.getY();
  for(int i=0;i<nx;i++){
    for(int j=0;j<ny;j++){
      // handle the x=y case, currently just set its denisty to zero [hmmm];
      y1 = leftx + i*dx;
      y2 = lefty + j*dy; 
      if(x1!= y1 && x2 != y2 && std::abs(f.getZ())>0 &&std::abs(f.getZ())>0 ){
	      rsq = std::pow(y1-x1,2)+ std::pow(y2-x2,2);
	      denom =((vecF[i][j].getY()/vecF[i][j].getZ())-(f.getY()/f.getZ()))*(y1-x1) -((vecF[i][j].getX()/vecF[i][j].getZ())-(f.getX()/f.getZ()))*(y2-x2);
	      windRate = windRate + dx*dy*weightTrap(i,j)*(denom/rsq);
        felicityRate = felicityRate+ vecF[i][j].getZ()*dx*dy*weightTrap(i,j)*(denom/rsq);
      }
    }
  }
  std::pair<double,double> windPair; 
  windPair.first = windfrac*windRate;
  windPair.second = felicityRate*windfrac;
  return windPair;
}


