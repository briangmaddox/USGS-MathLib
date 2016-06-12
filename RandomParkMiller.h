// $Id: RandomParkMiller.h,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Brian Maddox - USGS MCMC SES - bmaddox@usgs.gov
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Started: 8 June 1999

// This class performs a Park & Miller random number generation.  The code is
// from page 279 of the second edition of "Numerical Recipies in C" by
// Press et. al.
// This random number generator has a period of approx 2.1 x 10^9
#ifndef _RANDOMPARKMILLER_H_
#define _RANDOMPARKMILLER_H_

#include "RandomGenerator.h"

namespace MathLib
{

class RandomParkMiller : public RandomGenerator
{
 public:
  RandomParkMiller();
  ~RandomParkMiller() {};

  void reset() throw();
  void reset(long int& inseed) throw(); // reset to user supplied value

  float getRandom() throw();

 private:
  int        IA;
  long int   IM;
  float      AM;
  long int   IQ;
  int        IR;
  long int MASK;
  long int    k;
  float     ans;
  long int idum;
};


inline
float RandomParkMiller::getRandom()
  throw()
{
  idum ^= MASK;
  k = idum / IQ;
  idum = IA * (idum - k * IQ) - IR * k;
  if (idum < 0)
    idum += IM;
  ans = AM * idum;
  idum ^= MASK;
  return ans;
}

} // namespace MathLib

#endif // #ifndef _RANDOMPARKMILLER_H_


