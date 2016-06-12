// $Id: BiPolynomialInterpolator.cpp,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Originally by Christopher Bilderback

#ifndef _BIPOLYNOMIALINTERPOLATOR_CPP_
#define _BIPOLYNOMIALINTERPOLATOR_CPP_

#include "BiPolynomialInterpolator.h"
#include <math.h>

using namespace MathLib;

// **************************************************************************
BiPolynomialInterpolator::BiPolynomialInterpolator()
  : Interpolator(), Interpolators(NULL),
    numinterpolators(0), order(0)
{
  interpolatortype = 5;
}

//***************************************************************************
BiPolynomialInterpolator::BiPolynomialInterpolator
(const BiPolynomialInterpolator& rhs) 
  : Interpolators(NULL), numinterpolators(0),order(0)
{
  int counter;
  
  try
  {
    //get the appropriate sizes for the storage arrays
    numinterpolators = rhs.numinterpolators;
    order            = rhs.order;
    numpoints        = rhs.numpoints;
    interpolatortype = 5;
      
    if (!(Points = new (std::nothrow) Point[numpoints]))
      throw std::bad_alloc();

    for (counter = 0; counter < numpoints; counter++)
      Points[counter] = rhs.Points[counter];

    if (!(Interpolators = new(std::nothrow) 
          PolynomialInterpolator[numinterpolators]))
      throw std::bad_alloc();
      
    for (counter = 0; counter < numinterpolators; counter++)
      Interpolators[counter] = rhs.Interpolators[counter];

  }
  catch(...)
  {
    delete [] Interpolators;
    Interpolators = NULL;
  }
}

// **************************************************************************
BiPolynomialInterpolator::~BiPolynomialInterpolator()
{
  delete [] Interpolators;
}

// **************************************************************************
bool BiPolynomialInterpolator::setPoints(const std::vector<Point>& inPoints) 
  throw()

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




// **************************************************************************
bool BiPolynomialInterpolator::setPoints(const Point * inPoints, 
                                         const int& insize) throw()
{
  int    counter;
  int    passsize;
  bool   samegrid;
  Point* temp;

  try
  {
    if (!insize)
      return false;
    
    //check for the possiblity of using the same grid
    if (Points && (numpoints == insize))
    {
      samegrid = true; //assume true
      for (counter = 0; counter < numpoints; counter++)
        if (Points[counter] != inPoints[counter])
        {
          samegrid = false; //reset flag
          break;
        }
      if (samegrid)
        return true; //we have already been set up
    }
      
    //not already set up proceed with setup
    delete [] Points;
    numpoints = insize;
    if (!(Points = new (std::nothrow) Point[numpoints]))
      throw std::bad_alloc();
      
    //copy the points
    for (counter = 0; counter < numpoints; counter++)
      Points[counter] = inPoints[counter];
      
    //check the order
    if (!order)
    {  
      //figure out the order
      numinterpolators = static_cast<int>(sqrt(numpoints)) + 1;
      order = numinterpolators-2;
      //create the appropriate number of interpolators
      delete [] Interpolators;
      if (!(Interpolators = new (std::nothrow) 
            PolynomialInterpolator[numinterpolators]))
        throw std::bad_alloc();
      
    }
      
    if (!(temp = new (std::nothrow) Point[numpoints]))
      throw std::bad_alloc();
      
    for (counter = 0; counter < numpoints; counter++)
    {
      temp[counter].y = Points[counter].z;
      temp[counter].x = Points[counter].x;
    }
    passsize = numinterpolators-1;
    //set the x across vectors
    for (counter = 0; counter < passsize; counter++)
    {
      Interpolators[counter].setPoints(temp+counter*passsize,
                                       passsize);
    }
    
    // The Y Interpolator  will have to be set during
    // interpolate point.
      
    isvalid = true;
    delete [] temp;
    return true;
  }
  catch(...)
  {
    delete [] Interpolators;
    delete [] Points;
    delete [] temp;
    numpoints = numinterpolators = 0;
    Points = NULL;
    Interpolators = NULL;
    return false;
  }
}  


// ************************************************************************
Point BiPolynomialInterpolator::interpolatePoint(const Point& in) 
  throw(MathException)
{
  int    counter;
  int    sizeacross;
  Point* ys;
  Point  temp, temp1;

  try
  {
    if (numinterpolators < 3)
      throw MathException();
     
    sizeacross =  numinterpolators -1;
      
    if (!(ys = new (std::nothrow) Point[sizeacross]))
      throw std::bad_alloc();

    for (counter = 0; counter < sizeacross; counter++)
    {
      ys[counter] = Interpolators[counter].interpolatePoint(in);
      ys[counter].x = Points[counter*sizeacross].y;
    }
      
    //now do the intpolation in the y direction
    Interpolators[sizeacross].setPoints(ys, sizeacross);
    temp1 = in;
    temp1.x = temp1.y;
      
    temp = Interpolators[sizeacross].interpolatePoint(temp1);
    temp.z = temp.y;
    temp.x = in.x;
    temp.y = in.y;
    delete [] ys;
    return temp;
  }
  catch(...)
  {
    delete [] ys;
    throw MathException();
  }
}
      
// ******************************************************************
bool BiPolynomialInterpolator::getPoints(std::vector<Point>& in) const throw()
{
  int counter;
  
  in.resize(numpoints);
  
  for (counter = 0; counter < numpoints; counter++)
    in[counter] = Points[counter];
  
  return true;
}


// ******************************************************************
void BiPolynomialInterpolator::setOrder(int inorder) throw()
{
  try
  {
    order = inorder;
    //figure out the order
    numinterpolators = order + 2;
     
    //create the appropriate number of interpolators
    delete [] Interpolators;
    Interpolators = NULL;
    if (!(Interpolators = new (std::nothrow) 
          PolynomialInterpolator[numinterpolators]))
      throw std::bad_alloc();
  }
  catch(...)
  {
    delete [] Interpolators;
    Interpolators = NULL;
  }
}

// Operator overloads
// ***************************************************************************
bool BiPolynomialInterpolator:: operator==(const Interpolator& rhs) 
  const throw()
{
  int counter;
  bool samegrid = false;
  const BiPolynomialInterpolator *temp = 
  dynamic_cast<const BiPolynomialInterpolator*>(&rhs);
    
  if (numinterpolators != temp->numinterpolators)
    return false;
    
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
    
  return samegrid; //has the same grid so it is the same
}


// ***************************************************************************
bool BiPolynomialInterpolator::operator!=(const Interpolator& rhs)
  const throw()
{
  return !((*this) == rhs);
}

      
#endif






