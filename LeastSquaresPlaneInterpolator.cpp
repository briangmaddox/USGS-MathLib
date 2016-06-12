// $Id: LeastSquaresPlaneInterpolator.cpp,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Originally by Christopher Bilderback

#ifndef _LEASTSQUARESPLANEINTERPOLATOR_CPP_
#define _LEASTSQUARESPLANEINTERPOLATOR_CPP_

#include "LeastSquaresPlaneInterpolator.h"

using namespace MathLib;

// **************************************************************************
LeastSquaresPlaneInterpolator::LeastSquaresPlaneInterpolator()
  : Interpolator(), xcoef(0), ycoef(0), dvalue(0)
{
  interpolatortype = 6;
}

// **************************************************************************
LeastSquaresPlaneInterpolator::
LeastSquaresPlaneInterpolator(const LeastSquaresPlaneInterpolator& rhs)
  : Interpolator(), xcoef(0), ycoef(0), dvalue(0)
{
  std::vector<Point> temp;

  try
  {
    interpolatortype = 6;
    //Use get points to go from a different interpolator
    //casted as a LeastSquares Plane Interpolator
    //Note this needs to be changed in all classes
    rhs.getPoints(temp);
      
    setPoints(temp);//set the points 
  }
  catch(...)
  {
    delete [] Points;
    Points = NULL;
  }
}      

// *********************************************************************
bool LeastSquaresPlaneInterpolator::setPoints(const std::vector<Point>& 
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


// ********************************************************************
bool LeastSquaresPlaneInterpolator::setPoints(const Point* inPoints, 
                                              const int& insize) throw() 
{
  long double xsqrs, xys, xs, zxs, ysqrs, ys, zs, zys; // the sums
  int counter;
  Point temp;
  bool samepoints = true;

  try
  {
    //see if the previous setup call was the same as this
    if (Points && (numpoints == insize))
    {
      for (counter = 0; counter < numpoints; counter++)
      {
        if (Points[counter] != inPoints[counter])
        {
          samepoints = false;
          break;
        }
      }
      if (samepoints)
        return true; //the same grid will produce same plane
    }

    //not the same grid so setup
    delete [] Points;
    numpoints = insize;
    if (!(Points = new (std::nothrow) Point[numpoints]))
      throw std::bad_alloc();
    // copy the vector points and calc sums
    xsqrs = xys = xs = zxs = ysqrs = ys = zs = zys = 0;
    for (counter = 0; counter < numpoints; counter++)
    {
      Points[counter] = inPoints[counter];
      temp   = Points[counter];
      xs    += temp.x;
      xsqrs += temp.x * temp.x;
      xys   += temp.x * temp.y;
      zxs   += temp.x * temp.z;
      ysqrs += temp.y * temp.y;
      ys    += temp.y;
      zs    += temp.z;
      zys   += temp.y * temp.z;
    }
    //Mathcad :)
    xcoef = -(-(ys * ys * zxs) + ys * xys * zs + ys * zys * xs 
              - numpoints * xys * zys + numpoints * zxs * ysqrs - 
              zs * ysqrs * xs);
    xcoef = xcoef / (xys * xys * numpoints + ys * ys * xsqrs - 2 * ys * xs
                     * xys - ysqrs * xsqrs * numpoints + ysqrs * xs * xs);
    ycoef = xys * xcoef * numpoints - ys * xs * xcoef + ys * zs - 
      zys * numpoints;
    ycoef = ycoef / (-ysqrs * numpoints + ys * ys);
    
    dvalue = zs - ys * ycoef - xs * xcoef;
    dvalue = dvalue/numpoints;
      
    return true;
  }
  catch(...)
  {
    delete [] Points;
    Points = NULL;
    return false;
  }
}

// ***********************************************************************
bool LeastSquaresPlaneInterpolator::getPoints(std::vector<Point>& in) 
  const throw()
{
  int counter;
  if (!numpoints || !Points)
    return false;
  
  in.resize(numpoints);
  
  for (counter = 0; counter < numpoints; counter++)
    in[counter] = Points[counter];
  
  return true;
}  
      
// **********************************************************************
Point LeastSquaresPlaneInterpolator::interpolatePoint(const Point& in) 
  throw(MathException)
{
  Point temp;
  if (!Points || !numpoints)
    throw MathException();
  
  temp.x = in.x;
  temp.y = in.y;
  temp.z = in.x * xcoef + in.y * ycoef + dvalue;
  
  return temp;
}
  
// **********************************************************************
bool LeastSquaresPlaneInterpolator::operator==(const Interpolator& rhs) 
  const throw()
{
  int counter;
  bool samegrid = false;
  const LeastSquaresPlaneInterpolator *temp = 
    dynamic_cast<const LeastSquaresPlaneInterpolator*>(&rhs);
  
  if (!temp)
    return 0;
  
  if (Points && (numpoints == temp->numpoints))
  {
    samegrid = true; //assume true
    for (counter = 0; counter < numpoints; counter++)
      if (Points[counter] != temp->Points[counter])
      {
        samegrid = false; //reset flag
        break;
      }
  }
  
  if ((xcoef != temp->xcoef) || (ycoef != temp->ycoef) ||
      (dvalue != dvalue))
    return false;

  return samegrid; //return results
}

// **********************************************************************
bool LeastSquaresPlaneInterpolator::operator!=(const Interpolator& rhs) 
  const throw()
{
  return !((*this) == (rhs));
}

#endif







