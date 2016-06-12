// $Id: PolynomialInterpolator.cpp,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Originally by Brian Maddox

#ifndef _POLYNOMIALINTERPOLATOR_CPP_
#define _POLYNOMIALINTERPOLATOR_CPP_

#include "PolynomialInterpolator.h"
#include <vector>
#include <math.h>
#include <iostream>

using namespace MathLib;

// **************************************************************************
PolynomialInterpolator::PolynomialInterpolator()
: Interpolator(), lasterror(0)
{
  interpolatortype = 1;
}


// **************************************************************************
PolynomialInterpolator::PolynomialInterpolator(const PolynomialInterpolator& rhs)
  : Interpolator(rhs)
{
  lasterror = rhs.lasterror;
  interpolatortype = 1;
}
                                               
// **************************************************************************
PolynomialInterpolator::~PolynomialInterpolator()
{
}


// **************************************************************************
Point PolynomialInterpolator::interpolatePoint(const Point& in) 
  throw(MathException)
{
  long int loop1, loop2, index = 1;
  double den, difference, t_difference, ho, hp, w;
  double *c = NULL;
  double *d = NULL;
  double retval;
  Point temp;
  try
  {
    if (numpoints == 0) // have to have points to interpolate with
      throw MathException();

    difference = fabs(in.x - Points[1].x);
  
    // Find the difference between the input
    // value and the first value in the array

    // create our working arrays to the number of points to be used
    if (!(c = new (std::nothrow) double[numpoints +1]))
      throw std::bad_alloc();
    if (!(d = new (std::nothrow) double[numpoints +1]))
      throw std::bad_alloc();
   
    for (loop1 = 1; loop1 <= numpoints; loop1++)
    {
      t_difference = fabs(in.x - Points[loop1].x);

      if ( t_difference < difference)
      {
        index = loop1;
        difference = t_difference;
      }

      // Initialize the table of c and d values
      c[loop1] = d[loop1] = Points[loop1].y;
    }

    retval = Points[index--].y; // set the initial approximation of the return
                                // value

    // For each column of the table, loop over the current c and d values and
    // update them.
    for (loop2 = 1; loop2 < numpoints; loop2++)
    {
      for (loop1 = 1; loop1 <= (numpoints - loop2); loop1++)
      {
        ho = Points[loop1].x - in.x;
        hp = Points[loop1+loop2].x - in.x;
        w = c[loop1 + 1] - d[loop1];

        // Check for error.  This can only occur if two x values in the vector
        // are identical within the floating point error
        if ( (den = ho - hp) == 0.0)
          throw MathException();

        // No go and update the c and d values
        den = w / den;
        d[loop1] = hp * den;
        c[loop1] = ho * den;
      }

      // After each column is completed, we decide which correction, c or d, 
      // we want to add to our value of y (which path to take through the 
      // table.  We do this in such a way to take the most straight line route
      // through the table to the apex, updating the index accordingly to keep
      // track of where we are.  This route keeps partial approximations
      // centered on the target x.  The last error added is then the error
      // indication

      retval += (lasterror = ((2* index) < (numpoints - loop2) ? c[index + 1] :
                                                                 d[index--]));
    }

    // clear out our arrays
    delete [] c;
    delete [] d;
    
    temp.x = in.x;
    temp.y = retval;
    return temp;
  }
  catch (...)
  {
    delete [] c;
    delete [] d;
    throw MathException();
   
  }
}

// ***************************************************************************
bool PolynomialInterpolator::operator==(const Interpolator& rhs)
  const throw()
{
  const PolynomialInterpolator* trhs;

  try
  {
    trhs = dynamic_cast<const PolynomialInterpolator*>(&rhs);

    if (trhs != NULL)
      if (lasterror == trhs->getError())
        return Interpolator::operator==(rhs);

    return false;
  }
  catch (...)
  {
    return false;
  }
}

// ***************************************************************************
bool PolynomialInterpolator::operator!=(const Interpolator& rhs) const throw()
{
  return (!PolynomialInterpolator::operator!=(rhs));
}


#endif
