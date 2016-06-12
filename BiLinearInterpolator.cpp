

#ifndef _BILINEARINTERPOLATOR_H_
#define _BILINEARINTERPOLATOR_H_

#include "BiLinearInterpolator.h"


using namespace MathLib;

//************************************************************************
BiLinearInterpolator::BiLinearInterpolator() : Interpolator(),
    p0zoneoverp01(0), p1zoneoverp10(0), p2zoneoverp23(0), p3zoneoverp32(0),
    oneoverp03(0), oneoverp30(0)
{
  interpolatortype = BiLinear;
  numpoints = 4;
  try
  {
  
    if (!(Points = new (std::nothrow) Point[numpoints]))
      throw std::bad_alloc();
    isvalid = true;
  }
  catch(...)
  {
    isvalid = false;
  }

}

//************************************************************************
BiLinearInterpolator::BiLinearInterpolator(const BiLinearInterpolator & rhs)
  : Interpolator(),
    p0zoneoverp01(0), p1zoneoverp10(0), p2zoneoverp23(0), p3zoneoverp32(0),
    oneoverp03(0), oneoverp30(0)
{
  std::vector<Point> temp;
  numpoints = 4;
  try
  {
    //create the points
    if (!(Points = new (std::nothrow) Point[numpoints]))
      throw std::bad_alloc();

    interpolatortype = BiLinear;
    rhs.getPoints(temp);
      
    setPoints(temp);
    isvalid = true;
  }
  catch(...)
  {
    if (Points)
      delete [] Points;
    Points = 0;
    isvalid = false;

  }
}

//************************************************************************
BiLinearInterpolator::~BiLinearInterpolator()
{
}

//************************************************************************ 
bool BiLinearInterpolator::setPoints(const std::vector<Point>& inPoints) 
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


//*************************************************************************
bool BiLinearInterpolator::
setPoints(const Point * inPoints, const int& insize) throw()
{
  int counter(0);
  bool samegrid;
  
  try
    {
     
      if (!isvalid)
        return false;

      samegrid = true;
      for (; counter < numpoints; ++counter)
        if (!Points[counter].equals(inPoints[counter]))
        {
          samegrid = false;
          break;
        }
      if (samegrid)
      {
        return true;
      }
      //copy the points
      for (counter = 0; counter < numpoints; ++counter)
        Points[counter] = inPoints[counter];
           
      //precompute
      p0zoneoverp01 =  Points[0].z/(Points[0].x - Points[1].x);
      p1zoneoverp10 =  Points[1].z/(Points[1].x - Points[0].x);
      p2zoneoverp23 =  Points[2].z/(Points[2].x - Points[3].x);
      p3zoneoverp32 =  Points[3].z/(Points[3].x - Points[2].x);
      oneoverp03  =  1.0/(Points[0].y - Points[3].y);
      oneoverp30  = 1.0/(Points[3].y - Points[0].y);
      return true;
    }
  catch(...)
    {
      delete [] Points;
      Points = 0;
      isvalid = false;
      return false;
    }
}


//***********************************************************************
bool BiLinearInterpolator::getPoints(std::vector<Point>& in) const throw()
{
  int counter(0);
  
  try
  {
    if (!isvalid)
      return false;

    in.resize(numpoints);
    
    for (; counter < numpoints; ++counter)
      in[counter] = Points[counter];
    
    return true;
  }
  catch(...)
    {
      return false;
    }
}

//*************************************************************************
Point BiLinearInterpolator::interpolatePoint(const Point& in) 
  throw(MathException)
{
  Point temp;
  double tempz1, tempz2;
  try
    {
      if (!isvalid)
        throw MathException();
      
      tempz1 = (in.x - Points[1].x)*p0zoneoverp01;
      tempz1 += (in.x - Points[0].x)*p1zoneoverp10;
      tempz2 = (in.x - Points[3].x)*p2zoneoverp23;
      tempz2 += (in.x - Points[2].x)*p3zoneoverp32;
      
      temp.x = in.x;
      temp.y = in.y;
     
      temp.z = tempz1*(in.y - Points[3].y)*oneoverp03;
      temp.z += tempz2*(in.y - Points[0].y)*oneoverp30;

      return temp;
    }
  catch(...)
    {
      throw MathException();
    }
}


//**************************************************************************
bool BiLinearInterpolator::operator==(const Interpolator& rhs) const throw()
{
  int counter;
  bool samegrid = false;
  const BiLinearInterpolator *temp = 
    dynamic_cast<const BiLinearInterpolator*>(&rhs);
  
  if (!temp)
    return 0;
  
  if (temp->getInterpolatorType() != BiLinear)
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
  
  return samegrid; //return results
}

//***************************************************************************
bool BiLinearInterpolator::
operator!=(const Interpolator& rhs) const throw()
{
  return !((*this) == rhs);
}

#endif





