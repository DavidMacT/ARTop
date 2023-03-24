#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <fstream>
#include <sstream>


int main( int argc, const char* argv[] )
{
  std::ifstream infile;
  std::ofstream outfile;
  infile.open(argv[1]);
  outfile.open(argv[2]);
  std::string str;
  double val;
  double deltaLflux,deltaHflux;
  if(infile.is_open()){
     while(!infile.eof()){
      std::getline(infile,str);
      std::stringstream ss(str);
      std::vector<double> vals;
      // read in a line of data.
      while(ss>>val){
	ss.ignore(1);
	vals.push_back(val);
      }
      if(vals.size()==14){
	deltaLflux = std::abs(vals[4])-std::abs(vals[6]);
	deltaHflux = std::abs(vals[5])-std::abs(vals[7]);
	outfile<<vals[0]<<" "<<vals[1]<<" "<<vals[2]<<" "<<vals[3]<<" "<<vals[4]<<" "<<vals[5]<<" "<<vals[6]<<" "<<vals[7]<<" "<<vals[8]<<" "<<vals[9]<<" "<<vals[10]<<" "<<vals[11]<<" "<<deltaLflux<<" "<<deltaHflux<<"\n";
      }else if(vals.size()==15){
	deltaLflux = std::abs(vals[5])-std::abs(vals[7]);
	deltaHflux = std::abs(vals[6])-std::abs(vals[8]);
	outfile<<vals[0]<<" "<<vals[1]<<" "<<vals[2]<<" "<<vals[3]<<" "<<vals[4]<<" "<<vals[5]<<" "<<vals[6]<<" "<<vals[7]<<" "<<vals[8]<<" "<<vals[9]<<" "<<vals[10]<<" "<<vals[11]<<" "<<vals[12]<<" "<<deltaLflux<<" "<<deltaHflux<<"\n";
      }else if(vals.size()==1){
	outfile<<vals[0]<<"\n";
      }
     }
  }else{
    std::cout<<"input file invalid\n";
  }
  infile.close();
  outfile.close();
}
