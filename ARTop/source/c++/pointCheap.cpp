#include "point.h"

point::point(double x,double y,double z){
      X =x;
      Y =y;
      Z =z;                                  
}

point::point(){};


double point::getX(){
      return X;       
}

double point::getY(){
      return Y;       
}

double point::getZ(){
       return Z;
}

void point::setX(double xv){
      X=xv;       
}

void point::setY(double yv){
      Y=yv;       
}

void point::setZ(double zv){
      Z=zv;
}


point point::operator+(point p){
  point pout;
  pout.setX(X + p.getX());
  pout.setY(Y + p.getY());
  pout.setZ(Z + p.getZ());
  return pout;
  }

point point::operator-(point p){
  point pout;
  pout.setX(X - p.getX());
  pout.setY(Y - p.getY());
  pout.setZ(Z - p.getZ());
  return pout;
}

point point::operator*(double d){
  point pout;
  pout.setX(d*X);
  pout.setY(d*Y);
  pout.setZ(d*Z);
  return pout;
}

point point::operator/(double d){
  point pout;
  pout.setX(X/d);
  pout.setY(Y/d);
  pout.setZ(Z/d);
  return pout;
};
