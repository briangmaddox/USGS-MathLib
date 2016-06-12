// $Id: Point.h,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Originally by Christopher Bilderback

#ifndef _POINT_H_
#define _POINT_H_

namespace MathLib
{

class Point
{
 public:
  double x, y, z, w;
  //check to see if the two points are equal
  bool operator==(const Point& in) const throw();
  bool operator!=(const Point& in) const throw();

  //comparison without operoator overloading
  bool equals(const Point& in) const throw();

};
 
//*************************************************************************
inline
bool Point::equals(const Point& in) const throw()
{
  if ((x != in.x) || (y != in.y) || (z != in.z)
      || (w != in.w))
    return false;
  return true;
}


 // **************************************************************************
inline
bool Point::operator==(const Point& in) const throw()
{
  if ((x != in.x) || (y != in.y) || (z != in.z)
      || (w != in.w))
    return false;
  return true;
}

// ***************************************************************************
inline
bool Point::operator!=(const Point& in) const throw()
{
  return !((*this) == in);
}

}//namespace

#endif
