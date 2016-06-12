// $Id: DlgViewerInterpolator.cpp,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Originally by Christopher Bilderback

#ifndef _DLGVIEWERINTERPOLATOR_CPP_
#define _DLGVIEWERINTERPOLATOR_CPP_

#include "DlgViewerInterpolator.h"

using namespace MathLib;

// *************************************************************************
DlgViewerInterpolator::DlgViewerInterpolator() : Interpolator()
{
  interpolatortype = 7;
}

// *************************************************************************
DlgViewerInterpolator::
DlgViewerInterpolator(const DlgViewerInterpolator& rhs) : Interpolator()
{
  std::vector<Point> temp;

  try
  {
    interpolatortype = 7;
    rhs.getPoints(temp);
      
    setPoints(temp);
  }
  catch(...)
  {
  }
}

// **************************************************************************
bool DlgViewerInterpolator::setPoints(const std::vector<Point>& inPoints) 
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



// ***************************************************************
bool DlgViewerInterpolator::setPoints(const Point * inPoints, 
                                      const int& insize) throw()
{
  int counter;
  bool samegrid;

  try
  {
    if (Points && (insize == numpoints))
    {
      samegrid = true;
      for (counter = 0; counter < numpoints; counter++)
        if (Points[counter] != inPoints[counter])
        {
          samegrid = false;
          break;
        }
      if (samegrid)
        return true;
    }

      
    delete [] Points;
    numpoints = insize;
    if (!(Points = new (std::nothrow) Point[numpoints]))
      throw std::bad_alloc();
      
    for (counter = 0; counter < numpoints; counter++)
      Points[counter] = inPoints[counter];
    return true;
  }
  catch(...)
  {
    delete [] Points;
    Points = NULL;
    return false;
  }
}

// ****************************************************************
bool DlgViewerInterpolator:: getPoints(std::vector<Point>& in) const throw()
{
  int counter;
  
  try
  {
    if(!numpoints)
      return false;
    
    in.resize(numpoints);
    
    for (counter= 0; counter < numpoints; counter++)
      in[counter] = Points[counter];

    return true;
  }
  catch(...)
  {
    return false;
  }
}
  

// *******************************************************************
Point DlgViewerInterpolator::interpolatePoint(const Point& in) 
  throw(MathException)
{
  double u, v;
  double xL0, yL0, xL1, yL1, xL2, yL2, xL3, yL3;
  double m13, m02;
  Point temp;

  
  /* Use the viewer's bilinear interpolation to determine the 
     projected point*/
  
  try
  {
    if (!numpoints)
      throw MathException();
    
    // Calculate the parametric u and v
    u = ( in.x - Points[0].x ) / (Points[1].x - Points[0].x);
    v = ( Points[0].y - in.y ) / (Points[0].y - Points[3].y);
      
    // Do a (modified) bilinear interpolation to get the results
    xL0 = Points[0].z + u * ( Points[1].z - Points[0].z );
    yL0 = Points[0].w + u * ( Points[1].w - Points[0].w );
    xL1 = Points[1].z + v * ( Points[2].z - Points[1].z );
    yL1 = Points[1].w + v * ( Points[2].w - Points[1].w );
    xL2 = Points[3].z + u * ( Points[2].z - Points[3].z );
    yL2 = Points[3].w + u * ( Points[2].w - Points[3].w );
    xL3 = Points[0].z + v * ( Points[3].z - Points[0].z );
    yL3 = Points[0].w + v * ( Points[3].w - Points[0].w );
    
    if ( xL1 == xL3 )
    {
      temp.z = xL1;
      temp.w = yL3;
    }
    else if ( xL2 == xL0 )
    {
      temp.z = xL2;
      temp.w = yL3 + v * ( yL1 - yL3 );
    }
    else
    {
      m13 = ( yL1 - yL3 ) / ( xL1 - xL3 );
      m02 = ( yL2 - yL0 ) / ( xL2 - xL0 );
      
      temp.z = ( m13 * xL3 - yL3 - m02 * xL0 + yL0 ) / ( m13 - m02 );
      temp.w = m13 * ( temp.z - xL3 ) + yL3;
    }
    temp.x = in.x;
    temp.y = in.y;
    return temp;
    
  }
  catch(...)
  { //something went wrong
    throw MathException();
  }
}
  
// *********************************************************************
bool DlgViewerInterpolator::operator==(const Interpolator& rhs) const throw()
{
  int counter;
  bool samegrid = false;
  const DlgViewerInterpolator *temp = 
    dynamic_cast<const DlgViewerInterpolator*>(&rhs);
  
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
  
  return samegrid; //return results
}


// ************************************************************************
bool DlgViewerInterpolator::operator!=(const Interpolator& rhs) const throw()
{
  return !((*this) == rhs);
}

#endif



