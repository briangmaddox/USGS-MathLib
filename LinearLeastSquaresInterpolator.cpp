// $Id: LinearLeastSquaresInterpolator.cpp,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Originally by Christopher Bilderback

#ifndef _LINEARLEASTSQUARESINTERPOLATOR_CPP_
#define _LINEARLEASTSQUARESINTERPOLATOR_CPP_


#include "LinearLeastSquaresInterpolator.h"

using namespace MathLib;

// ***************************************************************************
LinearLeastSquaresInterpolator::LinearLeastSquaresInterpolator()
  : Slope(0), Intercept(0)
{
  interpolatortype = 3;
}

// ***************************************************************************
LinearLeastSquaresInterpolator::LinearLeastSquaresInterpolator
(const LinearLeastSquaresInterpolator& rhs) 
{
  Slope            = rhs.Slope;
  Intercept        = rhs.Intercept;
  interpolatortype = 3;
}


// ***************************************************************************
bool LinearLeastSquaresInterpolator::setPoints(const std::vector<Point>& 
                                               inPoints) throw()
{
  int counter;
  int size;
  Point * temppoints = NULL;
  bool success;
  try
    { 
      //create the temp points
      if (!(temppoints = new (std::nothrow) Point[inPoints.size()]))
        throw std::bad_alloc();

      size = static_cast<int>(inPoints.size());

      for (counter = 0; counter < size; counter++)
        temppoints[counter] = inPoints[counter];

      //call the overloaded set points
      success = setPoints(temppoints, size);
      delete [] temppoints;
      isvalid = success;
      return success;
    }
  catch(...)
    {
      delete [] temppoints;
      return false;
    }
}


  
// ***************************************************************************
bool LinearLeastSquaresInterpolator::setPoints(const Point* inPoints, 
                                               const int& insize) throw()  
{
  double sumx, sumxsqr, sumy, sumxy;
  double temp;
  double x, y;
  int N, counter;
  
  try
  {
    N = insize; 
      
    //now figure out the sums
    sumx = sumxsqr = sumy = sumxy = 0;
    for (counter = 0; counter < N; counter++)
    {
      x        = inPoints[counter].x;
      y        = inPoints[counter].y;
      sumx    += x;
      sumy    += y;
      sumxsqr += (x * x);
      sumxy   += (x * y);
    }
      
    //now solve for the coefficents    
    Intercept = (sumx * sumxy) / sumxsqr - sumy;
    temp      = ((sumx * sumx / sumxsqr) - N);
    Intercept = Intercept/temp;
    Slope     = ((sumxy - sumx * Intercept) / sumxsqr);
    isvalid   = true; // this is now valid
    return true;
  }
  catch(...)
  {
    isvalid = false;
    return false;
  }
}


// ***************************************************************************
Point LinearLeastSquaresInterpolator::interpolatePoint(const Point& in) 
  throw(MathException)
{
  Point temp;

  if (!isvalid)
    throw MathException(); // has to have the XY vectors set initially
  
  temp.x = in.x;
  temp.y = (Slope * in.x + Intercept); //simple as y = mx + b
  return temp;
}

// ***************************************************************************
bool LinearLeastSquaresInterpolator::operator==(const Interpolator& rhs) 
  const throw()
{
  const LinearLeastSquaresInterpolator* trhs = NULL;
  double tSlope, tIntercept;
  
  try
  {
    trhs = dynamic_cast<const LinearLeastSquaresInterpolator*>(&rhs);
    
    if (!trhs)
      return false;
    
    tSlope     = trhs->getSlope();
    tIntercept = trhs->getIntercept();
    
    if ((Slope == tSlope) && (Intercept == tIntercept))
      return true; //equal lines
    
    return false;
  }
  catch(...)
  {
    return false;
  }
}

// **************************************************************************
bool LinearLeastSquaresInterpolator::operator!=(const Interpolator& rhs)
  const throw()
{
  return !LinearLeastSquaresInterpolator::operator==(rhs);
}


#endif






