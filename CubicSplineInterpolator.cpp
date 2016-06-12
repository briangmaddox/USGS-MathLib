// $Id: CubicSplineInterpolator.cpp,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Originally by Brian Maddox

#include "CubicSplineInterpolator.h"

using namespace MathLib;

// ***************************************************************************
CubicSplineInterpolator::CubicSplineInterpolator()
  : Interpolator(), isnatural(true), secders(NULL), numsecders(0)
{
  first = nth = 1.0e30;
  interpolatortype = 4;
}


// ***************************************************************************
CubicSplineInterpolator::CubicSplineInterpolator
(const CubicSplineInterpolator& rhs)
  : Interpolator(rhs)
{
  std::vector<double> tsecders;
  int counter;

  try
  {
    interpolatortype = 4;
    isnatural = rhs.isNatural();
    rhs.getSecondDerivatives(tsecders);
    if (!tsecders.size())
    {
      secders = NULL;
      numsecders = 0;
    }
    else
    {
      numsecders = tsecders.size();
      
      if (!(secders = new (std::nothrow) double[numsecders+1]))
        throw std::bad_alloc();
      for (counter = 0; counter < numsecders; counter++)
        secders[counter+1] = tsecders[counter];
      
      getBoundaryDerivatives(first, nth);
    }
  }
  catch(...)
  {
    delete [] secders;
    secders = NULL;
  }
}

// ***************************************************************************
CubicSplineInterpolator::~CubicSplineInterpolator()
{
  delete [] secders;
}

// ***************************************************************************
void CubicSplineInterpolator::setNatural() throw(MathException)
{
  if (!isvalid) // bad programmer, didn't pass in XY vectors first.
    throw MathException(); 

  if (isnatural)
    return; // If it's valid and already natural, no need to do the setup again

  first = nth = 1.0e30; // for the function

  setupInterpolator(); // this will throw if it has an error
  
  isnatural = true; // everyting is ok so go ahead and set this
}


// ***************************************************************************
void CubicSplineInterpolator::setBoundaries(const double& infirst,
                                            const double& inn)
  throw(MathException)
{
  first = infirst;
  nth   = inn;

  if (!isvalid) // bad programmer, didn't pass in XY vectors first
    throw MathException(); 
  
  // Make sure they're not trying to use this to make a natural spline
  if ( (first >= 1.0e30) || (nth >= 1.0e30))
    isnatural = true;
  else
    isnatural = false;

  setupInterpolator();
}

// ****************************************************************************
bool CubicSplineInterpolator::setPoints(const std::vector<Point>& inPoints) 
  throw()
{
  try
  {
    if (!Interpolator::setPoints(inPoints))
      return false;
      
    // Ok, now the XY vectors should be set. The next step is to setup the 
    // object and get the second derivatives.  Everything should be set ok
    // up to this point so just go ahead and run the setup function.
    setupInterpolator();
    
    return true;
  }
  catch(...)
  {
    isvalid = false;
    return false;
  }
}

// ***************************************************************************
bool CubicSplineInterpolator::setPoints(const Point * inPoints, 
                                        const int& insize)  throw()
{
  try
  {
    if (!Interpolator::setPoints(inPoints, insize))
      return false;
      
    // Ok, now the XY vectors should be set. The next step is to setup the 
    // object and get the second derivatives.  Everything should be set ok
    // up to this point so just go ahead and run the setup function.
    setupInterpolator();
    
    return true;
  }
  catch(...)
  {
    isvalid = false;
    return false;
  }
}


// ***************************************************************************
bool CubicSplineInterpolator::getSecondDerivatives(std::vector<double>& inv) 
  const throw()
{
  // Just like the getXVector calls, this has to make sure that it translates
  // the internal vector to something more normal.
  
  try
  {
    inv.clear();
    inv.resize(numsecders);
    
    for (long int count = 0; count < numsecders; count++)
    {
      inv[count] = secders[count + 1];
    }

    return true;
  }
  catch (...)
  {
    return false;
  }
}


// ***************************************************************************
void CubicSplineInterpolator::setupInterpolator() throw(MathException)
{
  long int loop1, loop2;
  double p, qn, sig, un;
  double * u;

  if (!isvalid)
    throw MathException(); // has to have the XY vectors set initially

  try
  {
    isvalid = false; // only will become valid again if this finishes
    
    if (!(u = new (std::nothrow) double[numpoints + 1]))
      throw std::bad_alloc();
    
    // create the second derivatives
    delete [] secders;
    secders = NULL;
    if (!(secders = new (std::nothrow) double[numpoints+1]))
      throw std::bad_alloc();
    

    // The lower boundary condition is set either to be "natural" or else have
    // a specified first derivative.  
    if (first > 0.99e30)
      secders[1] = u[1] = 0.0;
    else
    {
      secders[1] = -0.5;
      u[1] = (3.0 / (Points[2].x - Points[1].x)) * ((Points[2].y - Points[1].y)
                                                    / (Points[2].x - 
                                                       Points[1].x) - first);
    }

    // This is the decomposition loop of the tridiagonal algorithm.  secders
    // and u are used for temporary storage of the decomposed factors.
    for (loop1 = 2; loop1 <= (numpoints - 1); loop1++)
    {
      sig = (Points[loop1].x - Points[loop1 - 1].x) / (Points[loop1 + 1].x - 
                                                       Points[loop1 - 1].x);
      p = sig * secders[loop1 - 1] + 2.0;
      secders[loop1] = (sig - 1.0) / p;
      u[loop1] = (Points[loop1 + 1].y - Points[loop1].y) / 
        (Points[loop1 + 1].x - Points[loop1].x) 
        - (Points[loop1].y - Points[loop1 - 1].y) / (Points[loop1].x - 
                                                     Points[loop1 - 1].x);
      u[loop1] = (6.0 * u[loop1] / (Points[loop1 + 1].x - Points[loop1 - 1].x)
                  - sig * u[loop1 - 1]) / p;
    }

    
    // The upper boundary condition is set either to be "natural" or to have
    // a specified first derivative.
    if (nth > 0.99e30)
      qn = un = 0.0;
    else
    {
      qn = 0.5;
      un = (3.0 / (Points[numpoints].x - Points[numpoints - 1].x)) * 
        (nth - (Points[numpoints].y - Points[numpoints - 1].y) / 
         (Points[numpoints].x - Points[numpoints - 1].x));
    }
    
    secders[numpoints] = (un - qn * u[numpoints - 1]) / 
      (qn * secders[numpoints - 1] + 1.0);

    // This is the backsubstitution loop of the tridiagonal algorithm.
    for (loop2 = numpoints - 1; loop2 >= 1; loop2--)
      secders[loop2] = secders[loop2] * secders[loop2 + 1] + u[loop2];

    // Got here so everything must be ok...right? ;)
    isvalid = true;
    delete [] u;
    return;
  }
  catch(...)
  {
    delete [] u;
    delete [] secders;
    secders = NULL;
    throw MathException();
  }
}


// ***************************************************************************
Point CubicSplineInterpolator::interpolatePoint(const Point& in)
  throw(MathException)
{
  long int klo, khi, k;
  double h, b, a;
  double retval;
  Point temp;

  // We will find the right place in the table by means of bisection.  This is
  // optimal if sequential calls to this routine are at random values of x. If
  // sequential calls are in order, and closely spaced, one would do better to
  // store previous valuse of klo and khi and test if they remain appropriate
  // to the next call.
  klo = 1;
  khi = numpoints;

  while ( (khi - klo) > 1)
  {
    k = (khi + klo) >> 1;
    if (Points[k].x > in.x)
      khi = k;
    else
      klo = k;

    // klo and khi now bracket the input value of x
  }

  h = Points[khi].x - Points[klo].x;
  if (h == 0.0)
    throw MathException();

  a = (Points[khi].x - in.x) / h;
  b = (in.x - Points[klo].x) / h;

  retval = a * Points[klo].y + b * Points[khi].y + ((a * a * a - a) 
                         * secders[klo] + (b * b * b - b) * secders[khi]) *
    (h * h) / 6.0;
  temp.x = in.x;
  temp.y = retval;
  return temp;
}


// ***************************************************************************
bool CubicSplineInterpolator::operator==(const Interpolator& rhs) const throw()
{
  const CubicSplineInterpolator* trhs = NULL;
  double tfirst, tnth;
  std::vector<double> tx, ty, tsec;;
  int counter;

  try
  {
    trhs = dynamic_cast<const CubicSplineInterpolator*>(&rhs);

    if (trhs != NULL)
    {
      trhs->getBoundaryDerivatives(tfirst, tnth);
      trhs->getSecondDerivatives(tsec);
      
      if ( (isnatural == trhs->isNatural()) &&
           (first == tfirst) && (nth == tnth) )
      {
        //check the second ders
        for (counter = 0; counter < numsecders; counter++)
          if (tsec[counter] != secders[counter+1])
            return false;
        
        return Interpolator::operator==(rhs);
      }
    }
    return false;
  }
  catch (...)
  {
    return false;
  }
}

// ***************************************************************************
bool CubicSplineInterpolator::operator!=(const Interpolator& rhs) const 
  throw()
{
  return !CubicSplineInterpolator::operator==(rhs);
}




