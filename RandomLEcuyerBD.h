// $Id: RandomLEcuyerBD.h,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Brian Maddox - USGS MCMC SES - bmaddox@usgs.gov
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Started: 8 June 1999

// This is the L'Ecuyer random number generator with a Bays-Durham shuffle.
// The period of this function is approximately 2 x 10^18

#ifndef _RANDOMLECUYERBD_H_
#define _RANDOMLECUYERBD_H_

#include "RandomGenerator.h"


namespace MathLib
{

class RandomLEcuyerBD : public RandomGenerator
{
 public:
  RandomLEcuyerBD();
  ~RandomLEcuyerBD();

  void reset() throw();
  void reset(long int& inseed) throw();

  float getRandom() throw();

 protected:
  long int    IM1;
  long int    IM2;
  float        AM;
  long int   IMM1;
  long int    IA1;
  long int    IA2;
  long int    IQ1;
  long int    IQ2;
  int         IR1;
  int         IR2;
  int        NTAB;
  float      NDIV;
  float       EPS;
  float      RNMX;
  long int   idum;
  long int  idum2;
  long int     iy;
  long int*    iv;
};

inline
float RandomLEcuyerBD::getRandom()
  throw()
{
  int j;
  long int k;
  float temp;

  if (idum <= 0)   // initialization
  {
    if (-idum < 1) // prevent idum = 0
      idum = 1;
    else
      idum = -idum;
    
    idum2 = idum;
    
    for (j = NTAB + 7; j >= 0; j--) // load shuffle table after 8 warm-ups
    {
      k = idum / IQ1;
      idum = IA1 * (idum - k * IQ1) - k * IR1;
      if (idum < 0)
        idum += IM1;
      if (j < NTAB)
        iv[j] = idum;
    }
    
    iy = iv[0];
  }

  // normal start is here unless initializing
  k = idum / IQ1;
  idum = IA1 * (idum - k * IQ1) - k * IR1; // compute without overflows with
  if (idum < 0)                            // Schrage's method
    idum += IM1;
  k = idum2 / IQ2;
  idum2 = IA2 * (idum2 - k * IQ2) - k * IR2;
  if (idum2 < 0)
    idum2 += IM2;
  j = static_cast<int>(iy / NDIV);    // will have a domain of 0 to NTAB-1
  iy = iv[j] - idum2;    // shuffle idum and combine idum2 and idum2
  iv[j] = idum;
  if (iy < 1)
    iy += IMM1;
  if ( (temp = AM * iy) > RNMX) // don't return endpoint values
    return RNMX;
  else
    return temp;
}


} // namespace

#endif // #ifndef _RANDOMLECUYERBD_H_
