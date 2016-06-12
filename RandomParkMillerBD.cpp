// $Id: RandomParkMillerBD.cpp,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Brian Maddox - USGS MCMC SES - bmaddox@usgs.gov
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Started: 8 June 1999

// Implementation file for the RandomParkMillerBD class

#ifndef _RANDOMPARKMILLERBD_CPP_
#define _RANDOMPARKMILLERBD_CPP_

#include <stdlib.h>
#include <time.h>
#include <new>
#include "RandomParkMillerBD.h"

MathLib::RandomParkMillerBD::RandomParkMillerBD()
{
  IA = 16807;
  IM = 2147483647;
  AM = 1.0 / IM;
  IQ = 127773;
  IR = 2836;
  NTAB = 32;
  NDIV = 1 + (IM - 1) / NTAB;
  EPS = 1.2e-7f;
  RNMX = 1.0 - EPS;
  
  if (!(iv = new(std::nothrow) long int[NTAB]))
    throw std::bad_alloc();

  reset();
}

// ***************************************************************************
MathLib::RandomParkMillerBD::~RandomParkMillerBD()
{
  delete [] iv;
}


// ***************************************************************************
void MathLib::RandomParkMillerBD::reset()
  throw()
{
  long int foo;

  iy = 0;

  srand(time(0));
  foo = rand();

  // If we're calling reset we want to reinitialize the random number 
  // sequence.  The Bays-Durham shuffle routine requires that the initializer
  // be negative to start a new sequence.  Therefore, this should set a 
  // negative value
  if (foo > 0)
    foo = -foo;

  idum = foo;
}


// ***************************************************************************
void MathLib::RandomParkMillerBD::reset(long int& inseed)
  throw()
{
  // Same thing as above.  If the user is calling this, they want to
  // reinitialize the random sequence.  Therefore, if the number passed in
  // isn't a negative number, make it so

  iy = 0;
  if (inseed)
    idum = -inseed;
  else
    idum = inseed;
}


#endif
