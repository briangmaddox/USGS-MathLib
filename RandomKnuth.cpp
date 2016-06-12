// $Id: RandomKnuth.cpp,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Brian Maddox - USGS MCMC SES - bmaddox@usgs.gov
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Started: 10 June 1999
// Implementation file for the Knuth generator

#ifndef _RANDOMKNUTH_CPP_
#define _RANDOMKNUTH_CPP_

#include "RandomKnuth.h"

#include <time.h>
#include <stdlib.h>

MathLib::RandomKnuth::RandomKnuth()
{
  MBIG = 1000000000;
  MSEED = 161803398;
  MZ = 0;
  FAC = 1.0f / MBIG;

  reset();
}

// **************************************************************************
void MathLib::RandomKnuth::reset()
  throw()
{
  srand(time(0));
  idum = rand();

  iff = 0;

  // Initialization is assumed if idum is negative so do the flop here
  if (idum < 0)
    idum = -idum;
}


// **************************************************************************
void MathLib::RandomKnuth::reset(long int& inseed)
  throw()
{
  if (inseed > 0)
    idum = -inseed;
  else
    idum = inseed;

  iff = 0;
}


#endif
