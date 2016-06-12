// $Id: RandomLEcuyerBD.cpp,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Brian Maddox - USGS MCMC SES - bmaddox@usgs.gov
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Started: 10 June 1999

// Implementation for the RandomLEcuyerBD class

#ifndef _RANDOMLECUYERBD_CPP_
#define _RANDOMLECUYERBD_CPP_

#include <new>
#include <stdlib.h>
#include <time.h>
#include "RandomLEcuyerBD.h"

MathLib::RandomLEcuyerBD::RandomLEcuyerBD()
{
  IM1   = 2147483563;
  IM2   = 2147483399;
  AM    = 1.0 / IM1;
  IMM1  = IM1 - 1;
  IA1   = 40014;
  IA2   = 40692;
  IQ1   = 53668;
  IQ2   = 52774;
  IR1   = 12211;
  IR2   = 3791;
  NTAB  = 32;
  NDIV  = (1 + IMM1) / NTAB;
  EPS   = 1.2e-7f;
  RNMX  = 1.0 - EPS;

  if (!(iv = new(std::nothrow) long int[NTAB]))
    throw std::bad_alloc();

  reset(); // do an initial setting
}

// **************************************************************************
MathLib::RandomLEcuyerBD::~RandomLEcuyerBD()
{
  delete [] iv;
}


// **************************************************************************
void MathLib::RandomLEcuyerBD::reset()
  throw()
{
  srand(time(0)); // initialize the C lib's random number seed
  idum = rand();

  iy = 0;
  idum2 = 123456789;

  // If we're calling this we want to reinitialize the random number sequence.
  // The added safeguards here require idum to be a negative value on
  // init so we check for this here.
  if (idum > 0)
    idum = -idum;
}


// **************************************************************************
void MathLib::RandomLEcuyerBD::reset(long int& input)
  throw()
{
  if (input > 0)
    idum = -input;
  else
    idum = input;

  iy = 0;
  
  idum2 = 123456789;
}


#endif
