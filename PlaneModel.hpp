#pragma once

#include "DimModel.hpp"

typedef std::array<PRANSAC::NPfloat, 3> PointArray3P;

class Point3D 
     : public PRANSAC::DimSpace
{
public:
	Point3D(PRANSAC::NPdouble x, PRANSAC::NPdouble y, PRANSAC::NPdouble z)
	    PointArray3P n_Point3D; 
	{
		n_Point3D[0] = x;
		n_Point3D[1] = y;
		n_Point3D[2] = z;
	}
};

class PlaneModel 
	: public PRANSAC::Plane3D<3>
{
  protected:
  	// Parametric form
  	PRANSAC::NPdouble n_a, n_b, n_c;
  	PRANSAC::NPdouble n_DistDenominator;

  	
}