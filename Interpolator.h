// $Id: Interpolator.h,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Originally by Brian Maddox

#ifndef INTERPOLATOR_H
#define INTERPOLATOR_H

#include <vector>
#include "Point.h"
#include "MathException.h"
#include "InterpolatorTypes.h"

// This is the base interpolation class. It provides the basic interface that
// all other interpolators must follow.  Several methods here must be 
// implemented in the child classes.

namespace MathLib
{

class Interpolator
{
 public:
  Interpolator();
  Interpolator(const Point * inPoints, const int& insize);
  Interpolator(const std::vector<Point>& inPoints); 
  Interpolator(const Interpolator& interp); // deep copy
  virtual ~Interpolator();

  // These functions deal with the number of points being used for the
  // interpolation (the degree of the interpolation)
  long int getNumberPoints() const throw();
  
  // This allows the user to see exactly what type of interpolation 
  // they are using
  long int getInterpolatorType() const throw(); 

  // Allow the user to change the points used in the interpolation 
  virtual bool setPoints(const std::vector<Point>& inPoints) throw();
  
  // Overloaded setPoints takes a array of points and a size
  virtual bool setPoints(const Point * inPoints, const int& insize) throw(); 

  // Return copies of the internal Points in the class
  virtual bool getPoints(std::vector<Point>& in) const throw();
  
  bool isValid() const throw(); // to see if this interpolator is OK

  // This function must be overloaded in all child classes.  This is the 
  // function that will take the input coordinate and return the interpolated
  // one.  This function will fail if the object is not in a valid state
  // (ie, it doesn't have the vector arrays set or the error is way off base.
  virtual Point interpolatePoint(const Point& in) throw(MathException) = 0;

  // Operator overloads
  virtual bool operator==(const Interpolator& rhs) const throw();
  virtual bool operator!=(const Interpolator& rhs) const throw() = 0;

 protected:
  long int numpoints;     // number of points to use for interpolation
  Point*   Points;        // each interpolator has in common a array
                          // points to do the interpolatation over
  bool isvalid;
  long int interpolatortype;
  
};
 
// ***************************************************************************
inline long int Interpolator::getNumberPoints() const throw()
{
  return numpoints;
}

// ***************************************************************************
inline long int Interpolator::getInterpolatorType() const throw()
{
  return interpolatortype;
}

// ***************************************************************************
inline bool Interpolator::isValid() const throw()
{
  return isvalid;
}

} // namespace MathLib

#endif
