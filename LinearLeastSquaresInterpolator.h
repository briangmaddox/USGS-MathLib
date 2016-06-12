// $Id: LinearLeastSquaresInterpolator.h,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Originally by Christopher Bilderback

#ifndef _LINEARLEASTSQUARESINTERPOLATOR_H_
#define _LINEARLEASTSQUARESINTERPOLATOR_H_

// This class is a implemention of the linear least squares interpolation.  
// The interface for this class should be consistent with the rest of the
// interpolator classes

#include "Interpolator.h"

namespace MathLib
{

class LinearLeastSquaresInterpolator : public Interpolator
{
 public: 
  LinearLeastSquaresInterpolator();
  LinearLeastSquaresInterpolator(const LinearLeastSquaresInterpolator& rhs);
  virtual ~LinearLeastSquaresInterpolator() {}
  
  // Allow the user to change the points used in the interpolation 
  bool setPoints(const std::vector<Point>& inPoints) throw();
  
  // Overloaded setPoints takes a array of points and a size
  bool setPoints(const Point* inPoints, const int& insize) throw(); 
 
  // Interpolate the input point.  Note that this function will fail if the
  // input points are not set or if for some reason the setup function did not
  // complete successfully.
  Point interpolatePoint(const Point& in) throw(MathException);
  
  //Lets you see what the slope and intersept is for the line
  double getSlope()                        const throw();
  double getIntercept()                    const throw();


  // Operator overrides.
  bool operator==(const Interpolator& rhs) const throw();
  bool operator!=(const Interpolator& rhs) const throw();

 protected:
  //the coefficients for the linear 
  double Slope, Intercept;
};

// ***************************************************************************
inline
double LinearLeastSquaresInterpolator::getSlope() const throw()
{
  return Slope;
}

// ***************************************************************************
inline
double LinearLeastSquaresInterpolator::getIntercept()  const throw()
{
  return Intercept;
}


}
#endif




