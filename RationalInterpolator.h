// $Id: RationalInterpolator.h,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Originally by Brian Maddox

#ifndef _RATIONALINTERPOLATOR_H_
#define _RATIONALINTERPOLATOR_H_

#include "Interpolator.h"

// This is a rational function interpolator.  It also originates from the 
// "Numerical Recipies in C" book.

namespace MathLib
{

class RationalInterpolator : public Interpolator
{
 public:
  RationalInterpolator();
  RationalInterpolator(const RationalInterpolator& interp);
  ~RationalInterpolator() {};

  Point interpolatePoint(const Point& in) throw(MathException);
  double getError() const throw(); // returns the error of the last
                                   // operation

  // Operator overloads
  bool operator==(const Interpolator& rhs) const throw();
  bool operator!=(const Interpolator& rhs) const throw();

 protected:
   double lasterror; // amount of error from the previous operation
};

// ***************************************************************************
inline double RationalInterpolator::getError() const throw()
{
  return lasterror;
}

} // namespace MathLib

#endif // #ifndef _RATIONALINTERPOLATOR_H_
