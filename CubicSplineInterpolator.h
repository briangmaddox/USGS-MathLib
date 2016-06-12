// $Id: CubicSplineInterpolator.h,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Originally by Brian Maddox

#ifndef _CUBICSPLINEINTERPOLATOR_H_
#define _CUBICSPLINEINTERPOLATOR_H_

// This class is an implementation of cubic spline interpolation.  This 
// function differs slightly from the others in that the user has a choice of
// selecting a natural cubic spline (with second derivatives with value zero at
// one of both of its boundaries) or a normal cubic spline which has defined
// second derivatives at the boundaries.  The default here is to assume a
// natural cubic spine.

#include "Interpolator.h"

namespace MathLib
{

class CubicSplineInterpolator : public Interpolator
{
public:
  CubicSplineInterpolator();
  CubicSplineInterpolator(const CubicSplineInterpolator& rhs);
  virtual ~CubicSplineInterpolator();

  // This is an override of the default function.  This function will call the
  // base's setPoints function and then will call the setupInterpolators
  // function.  Note that the default operation of this class is to assume a
  // natural cubic spline
  bool setPoints(const std::vector<Point>& inPoints) throw();
  
  // Second override of the default function.  This function will call the 
  // base's setPoints function and then will call the  setupInterpolators
  // function.  Note that the default operation of this class is to assume a
  // natural cubic spline
  bool setPoints(const Point * inPoints, const int& insize) throw(); 
  
  // This function is a convenience function.  It sets the second derivatives
  // of both of the boundary points to zero and calls the setup function.
  void setNatural() throw(MathException);

  // This function allows the user to set the second derivatives of the 
  // boundary points.  It also calls the setup function
  void setBoundaries(const double& infirst, const double& inn) 
    throw(MathException);

  // Return the first derivatives of the boundary points.
  void getBoundaryDerivatives(double& infirst, double& innth) const throw();

  // Return the vector of second derivatives in the object
  bool getSecondDerivatives(std::vector<double>& inv) const throw();

  // Interpolate the input point.  Note that this function will fail if the
  // input points are not set or if for some reason the setup function did not
  // complete successfully.
  Point interpolatePoint(const Point& in) throw(MathException);

  // Operator overrides.
  bool operator==(const Interpolator& rhs) const throw();
  bool operator!=(const Interpolator& rhs) const throw();

  // To tell whether or not this is a natural cubic spline
  bool isNatural() const throw();

 protected:
  // This function is called when the user sets the type of spline.  It sets up
  // the vector of second derivatives that interpolatePoint needs to function.
  void setupInterpolator() throw(MathException);

  double first, nth; // First derivatives of the boundary points
  bool   isnatural;
  double * secders;
  int    numsecders; // the number of second ders
};

// ***************************************************************************
inline void CubicSplineInterpolator::getBoundaryDerivatives(double& infirst,
                                                            double& innth) 
  const throw()
{
  infirst = first;
  innth = nth;
}

// ***************************************************************************
inline bool CubicSplineInterpolator::isNatural() const throw()
{
  return isnatural;
}

} // namespace MathLib

#endif // #ifndef _CUBICSPLINEINTERPOLATOR_H_
