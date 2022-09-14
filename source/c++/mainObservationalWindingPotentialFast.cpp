#include "interpolation2Large.h"
#include "biotSavartGauge.h"

template<typename T> std::istream &binary_read(std::istream& stream,T& value){
  return stream.read(reinterpret_cast<char*>(&value),sizeof(T));
}



int main( int argc, const char* argv[] ){
/**************************************************

Read in the files

*************************************************/
  std::ifstream vxfl;std::ifstream vyfl;std::ifstream vzfl;
  std::ifstream bxfl;std::ifstream byfl;std::ifstream bzfl;
  std::ifstream bxflPot;std::ifstream byflPot;
  int nx = std::atoi(argv[7]);
  int ny = std::atoi(argv[8]);
  double dx = std::atof(argv[9]);
  double dy = std::atof(argv[10]);
  double cutoff =std::atof(argv[11]);
  int downSampleFactor = std::atoi(argv[15]);
  std::cout<<cutoff<<"\n";
  std::vector<std::vector<double> > vx(nx,std::vector<double>(ny,0.0));
  vxfl.open(argv[1]);
  double val;std::string str;
  int indexNo=0;
  int xind=0,yind=0;
  bool allFilesPresent =true;
  std::cout << vxfl.is_open() << "\n";
  if(vxfl.is_open()){
    while(!vxfl.eof()){
      indexNo++;
      std::getline(vxfl,str);
      std::stringstream ss(str);
      ss>>val;
      if(indexNo%nx==0){
	yind++;
	xind=0;
	vx[xind][yind] = val;
      }else{
	xind++;
	vx[xind][yind] = val;
      }
    }
  }else{
    allFilesPresent=false;
    std::cout<<"invalid  vx file\n";
  }
    
  // read in vy 

  std::vector<std::vector<double> > vy(nx,std::vector<double>(ny,0.0));
  vyfl.open(argv[2]);
  indexNo=0;
  xind=0,yind=0;
  if(vyfl.is_open()){
    while(!vyfl.eof()){
      indexNo++;
      std::getline(vyfl,str);
      std::stringstream ss(str);
      ss>>val;
      if(indexNo%nx==0){
	yind++;
	xind=0;
	vy[xind][yind] = val;
      }else{
	xind++;
	vy[xind][yind] = val;
      }
    }
  }else{
    allFilesPresent=false;
    std::cout<<"invalid  vy file\n";
  }

  // read in vz
  std::vector<std::vector<double> > vz(nx,std::vector<double>(ny,0.0));
  vzfl.open(argv[3]);
  indexNo=0;
  xind=0,yind=0;
  if(vzfl.is_open()){
    while(!vzfl.eof()){
      indexNo++;
      std::getline(vzfl,str);
      std::stringstream ss(str);
      ss>>val;
      if(indexNo%nx==0){
	yind++;
	xind=0;
	vz[xind][yind] = val;
      }else{
	xind++;
	vz[xind][yind] = val;
      }
    }
  }else{
    allFilesPresent=false;
    std::cout<<"invalid  vz file\n";
  }
  
  // bx file

  std::vector<std::vector<double> > bx(nx,std::vector<double>(ny,0.0));
  bxfl.open(argv[4]);
  indexNo=0;
  xind=0,yind=0;
  if(bxfl.is_open()){
    while(!bxfl.eof()){
      indexNo++;
      std::getline(bxfl,str);
      std::stringstream ss(str);
      ss>>val;
      if(indexNo%nx==0){
	yind++;
	xind=0;
	bx[xind][yind] = val;
      }else{
	xind++;
	bx[xind][yind] = val;
      }
    }
  }else{
    allFilesPresent=false;
    std::cout<<"invalid  bx file\n";
  }

  // bx potential 
  
  std::vector<std::vector<double> > bxPot(nx,std::vector<double>(ny,0.0));
  bxflPot.open(argv[12]);
  indexNo=0;
  xind=0,yind=0;
  if(bxflPot.is_open()){
    while(!bxflPot.eof()){
      indexNo++;
      std::getline(bxflPot,str);
      std::stringstream ss(str);
      ss>>val;
      if(indexNo%nx==0){
	yind++;
	xind=0;
	bxPot[xind][yind] = val;
      }else{
	xind++;
	bxPot[xind][yind] = val;
      }
    }
  }else{
    allFilesPresent=false;
    std::cout<<"invalid  bx pot file\n";
  }

  // by file

  std::vector<std::vector<double> > by(nx,std::vector<double>(ny,0.0));
  byfl.open(argv[5]);
  indexNo=0;
  xind=0,yind=0;
  if(byfl.is_open()){
    while(!byfl.eof()){
      indexNo++;
      std::getline(byfl,str);
      std::stringstream ss(str);
      ss>>val;
      if(indexNo%nx==0){
	yind++;
	xind=0;
	by[xind][yind] = val;
      }else{
	xind++;
	by[xind][yind] = val;
      }
    }
  }else{
    allFilesPresent=false;
    std::cout<<"invalid  by file\n";
  }

  // by Potential file
  
  std::vector<std::vector<double> > byPot(nx,std::vector<double>(ny,0.0));
  byflPot.open(argv[13]);
  indexNo=0;
  xind=0,yind=0;
  if(byflPot.is_open()){
    while(!byflPot.eof()){
      indexNo++;
      std::getline(byflPot,str);
      std::stringstream ss(str);
      ss>>val;
      if(indexNo%nx==0){
	yind++;
	xind=0;
	byPot[xind][yind] = val;
      }else{
	xind++;
	byPot[xind][yind] = val;
      }
    }
  }else{
    allFilesPresent=false;
    std::cout<<"invalid  by pot file\n";
  }

  // bz file

  std::vector<std::vector<double> > bz(nx,std::vector<double>(ny,0.0));
  bzfl.open(argv[6]);
  indexNo=0;
  xind=0,yind=0;
  if(bzfl.is_open()){
    while(!bzfl.eof()){
      indexNo++;
      std::getline(bzfl,str);
      std::stringstream ss(str);
      ss>>val;
      if(indexNo%nx==0){
	yind++;
	xind=0;
	bz[xind][yind] = val;
      }else{
	xind++;
	bz[xind][yind] = val;
      }
    }
  }else{
    allFilesPresent=false;
    std::cout<<"invalid  bz file\n";
  }
  
  /*************************************************

   construct the field line velocity

   ************************************************/

  std::vector<std::vector<point> > photosphereBField;
  std::vector<std::vector<point> > photosphereVField;
  std::vector<std::vector<point> > photosphereUCurField;
  std::vector<std::vector<point> > photosphereRCurField;
  std::vector<std::vector<point> > photosphereUPotField;
  std::vector<std::vector<point> > photosphereRPotField;


  // projection

  std::cout<<"file check okay? "<<allFilesPresent<<"\n";
   
 if(allFilesPresent==true){
   for(int i=0;i<nx;i++){
     for(int j=0;j<ny;j++){
       double bsqx = bx[i][j]*bx[i][j];
       double bsqy = by[i][j]*by[i][j];
       double bsqz = bz[i][j]*bz[i][j];
       double bsq = bsqx+bsqy+bsqz;
       double VdotB = bx[i][j]*vx[i][j] + by[i][j]*vy[i][j] + bz[i][j]*vz[i][j];
       if(bsq>0.00001){
	 vx[i][j]=vx[i][j]-VdotB*bx[i][j]/bsq;
	 vy[i][j]=vy[i][j]-VdotB*by[i][j]/bsq;
	 vz[i][j]=vz[i][j]-VdotB*bz[i][j]/bsq;
       }
     }
   }
   
   // extract of the current carrying part of the
   
   std::vector<std::vector<double> > bxCur(nx,std::vector<double>(ny,0.0));
   std::vector<std::vector<double> > byCur(nx,std::vector<double>(ny,0.0));
   
   for(int i=0;i<nx;i++){
     for(int j=0;j<ny;j++){
       //std::cout<<i<<" "<<j<<" "<<nx-1<<" "<<ny-1<<" "<<bxPot[i][j]<<" "<<byPot[i][j]<<"\n"; 
       bxCur[i][j] = bx[i][j]-bxPot[i][j];
       byCur[i][j] = by[i][j]-byPot[i][j];
     }
   }
   
   
   for(int i=0;i<nx;i++){
     //std::cout<<i<<" "<<nx<<"\n";
     std::vector<point> photosphereBFieldSlice;
     std::vector<point> photosphereVFieldSlice;
     std::vector<point> photosphereUFieldCurSlice;
     std::vector<point> photosphereRFieldCurSlice;
     std::vector<point> photosphereUFieldPotSlice;
     std::vector<point> photosphereRFieldPotSlice;
     for(int j=0;j<ny;j++){
       double rvXTermCur,rvYTermCur,rvXTermPot,rvYTermPot,uXTermCur,uYTermCur,uXTermPot,uYTermPot;
       if(std::abs(bz[i][j])>0.000001){
	 rvXTermCur = vz[i][j]*bxCur[i][j]/bz[i][j];
	 rvYTermCur = vz[i][j]*byCur[i][j]/bz[i][j];
	 rvXTermPot = vz[i][j]*bxPot[i][j]/bz[i][j];
	 rvYTermPot = vz[i][j]*byPot[i][j]/bz[i][j];
	 uXTermCur = vx[i][j]- rvXTermCur;
	 uYTermCur = vy[i][j]- rvYTermCur;
	 uXTermPot = vx[i][j]- rvXTermPot;
	 uYTermPot = vy[i][j]- rvYTermPot;
       }else{
	 rvXTermCur = 0.0;rvYTermCur = 0.0;
	 rvXTermPot = 0.0;rvYTermPot = 0.0;
	 uXTermCur = 0.0;uYTermCur = 0.0;
	 uXTermPot = 0.0;uYTermPot = 0.0;
       }
       point pb(bx[i][j],by[i][j],bz[i][j]);
       point pv(vx[i][j],vy[i][j],vz[i][j]);
       point uCur(uXTermCur,uYTermCur,1.0);
       point rCur(rvXTermCur,rvYTermCur,1.0);
       point uPot(uXTermPot,uYTermPot,1.0);
       point rPot(rvXTermPot,rvYTermPot,1.0);
       photosphereBFieldSlice.push_back(pb);
       photosphereVFieldSlice.push_back(pv);
       photosphereUFieldCurSlice.push_back(uCur);
       photosphereRFieldCurSlice.push_back(rCur);
       photosphereUFieldPotSlice.push_back(uPot);
       photosphereRFieldPotSlice.push_back(rPot);
     }
     photosphereBField.push_back(photosphereBFieldSlice);
     photosphereVField.push_back(photosphereVFieldSlice);
     photosphereUCurField.push_back(photosphereUFieldCurSlice);
     photosphereRCurField.push_back(photosphereRFieldCurSlice);
     photosphereUPotField.push_back(photosphereUFieldPotSlice);
     photosphereRPotField.push_back(photosphereRFieldPotSlice);
     photosphereBFieldSlice.clear();photosphereVFieldSlice.clear();
     photosphereUFieldCurSlice.clear();photosphereUFieldPotSlice.clear();
     photosphereRFieldCurSlice.clear();photosphereRFieldPotSlice.clear();   
   }
   
   // downsample
   
   int nxSize = std::round(nx/downSampleFactor);
   int nySize = std::round(ny/downSampleFactor);
   
   double xmin =0.0;
   double ymin =0.0;
   double lx=dx*nx;
   double ly=dy*ny;
   std::vector<std::vector<double> > windingDat;
   int j;double xv,yv;
   
   std::cout<<nx<<" "<<ny<<" "<<nxSize<<" "<<nySize<<"\n";
   
#pragma omp parallel  
   {
     std::vector<std::vector<double> > windingDat_private;
     
     // start the actual parallel for (nowait allows me to bring the vectors together in order later)
     
#pragma omp for private(j,xv,yv) nowait
     for(int i=0;i<nx;i=i+downSampleFactor){
       for(j=0;j<ny;j=j+downSampleFactor){
	 
	 // biotSavartGauge is a class which has routines for calculating the winding helcity
	 
	 biotSavartGauge bsg(xmin,ymin,lx,ly,nx,ny);
	 
	 // the routine that actually caculates the winding it uses three weak field cut-offs...
	 int dsf=1;
	 std::vector<double> windVals = bsg.getWindingObsPotFast(i,j,photosphereUCurField,photosphereUPotField,photosphereBField,photosphereVField,photosphereRCurField,photosphereRPotField,cutoff,dsf);
	 
	 windingDat_private.push_back(windVals);
       }
     }
#pragma omp critical
     //here is where we recombine the vectors from individual threads
     windingDat.insert(windingDat.end(),windingDat_private.begin(),windingDat_private.end());
   }
  /**********
	     write to file
  *****************************************/
   std::ofstream outfile;
   outfile.open(argv[14]);
   double totWindCur= 0.0;
   double totHelCur =0.0;
   double totWindPot= 0.0;
   double totHelPot =0.0;
   double totWindMix= 0.0;
   double totHelMix =0.0;
   double totWindVel= 0.0;
   double totHelVel =0.0;
   double totWind =0.0;
   double totHel =0.0;
   double deltaLflux = 0.0;
   double deltaHflux = 0.0;
   double temp1 = 0.0;
   double temp2 = 0.0;
   for(int i=0;i<windingDat.size();i++){
     std::vector<double> windvals = windingDat[i];
     // the minus 1 is part of the formula.
     double windvalCur =(-1.0)*windvals[5];
     double helvalCur = (-1.0)*windvals[6];
     double windvalPot =(-1.0)*windvals[7];
     double helvalPot = (-1.0)*windvals[8];
     double windvalVelOnly = (-1.0)*windvals[9];
     double helvalVelOnly = (-1.0)*windvals[10];
     double wind =windvalCur+windvalPot-windvalVelOnly;
     double hel =helvalCur+helvalPot-helvalVelOnly;
     totWindCur=totWindCur+windvalCur;
     totHelCur=totHelCur+helvalCur;
     totWindPot=totWindPot+windvalPot;
     totHelPot=totHelPot+helvalPot;
     totWindVel=totWindVel+windvalVelOnly;
     totHelVel=totHelVel+helvalVelOnly;
     temp1 = std::abs(windvalCur)-std::abs(windvalPot);
     temp2 = std::abs(helvalCur)-std::abs(helvalPot);
     deltaLflux=deltaLflux+temp1;
     deltaHflux=deltaHflux+temp2;
     outfile<<windvals[0]<<" "<<windvals[1]<<" "<<windvals[2]<<" "<<windvals[3]<<" "<<windvals[4]<<" "<<windvalCur<<" "<<helvalCur<<" "<<windvalPot<<" "<<helvalPot<<" "<<windvalVelOnly<<" "<<helvalVelOnly<<" "<<wind<<" "<<hel<<" "<<temp1<<" "<<temp2<<"\n";
   }
   // don't forget to upscale to account for missing values
   outfile<<totWindCur*dx*dy*((nx*ny)/windingDat.size())<<"\n";
   outfile<<totHelCur*dx*dy*((nx*ny)/windingDat.size())<<"\n";
   outfile<<totWindPot*dx*dy*((nx*ny)/windingDat.size())<<"\n";
   outfile<<totHelPot*dx*dy*((nx*ny)/windingDat.size())<<"\n";
   outfile<<totWindVel*dx*dy*((nx*ny)/windingDat.size())<<"\n";
   outfile<<totHelVel*dx*dy*((nx*ny)/windingDat.size())<<"\n";
   outfile<<(totWindCur+totWindPot-totWindVel)*dx*dy*((nx*ny)/windingDat.size())<<"\n";
   outfile<<(totHelCur+totHelPot-totHelVel)*dx*dy*((nx*ny)/windingDat.size())<<"\n";
   outfile<<deltaLflux*dx*dy*((nx*ny)/windingDat.size())<<"\n";
   outfile<<deltaHflux*dx*dy*((nx*ny)/windingDat.size())<<"\n";
   outfile.close();
 }else{
   // if we are here then at least one of the magnetic field/velocity files is absent so no caculations are performed.
   std::ofstream outfile;
   outfile.open(argv[14]);
   std::cout<<"here ? "<<argv[14]<<"\n";
   outfile<<"N\n";
   for(int i=1;i<=10;i++){
     outfile<<0<<"\n";
   }
    outfile.close();
 }
}
