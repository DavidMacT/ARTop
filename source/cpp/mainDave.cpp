#include "dave4VM.h"
#include<iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <chrono>

using namespace std::chrono;

void readDataFileIn(std::ifstream &file,std::vector<std::vector<double> > &fieldVec,int &nx,int &ny,bool &checkRead){
 if(file.is_open()){
   int indexNo=0;
   int xind=0,yind=0;
   double val;std::string str;
    while(!file.eof()){
      indexNo++;
      std::getline(file,str);
      if(str.size()>0){
      std::stringstream ss(str);
      ss>>val;	
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

argv[1] is the location of the files (e.g. AR_377_Output/Data/)
argv[2] is the field tag e.g. 377
argv[3] nx the number of x pixels
argv[4] ny the nuber of y pixels


 ******************************/

int main( int argc, const char* argv[] ){

  std::ifstream bxfl;std::ifstream byfl;std::ifstream bzfl;
  
  // locations for the field data
  std::string fieldLoc =argv[1];
  std::string fieldTag =argv[2];
  int window = std::atoi(argv[7]);
  std::string bxLoc =fieldLoc+"/bx_"+fieldTag+"_";
  std::string byLoc =fieldLoc+"/by_"+fieldTag+"_";
  std::string bzLoc =fieldLoc+"/bz_"+fieldTag+"_";
  std::string vxLoc =fieldLoc+"/Ux_"+fieldTag+"_"+argv[7]+"_";
  std::string vyLoc =fieldLoc+"/Uy_"+fieldTag+"_"+argv[7]+"_";
  std::string vzLoc =fieldLoc+"/Uz_"+fieldTag+"_"+argv[7]+"_";

  // read in the grid sizes nx and ny 
  int nx = std::atoi(argv[3]);
  int ny = std::atoi(argv[4]);

  // file range to read in
  int startIndex = std::atoi(argv[5]);
  int endIndex = std::atoi(argv[6]);
  
  bool checkRead = true;
 
    // Get duration. Substart timepoints to
    // get duration. To cast it to proper unit
    // use duration cast method

  /*************************************************************

       main loop

   ************************************************************/
  for(int findex = startIndex;findex<=endIndex;findex++){
     std::vector<std::vector<double> > bxminus(nx,std::vector<double>(ny,0.0));
     std::vector<std::vector<double> > byminus(nx,std::vector<double>(ny,0.0));
     std::vector<std::vector<double> > bzminus(nx,std::vector<double>(ny,0.0));
     
     std::vector<std::vector<double> > bxplus(nx,std::vector<double>(ny,0.0));
     std::vector<std::vector<double> > byplus(nx,std::vector<double>(ny,0.0));
     std::vector<std::vector<double> > bzplus(nx,std::vector<double>(ny,0.0));
     
     std::vector<std::vector<double> > bx(nx,std::vector<double>(ny,0.0));
     std::vector<std::vector<double> > by(nx,std::vector<double>(ny,0.0));
     std::vector<std::vector<double> > bz(nx,std::vector<double>(ny,0.0));
     
     std::vector<std::vector<double> > bxt(nx,std::vector<double>(ny,0.0));
     std::vector<std::vector<double> > byt(nx,std::vector<double>(ny,0.0));
     std::vector<std::vector<double> > bzt(nx,std::vector<double>(ny,0.0));
     
     bool checkRead = true;
     std::cout<<"making velocity step "<<findex<<" from "<<startIndex<<" to "<<endIndex<<"\n";
     std::string bxLocCurrMinus = bxLoc;
     bxLocCurrMinus  += std::to_string(findex);
     bxLocCurrMinus  +=".txt";
     bxfl.open(bxLocCurrMinus);
     readDataFileIn(bxfl,bxminus,nx,ny,checkRead);
     bxfl.close();
     std::string byLocCurrMinus = byLoc;
     byLocCurrMinus  += std::to_string(findex);
     byLocCurrMinus  +=".txt";
     byfl.open(byLocCurrMinus);
     readDataFileIn(byfl,byminus,nx,ny,checkRead);
     byfl.close();
     std::string bzLocCurrMinus = bzLoc;
     bzLocCurrMinus  += std::to_string(findex);
     bzLocCurrMinus  +=".txt";
     bzfl.open(bzLocCurrMinus);
     readDataFileIn(bzfl,bzminus,nx,ny,checkRead);
     bzfl.close();
     int findex2 =findex+1;
     std::string bxLocCurrPlus = bxLoc;
     bxLocCurrPlus  += std::to_string(findex2);
     bxLocCurrPlus  +=".txt";
     bxfl.open(bxLocCurrPlus);
     readDataFileIn(bxfl,bxplus,nx,ny,checkRead);
     bxfl.close();
     std::string byLocCurrPlus = byLoc;
     byLocCurrPlus  += std::to_string(findex2);
     byLocCurrPlus  +=".txt";
     byfl.open(byLocCurrPlus);
     readDataFileIn(byfl,byplus,nx,ny,checkRead);
     byfl.close();
     std::string bzLocCurrPlus = bzLoc;
     bzLocCurrPlus  += std::to_string(findex2);
     bzLocCurrPlus  +=".txt";
     bzfl.open(bzLocCurrPlus);
     readDataFileIn(bzfl,bzplus,nx,ny,checkRead);
     bzfl.close();
     
     // We take the mean of these for the field used and the difference for the temporal derivatioevs

     if(checkRead==true){
       double dtinv = 1.0/720.0;
       for(int i=0;i<nx;i++){
	 for(int j=0;j<ny;j++){
	   bx[i][j] = 0.5*(bxplus[i][j]+bxminus[i][j]);
	   by[i][j] = 0.5*(byplus[i][j]+byminus[i][j]);
	   bz[i][j] = 0.5*(bzplus[i][j]+bzminus[i][j]);
	   bxt[i][j] = dtinv*(bxplus[i][j]-bxminus[i][j]);
	   byt[i][j] = dtinv*(byplus[i][j]-byminus[i][j]);
	   bzt[i][j] = dtinv*(bzplus[i][j]-bzminus[i][j]);
	 }
       }
     
     
       double dx =360;double dy =360;
       dave4VM dtest(dx,dy,nx,ny);
       std::vector<std::vector<point> >  velocityOut;
       dtest.getDerivatives(bx,by,bz,dx,dy);
       dtest.weightFunctions(window);
       dtest.setMatrixTermsPreFilter(bxt,byt,bzt);
       velocityOut = dtest.composeMatrixAndInvert();
       std::ofstream outputVx;
       std::ofstream outputVy;
       std::ofstream outputVz;
       
       std::string vxLocCurr = vxLoc;
       vxLocCurr  += std::to_string(findex);
       vxLocCurr  +=".txt";
       std::string vyLocCurr = vyLoc;
       vyLocCurr  += std::to_string(findex);
       vyLocCurr  +=".txt";
       std::string vzLocCurr = vzLoc;
       vzLocCurr  += std::to_string(findex);
       vzLocCurr +=".txt";
       
       outputVx.open(vxLocCurr);
       outputVy.open(vyLocCurr);
       outputVz.open(vzLocCurr);
       
       for(int j=0;j<velocityOut[0].size();j++){
	 for(int i=0;i<velocityOut.size();i++){
	   outputVx<<velocityOut[i][j].getX()<<"\n";
	   outputVy<<velocityOut[i][j].getY()<<"\n";
	   outputVz<<velocityOut[i][j].getZ()<<"\n";
	 }
       }
       outputVx.close();outputVy.close();outputVz.close();
     }
  }
}
  
