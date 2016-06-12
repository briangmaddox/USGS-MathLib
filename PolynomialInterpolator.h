// $Id: PolynomialInterpolator.h,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Originally by Brian Maddox

#ifndef _POLYNOMIALINTERPOLATOR_H_
#define _POLYNOMIALINTERPOLATOR_H_

#include "Interpolator.h"

// This class performs a polynomial interpolation on the input value.  This
// routine is based on the polynomial interpolation found in "Numerical 
// Recipies in C" 2nd edition.  If unable to interpolate the point, an
// exception will be thrown to notify the user.  The scheme used here is to
// track the differences between levels of Neville's algorithm.

namespace MathLib
{

class PolynomialInterpolator : public Interpolator  
{
public:
  PolynomialInterpolator();
  PolynomialInterpolator(const PolynomialInterpolator& rhs);
  virtual ~PolynomialInterpolator();

  Point interpolatePoint(const Point& in) throw(MathException);
  double getError() const throw(); // returns the error associated with the
                                   // last operation

  // Operator overloads
  virtual bool operator==(const Interpolator& rhs) const throw();
  virtual bool operator!=(const Interpolator& rhs) const throw();

protected:
  double lasterror;
};

// **************************************************************************
inline double PolynomialInterpolator::getError() const throw()
{
  return lasterror;
}

} // namespace

#endif // #ifndef _POLYNOMIALINTERPOLATOR_H_
