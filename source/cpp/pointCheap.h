#ifndef PNT_H
#define PNT_H
#include<iostream>
#include<fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <vector>


class point{
      public:
            point(double x,double y,double z);
	    point();
            double getX();
            double getY();
            double getZ();
            void setX(double xv);
            void setY(double yv);
            void setZ(double zv);
	     //point operator+(point &p);
	    point operator+(point p);
	    // point operator-(point &p);
	    point operator-(point p);
	    //point operator*(point &p,double d);
	    //point operator*(double d,point &p);
	    point operator/(double d);
	    point operator*(double d);
      private:
            double X,Y,Z,norm;
};

#endif
