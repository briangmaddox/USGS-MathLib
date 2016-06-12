// $Id: RandomParkMillerBD.h,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Brian Maddox - USGS MCMC SES - bmaddox@usgs.gov
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Started: 8 June 1999

// This is the Park & Miller random number generator with a Bays-Durham
// shuffle applied to the values.  The original code comes from page 280
// of the second edition of "Numerical Recipes in C".


#ifndef _RANDOMPARKMILLERBD_H_
#define _RANDOMPARKMILLERBD_H_

#include "RandomGenerator.h"

namespace MathLib
{

class RandomParkMillerBD : public RandomGenerator
{
 public:
  RandomParkMillerBD();
  ~RandomParkMillerBD();

  void reset() throw();                 // reset via random number
  void reset(long int& inseed) throw(); // reset via user passed value

  float getRandom() throw();

 private:
  int        IA;
  long int   IM;
  float      AM;
  long int   IQ;
  int        IR;
  int      NTAB;
  float    NDIV;
  float     EPS;
  float    RNMX;
  int         j;
  int         k;
  long int   iy;
  long int*  iv;
  float    temp;
  long int idum;
};

inline
float RandomParkMillerBD::getRandom()
  throw()
{
  if ( (idum <= 0) || (!iy) ) // must be in the initialization sequence
  {
    if (-idum < 1)  // prevent idum = 0;
      idum = 1;
    else
      idum = -idum;
    
    for (j = NTAB + 7; j >= 0; j--) // load the shuffle table after 8 warm ups
    {
      k = idum / IQ;
      idum = IA * (idum - k * IQ) - IR * k;
      if (idum < 0)
        idum += IM;
      if (j < NTAB)
        iv[j] = idum;
    }

    iy = iv[0];
  }

  // Non initialization begin
  k = idum / IQ;
  idum = IA * (idum - k * IQ) - IR * k;
  if (idum < 0)
    idum += IM;
  j = static_cast<int>(iy / NDIV);
  iy = iv[j];
  iv[j] = idum;
  if ( (temp = AM * iy) > RNMX)
    return RNMX;
  else
    return temp;
}
    

} // namespace MathLib

#endif // #ifndef _RANDOMPARKMILLERBD_H_
