// $Id: DlgViewerInterpolator.h,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Originally by Christopher Bilderback

#ifndef _DLGVIEWERINTERPOLATOR_H_
#define _DLGVIEWERINTERPOLATOR_H_

// This is the interpolator from dlgviewer32 which is essential a modified
// bilinear interpolator (like the bipolynomial) except that it has been
// modified to work with two functional coordinates (because each map
// has a x AND a y for each point).  In this since it is probably more
// effienct for map based interpolation where there is some corrolation
// inbetween the interpolated x and y


#include "Interpolator.h"

namespace MathLib
{

class DlgViewerInterpolator : public Interpolator
{
 public:
  DlgViewerInterpolator();
  DlgViewerInterpolator(const DlgViewerInterpolator& rhs);
  virtual ~DlgViewerInterpolator() {}
  
  // Both of these functions accept four (x,y,z(x,y),w(x,y)) points
  // that are in a regular rectanglar grid as follows:
  //  X0             X1
  // 
  //
  //  X3             X2
  // Any deviation from this will result in instant death to the user
  bool setPoints(const std::vector<Point>& inPoints) throw();
  bool setPoints(const Point * inPoints, const int& insize) throw(); 

  // Return copies of the internal Points in the class
  bool getPoints(std::vector<Point>& in) const throw();
  
  // This function returns a (x,y,z,w) tuple were z and w are the
  // the interpolated x and y values
  Point interpolatePoint(const Point& in) throw(MathException);

  // Operator overloads
  virtual bool operator==(const Interpolator& rhs) const throw();
  virtual bool operator!=(const Interpolator& rhs) const throw();
};

}// namespace

#endif  
