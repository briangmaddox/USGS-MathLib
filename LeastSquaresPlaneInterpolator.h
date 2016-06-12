// $Id: LeastSquaresPlaneInterpolator.h,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Originally by Christopher Bilderback

#ifndef _LEASTSQUARESPLANEINTERPOLATOR_H_
#define _LEASTSQUARESPLANEINTERPOLATOR_H_

// This is a Least Squares plane interpolator which is 
// different from a BiLinearLeastSquares interpolator in
// that it actually creates a planar equation 

#include "Interpolator.h"




namespace MathLib
{

class LeastSquaresPlaneInterpolator : public Interpolator
{
 public: 
  LeastSquaresPlaneInterpolator();
  LeastSquaresPlaneInterpolator(const LeastSquaresPlaneInterpolator& rhs);
  virtual ~LeastSquaresPlaneInterpolator() {};
   
  // These two functions setup the interpolation 
  // they accept a grid of unique 3 dimesional points (x, y, z)
  // in any order (probably should have at least four points)
  bool setPoints(const std::vector<Point>& inPoints)       throw();
  bool setPoints(const Point* inPoints, const int& insize) throw(); 

  // Return copies of the internal Points in the class
  bool getPoints(std::vector<Point>& in) const throw();
  
  Point interpolatePoint(const Point& in) throw(MathException);

  // Operator overloads
  bool operator==(const Interpolator& rhs) const throw();
  bool operator!=(const Interpolator& rhs) const throw();

protected:

  double xcoef, ycoef, dvalue; //the planer equation
};

}

#endif


