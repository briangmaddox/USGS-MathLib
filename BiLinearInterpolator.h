
#ifndef _BILINEARINTEROPLATOR_H_
#define _BILINEARINTERPOLATOR_H_


//Simple (and hopefully fast) bilinear interpolator class originally by 
//Chris Bilderback.  The main reason for this class is to improve upon the 
//performance of the BiPolynomial class, which, though flexable, is pretty 
//slow. 


#include "Interpolator.h"

namespace MathLib
{

class BiLinearInterpolator : public Interpolator
{
public:
  //Construction and Destruction
  BiLinearInterpolator();
  BiLinearInterpolator(const BiLinearInterpolator & rhs);
  virtual ~BiLinearInterpolator();

  // These functions take FOUR points specified in the following
  // fashinion:
  //  X1,Y1,Z1          X2, Y2, Z2
  //
  //  X3,Y3,Z3          X4, Y4, Z4
  // Region needs to be rectangular
  bool setPoints(const std::vector<Point>& inPoints) throw();
  bool setPoints(const Point * inPoints, const int& insize) throw(); 

  // Return copies of the internal Points in the class
  bool getPoints(std::vector<Point>& in) const throw();

  // Project a point with in a grid specified by setPoints
  Point interpolatePoint(const Point& in) throw(MathException);

  // Operator overloads
  bool operator==(const Interpolator& rhs) const throw();
  bool operator!=(const Interpolator& rhs) const throw();

protected:
  //(pathetic) attempt to speed interpolation of same grid points
  double p0zoneoverp01, p1zoneoverp10, p2zoneoverp23, p3zoneoverp32,
    oneoverp03, oneoverp30;



};
}

#endif


