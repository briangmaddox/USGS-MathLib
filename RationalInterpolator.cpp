// $Id: RationalInterpolator.cpp,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Originally by Brian Maddox

#include <math.h>
#include "RationalInterpolator.h"

using namespace MathLib;

// ***************************************************************************
RationalInterpolator::RationalInterpolator()
  : Interpolator()
{
  interpolatortype = 2;
}


// ***************************************************************************
RationalInterpolator::RationalInterpolator(const RationalInterpolator& interp)
  : Interpolator(interp)
{
  interpolatortype = 2;
}


// ***************************************************************************
Point RationalInterpolator::interpolatePoint(const Point& in) 
  throw(MathException)
{
  const double offsetval = 1.0e-25;
  long int loop1, loop2, index = 1;
  double w, t, hh, h, dd;
  double* c = NULL;
  double* d = NULL;
  double retval;
  Point temp;

  try
  {
    if (numpoints == 0) // can't do anything, no points entered
      throw MathException();

    if (!(c = new (std::nothrow) double [numpoints + 1]))
      throw std::bad_alloc();
    if (!(d = new (std::nothrow) double [numpoints + 1]))
      throw std::bad_alloc();
    
    hh = fabs(in.x - Points[1].x);

    for (loop1 = 1; loop1 <= numpoints; loop1++)
    {
      h = fabs(in.x - Points[loop1].x);
      if (h == 0.0)
      {
        retval    = Points[loop1].y;
        lasterror = 0.0;
        temp.x = in.x;
        temp.y = retval;
        
        return temp;
      }
      else if (h < hh)
      {
        index = loop1;
        hh    = h;
      }
      c[loop1] = Points[loop1].y;
      d[loop1] = Points[loop1].y + offsetval; // to prevent a 0/0 condition
    }
    
    retval = Points[index--].y; // initial setting of the return;

    for (loop2 = 1; loop2 < numpoints; loop2++)
    {
      for (loop1 = 1; loop1 <= (numpoints - loop2); loop1++)
      {
        w = c[loop1 + 1] - d[loop1];
        h = Points[loop1+loop2].x - in.x; // this can never be 0;
        t = (Points[loop1].x - in.x) * d[loop1] / h;
        dd = t - c[loop1 + 1];

        // if this happens, this be bad.  Means there is a pole at the input
        // x value inx.
        if (dd == 0.0) 
          throw MathException();
        
        dd = w / dd;
        d[loop1] = c[loop1 + 1] * dd;
        c[loop1] = t * dd;
      }

      lasterror = (2 * index) < (numpoints - loop2) ? c[index + 1] 
        : d[index--];
      retval += lasterror;
    }
    temp.x = in.x;
    temp.y = retval;
    delete [] c;
    delete [] d;
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
bool RationalInterpolator::operator==(const Interpolator& rhs) const throw()
{
  const RationalInterpolator* trhs = NULL;
  
  try
  {
    trhs = dynamic_cast<const RationalInterpolator*>(&rhs);
    
    if (trhs != NULL)
      return Interpolator::operator==(rhs);

    return false;
  }
  catch(...)
  {
    return false;
  }
}


// ***************************************************************************
bool RationalInterpolator::operator!=(const Interpolator& rhs) const throw()
{
  return !(RationalInterpolator::operator==(rhs));
}

        

