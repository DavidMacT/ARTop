#include "interpolation2Large.h"
#include "biotSavartGauge.h"

template<typename T> std::istream &binary_read(std::istream& stream,T& value){
  return stream.read(reinterpret_cast<char*>(&value),sizeof(T));
}

// readDataFileIn is a general routine to read in the magnetic field and veliocity fields.

void readDataFileIn(std::ifstream &file,std::vector<std::vector<double> > &fieldVec,int &nx,int &ny,bool &checkRead){
 if(file.is_open()){
   int indexNo=0;
   int xind=0,yind=0;
    double val;std::string str;
    while(!file.eof()){
      indexNo++;
      std::getline(file,str);
      //std::cout<<str<<" "<<str.length()<<"\n";
      if(str.length()>0){
	std::stringstream ss(str);
	ss>>val;
	//std::cout<<val<<"\n";
	if(indexNo%nx==0){
	  yind++;
	  xind=0;
	  if(yind<ny){
	    fieldVec[xind][yind] = val;
	  }
	}else{
	  fieldVec[xind][yind] = val;
	  xind++;
	}
      }
    }
  }else{
   checkRead= false;
  }
}


/*******************************

Components of argv[], the parameters which are read in

argv[1] is the location of the input files
argv[2] is the field tag e.g. 377
argv[3] is the velocity smoothing tag, e.g 12 or 20 (for reading in velocity files)
argv[4] is the start field snapshot number (usually 0)
argv[5] is the end field snapshot number 
argv[6] and argv[7] are respectively, nx and ny the field pixel numbers]
argv[8] and argv[9] are respectively, dx and dy the grid spacing
argv[10] is the field cut off CO (e.g 50)
argv[11] is the down sample factor DS (e.g 3)
argv[12] is the output file format e.g. "AR_377_Output/Data/windDatPotentialFast"




 ******************************/


int main( int argc, const char* argv[] ){
/**************************************************

Read in the files

*************************************************/
  // initiate the streams for each data file
  std::ifstream vxfl;std::ifstream vyfl;std::ifstream vzfl;
  std::ifstream bxfl;std::ifstream byfl;std::ifstream bzfl;
  std::ifstream bxPotfl;std::ifstream byPotfl;
  
  // read in the minimum and maximum field snapshot indicies
  int fv1 = std::atoi(argv[4]);
  int fv2 = std::atoi(argv[5]);

  // read in the grid sizes nx and ny 
  int nx = std::atoi(argv[6]);
  int ny = std::atoi(argv[7]);

  // read in the grid spacings dx,dy
  double dx = std::atof(argv[8]);
  double dy = std::atof(argv[9]);
  double xmin =0.0;
  double ymin =0.0;
  double lx=dx*nx;
  double ly=dy*ny;;
  
  // read in the magnetic field cut-off, the parameter CO
  double cutoff =std::atof(argv[10]);

  // read in the down sampling factor, the parameter d set the down sample nx's based on this
  int downSampleFactor = std::atoi(argv[11]);
  int nxSize = std::round(nx/downSampleFactor);
  int nySize = std::round(ny/downSampleFactor);
  // read in the output file location tag
  std::string outputFile= argv[12];

    //declare the main winding vector
  int j;double xv,yv;
  
    
  // Set the field location
  std::string fieldLoc =argv[1];
  std::string fieldTag =argv[2];
  std::string velTag = argv[3];  
  std::string vxLoc =fieldLoc+"/Ux_"+fieldTag+"_"+velTag+"_";
  std::string vyLoc =fieldLoc+"/Uy_"+fieldTag+"_"+velTag+"_";
  std::string vzLoc =fieldLoc+"/Uz_"+fieldTag+"_"+velTag+"_";
  std::string bxLoc =fieldLoc+"/bx_"+fieldTag+"_";
  std::string byLoc =fieldLoc+"/by_"+fieldTag+"_";
  std::string bzLoc =fieldLoc+"/bz_"+fieldTag+"_";
  std::string bxPotLoc =fieldLoc+"/Bxp_"+fieldTag+"_";
  std::string byPotLoc =fieldLoc+"/Byp_"+fieldTag+"_";

   /*************************************************




               The main loop






   ************************************************/
  
  for(int findex=fv1;findex<=fv2;findex++){
    std::cout<<"Topology calculation  "<<findex<<" of "<<fv2<<"\n";

    //declare the vectors for the field variables:
    std::vector<std::vector<double> > vx(nx,std::vector<double>(ny,0.0));
    std::vector<std::vector<double> > vy(nx,std::vector<double>(ny,0.0));
    std::vector<std::vector<double> > vz(nx,std::vector<double>(ny,0.0));
    std::vector<std::vector<double> > bx(nx,std::vector<double>(ny,0.0));
    std::vector<std::vector<double> > by(nx,std::vector<double>(ny,0.0));
    std::vector<std::vector<double> > bz(nx,std::vector<double>(ny,0.0));
    std::vector<std::vector<double> > bxPot(nx,std::vector<double>(ny,0.0));
    std::vector<std::vector<double> > byPot(nx,std::vector<double>(ny,0.0));
    
    
    // declare the vectors for the field line velocities
    std::vector<std::vector<point> > photosphereBField;
    std::vector<std::vector<point> > photosphereVField;
    std::vector<std::vector<point> > photosphereUCurField;
    std::vector<std::vector<point> > photosphereRCurField;
    std::vector<std::vector<point> > photosphereUPotField;
    std::vector<std::vector<point> > photosphereRPotField;
   
    
    // read in the data
    bool checkRead =true;
    std::string vxLocCurr = vxLoc;
    vxLocCurr  += std::to_string(findex);
    vxLocCurr  +=".txt";
    vxfl.open(vxLocCurr);
    //std::cout<<vxLocCurr<<"\n";
    readDataFileIn(vxfl,vx,nx,ny,checkRead);
    vxfl.close();
    std::string vyLocCurr = vyLoc;
    vyLocCurr  += std::to_string(findex);
    vyLocCurr  +=".txt";
    vyfl.open(vyLocCurr);
    readDataFileIn(vyfl,vy,nx,ny,checkRead);
    vyfl.close();
    std::string vzLocCurr = vzLoc;
    vzLocCurr  += std::to_string(findex);
    vzLocCurr  +=".txt";
    vzfl.open(vzLocCurr);
    readDataFileIn(vzfl,vz,nx,ny,checkRead);
    vzfl.close();
    std::string bxLocCurr = bxLoc;
    bxLocCurr  += std::to_string(findex);
    bxLocCurr  +=".txt";
    bxfl.open(bxLocCurr);
    //std::cout<<bxLocCurr<<"\n";
    readDataFileIn(bxfl,bx,nx,ny,checkRead);
    bxfl.close();
    std::string byLocCurr = byLoc;
    byLocCurr  += std::to_string(findex);
    byLocCurr  +=".txt";
    byfl.open(byLocCurr);
    readDataFileIn(byfl,by,nx,ny,checkRead);
    byfl.close();
    std::string bzLocCurr = bzLoc;
    bzLocCurr  += std::to_string(findex);
    bzLocCurr  +=".txt";
    bzfl.open(bzLocCurr);
    readDataFileIn(bzfl,bz,nx,ny,checkRead);
    bzfl.close();
    std::string bxPotLocCurr = bxPotLoc;
    bxPotLocCurr  += std::to_string(findex);
    bxPotLocCurr  +=".txt";
    bxPotfl.open(bxPotLocCurr);
    readDataFileIn(bxPotfl,bxPot,nx,ny,checkRead);
    bxPotfl.close();
    std::string byPotLocCurr = byPotLoc;
    byPotLocCurr  += std::to_string(findex);
    byPotLocCurr  +=".txt";
    byPotfl.open(byPotLocCurr);
    readDataFileIn(byPotfl,byPot,nx,ny,checkRead);
    byPotfl.close();

    //std::cout<<"check fields "<<bx[100][100]<<" "<<by[100][100]<<" "<<bz[100][100]<<" "<<vx[100][100]<<" "<<vy[100][100]<<" "<<vz[100][100]<<"\n";

    //
    if(checkRead==true){
      
      /*************************************************

     apply the line of sight correction to the velocities

      ************************************************/
      
      // we have to calculate |B|^2 here and it is used later in the main winding routine
      //so we store the mag values here

  
      for(int i=0;i<nx;i++){
	for(int j=0;j<ny;j++){
	  double bsqx = bx[i][j]*bx[i][j];
	  double bsqy = by[i][j]*by[i][j];
	  double bsqz = bz[i][j]*bz[i][j];
	  double bsq = bsqx+bsqy+bsqz;
	  if(bsq>0.00001){
	    double vdotb = vx[i][j]*bx[i][j]+vy[i][j]*by[i][j]+vz[i][j]*bz[i][j];
	    vx[i][j]= vx[i][j] - (vdotb)*bx[i][j]/bsq;
	    vy[i][j]= vy[i][j] - (vdotb)*by[i][j]/bsq;
	    vz[i][j]= vz[i][j] - (vdotb)*bz[i][j]/bsq;
	  }
	}
      }
      
      /*************************************************

      Extract of the ``current carrying" part of the magnetogram data.

      ************************************************/
      
       // declare the current carrying field vectors 
      std::vector<std::vector<double> > bxCur(nx,std::vector<double>(ny,0.0));
      std::vector<std::vector<double> > byCur(nx,std::vector<double>(ny,0.0));
      
      for(int i=0;i<nx;i++){
	for(int j=0;j<ny;j++){
	  bxCur[i][j] = bx[i][j]-bxPot[i][j];
	  byCur[i][j] = by[i][j]-byPot[i][j];
	}
      }
      
      /*************************************************

     Calculate all the relevant field line velocities

      ************************************************/
      
      for(int i=0;i<nx;i++){
	//std::cout<<i<<" "<<nx<<" "<<bz[i][j]<<"\n";
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
      
      /******************************************************************************

      Calculate the topological quantities

      *****************************************************************************/
      // the main vector for the winding densities 
       std::vector<std::vector<double> > windingDat;


#pragma omp parallel  
      {
	std::vector<std::vector<double> > windingDat_private;
	
	// start the actual parallel for (nowait allows me to bring the vectors together in order later)
	
#pragma omp for private(j,xv,yv) nowait
      for(int i=0;i<nx;i=i+downSampleFactor){
	for(j=0;j<ny;j=j+downSampleFactor){
	  
	  // biotSavartGauge is a class which has routines for calculating the winding helcity
	  //std::cout<<"test openmp "<<i<<" "<<j<<"\n";
	  biotSavartGauge bsg(xmin,ymin,lx,ly,nx,ny);
	  
	  // the routine that actually caculates the winding it uses three weak field cut-offs...
	  int dsf;
	  /*if(downSampleFactor==1){
	    dsf=1;
	  }else{
	    dsf=downSampleFactor-1;
	    }*/
	  dsf =1;
	  std::vector<double> windVals = bsg.getWindingObsPotFast(i,j,photosphereUCurField,photosphereUPotField,photosphereBField,photosphereVField,photosphereRCurField,photosphereRPotField,cutoff,dsf);
   	  windingDat_private.push_back(windVals);
	  //std::cout<<windVals[7]<<"\n";
	}
      }
#pragma omp critical
      //here is where we recombine the vectors from individual threads
      windingDat.insert(windingDat.end(),windingDat_private.begin(),windingDat_private.end());
    }
      
    /***********************************************


                 write to file

    ***********************************************/
      std::ofstream outfile;
      std::string outputFileCurr = outputFile+"CO"+std::to_string(int(cutoff))+"_VS"+velTag+"_"+ std::to_string(downSampleFactor)+"_"+std::to_string(findex)+".dat";
      outfile.open(outputFileCurr);
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
      for(int i=0;i<windingDat.size();i++){
	std::vector<double> windvals = windingDat[i];
	// the minus 1 is part of the formula.
	double windvalCur =(-1.0)*windvals[4];
	double helvalCur = (-1.0)*windvals[5];
	double windvalPot =(-1.0)*windvals[6];
	double helvalPot = (-1.0)*windvals[7];
	double windvalVelOnly = (-1.0)*windvals[8];
	double helvalVelOnly = (-1.0)*windvals[9];
	double wind =windvalCur+windvalPot-windvalVelOnly;
	double hel =helvalCur+helvalPot-helvalVelOnly;
	totWindCur=totWindCur+windvalCur;
	totHelCur=totHelCur+helvalCur;
	totWindPot=totWindPot+windvalPot;
	totHelPot=totHelPot+helvalPot;
	totWindVel=totWindVel+windvalVelOnly;
	totHelVel=totHelVel+helvalVelOnly;
	double deltaLfluxden = (std::abs(windvalCur)-std::abs(windvalPot));
	double deltaHfluxden = (std::abs(helvalCur)-std::abs(helvalPot));
	deltaLflux=deltaLflux+deltaLfluxden;
	deltaHflux=deltaHflux+deltaHfluxden;
	outfile<<windvals[0]<<" "<<windvals[1]<<" "<<windvals[2]<<" "<<windvals[3]<<" "<<windvals[4]<<" "<<windvalCur<<" "<<helvalCur<<" "<<windvalPot<<" "<<helvalPot<<" "<<windvalVelOnly<<" "<<helvalVelOnly<<" "<<wind<<" "<<hel<<" "<<deltaLfluxden<<" "<<deltaHfluxden<<"\n";
     }
      // don't forget to upscale to account for missing values
      double fac = double(nx*ny)/double(windingDat.size());
      outfile<<totWindCur*dx*dy*fac<<"\n";
      outfile<<totHelCur*dx*dy*fac<<"\n";
      outfile<<totWindPot*dx*dy*fac<<"\n";
      outfile<<totHelPot*dx*dy*fac<<"\n";
      outfile<<totWindVel*dx*dy*fac<<"\n";
      outfile<<totHelVel*dx*dy*fac<<"\n";
      outfile<<(totWindCur+totWindPot-totWindVel)*dx*dy*fac<<"\n";
      outfile<<(totHelCur+totHelPot-totHelVel)*dx*dy*fac<<"\n";
      outfile<<deltaLflux*dx*dy*fac<<"\n";
      outfile<<deltaHflux*dx*dy*fac<<"\n";
      outfile.close();
    }else{
      // if we are here then at least one of the magnetic field/velocity files is absent so no calculations are performed.
      std::ofstream outfile;
      std::string outputFileCurr = outputFile+"CO"+std::to_string(int(cutoff))+"_VS"+velTag+"_"+ std::to_string(int(downSampleFactor))+"_"+std::to_string(findex)+".dat";
      outfile.open(outputFileCurr);
      outfile<<"N\n";
      for(int i=1;i<=10;i++){
	outfile<<0<<"\n";
      }
      outfile.close();
   }
  }
}
