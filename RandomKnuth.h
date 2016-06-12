// $Id: RandomKnuth.h,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Brian Maddox - USGS MCMC SES - bmaddox@usgs.gov
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Started: 10 June 1999

// This is a portable random number generator suggested by Knuth.  It uses a
// subtractive method instead of a linear congruential method to perform its
// calculations

#ifndef _RANDOMKNUTH_H_
#define _RANDOMKNUTH_H_

#include "RandomGenerator.h"

namespace MathLib
{

class RandomKnuth : public RandomGenerator
{
 public:
  RandomKnuth();
  ~RandomKnuth() {};

  void reset() throw();
  void reset(long int& inseed) throw();

  float getRandom() throw();

 protected:
  long int MBIG;
  long int MSEED;
  long int MZ;
  float    FAC;

  int      inext, inextp;
  long int ma[56];
  int      iff;
  long int idum;
};

inline 
float RandomKnuth::getRandom()
  throw()
{
  long int mj, mk;
  int i, ii, k;

  // Initialization phase
  if ( (idum < 0) || (iff == 0) )
  {
    iff = 1;
    mj = MSEED - (idum < 0 ? -idum : idum);
    mj %= MBIG;
    ma[55] = mj;
    mk = 1;
    
    for (i = 1; i < 55; i++)
    {
      ii = (21 * i) % 55;
      ma[ii] = mk;
      mk = mj - mk;
      if (mk < MZ) 
        mk += MBIG;
      mj = ma[ii];
    }

    for (k = 1; k < 5; k++)
      for (i = 1; i < 56; i++)
      {
        ma[i] -= ma[1 + (i + 30) % 55];
        if (ma[i] < MZ)
          ma[i] += MBIG;
      }

    inext = 0;
    inextp = 31;
    idum = 1;
  }

  if (++inext == 56)
    inext = 1;
  if (++inextp == 56)
    inextp = 1;
  mj = ma[inext] - ma[inextp];
  if (mj < MZ)
    mj += MBIG;
  ma[inext] = mj;
  return (mj * FAC);
}

} // namespace 

#endif // #ifndef _RANDOMKNUTH_H_
