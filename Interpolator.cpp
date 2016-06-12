// $Id: Interpolator.cpp,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Originally by Brian Maddox

#ifndef _INTERPOLATOR_CPP_
#define _INTERPOLATOR_CPP_

#include "Interpolator.h"
#include "memory.h"

using namespace MathLib;

// **************************************************************************
Interpolator::Interpolator()
  : numpoints(0), Points(NULL), isvalid(false), interpolatortype(0)
{
}

// **************************************************************************
Interpolator::Interpolator(const Point * inPoints, const int& insize)
  : numpoints(0), Points(NULL), isvalid(false), interpolatortype(0)
{
  try
  {
    setPoints(inPoints, insize);
  }
  catch(...)
  {
    delete Points;
    Points = NULL;
  }
}

// *************************************************************************
Interpolator::Interpolator(const std::vector<Point>& inPoints)
  : numpoints(0), Points(NULL), isvalid(false), interpolatortype(0)
{
  try
  {
    setPoints(inPoints);
  }
  catch(...)
  {
    delete [] Points;
    Points = NULL;
  }
}

// **************************************************************************
Interpolator::Interpolator(const Interpolator& interp)
  : numpoints(0), Points(NULL), isvalid(false), interpolatortype(0)
{
  int counter;

  try
  {
    if (interp != *this)
    {
      numpoints = interp.numpoints;
      isvalid = interp.isvalid;
          
      if (!(Points = new (std::nothrow) Point [numpoints]))
        throw std::bad_alloc();

      for (counter = 1; counter < numpoints; counter++)
        Points[counter] = interp.Points[counter];
    }
  }
  catch(...)
  {
    delete [] Points;
    Points = NULL;
  }
}

// ***************************************************************************
Interpolator::~Interpolator()
{
  try
  {
    delete [] Points;
  }
  catch(...)
  {
    //just let it go
  }
}

// *************************************************************************
bool Interpolator::setPoints(const std::vector<Point>& inPoints) throw()
{
  long int vectsize;
  int counter;

  try
  {
    isvalid = false; // temporarily.  If the function completes, it will change
                     // valid back to true.
    
    vectsize = inPoints.size();

    //now construct the points array
    delete [] Points;
    if (!(Points = new (std::nothrow) Point[vectsize+1]))
      throw std::bad_alloc();
    
    
    // This looks funky, but deal :)  Basically, a lot of numerical stuff
    // tends to assume that vectors are 1 based instead of zero based. So,
    // this translates the input vectors into a one based array by shifting
    // all values to the right. If you need zero based array then overload
    std::vector<Point>::const_iterator it = inPoints.begin();
    for (counter = 1; counter <= vectsize; counter++)
    {
      memcpy(&(Points[counter]), &(*it), sizeof(Point));
      it++;
    }

    numpoints = vectsize; //not using the first one
    
    isvalid = true; // have data to process now

    return true;
  }
  catch (...)
  {
    delete [] Points;
    Points = NULL;
    isvalid = false; // just to make sure
    return false;
  }
}
  
// **************************************************************************
bool Interpolator::setPoints(const Point * inPoints, const int& insize) throw()
{
  try
  {
    isvalid = false;
    if (insize <= 0)
      return false;
     
    numpoints = insize;
    delete [] Points;
    if (!(Points = new (std::nothrow) Point[numpoints+1]))
      throw std::bad_alloc();
    
    // This looks funky, but deal :)  Basically, a lot of numerical stuff
    // tends to assume that vectors are 1 based instead of zero based. So,
    // this translates the input vectors into a one based array by shifting
    // all values to the right.
    memcpy((Points+1), inPoints, numpoints*sizeof(Point));
    isvalid = true;
    return true;

  }
  catch(...)
  {
    delete [] Points;
    Points = NULL;
    return false;
  }
}

//  **************************************************************************
bool Interpolator::getPoints(std::vector<Point>& in) const throw()
{
  std::vector<Point>::iterator it;

  try
  {
    in.resize(numpoints);
    // Ok, here, since we have to internally store our points with a 1 based
    // vector, load the input vector with our values shifted to the left so
    // the caller doesn't get something unexpected.
      
    it = in.begin();
    for (int counter = 0; counter < numpoints; counter++)
      memcpy(&(*it), (Points+1), sizeof(Point));
    
    return true;
  }
  catch(...)
  {
    return false;
  }
}

// ***************************************************************************
bool Interpolator::operator==(const Interpolator& rhs) const throw()
{
  try
  {
    if (numpoints != rhs.numpoints)
      return false;
    if (isvalid != rhs.isvalid)
      return false;
    //This should work
    if (!memcmp((Points+1), (rhs.Points+1), numpoints * sizeof(Points)))
      return true;
    
    return false;
  }
  catch(...)
  {
    return false;
  }
}

#endif






