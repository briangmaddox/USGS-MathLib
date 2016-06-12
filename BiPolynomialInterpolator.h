// $Id: BiPolynomialInterpolator.h,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Originally by Christopher Bilderback

#ifndef _BIPOLYNOMIALINTERPOLATOR_H_
#define _BIPOLYNOMIALINTERPOLATOR_H_

// This class is a attempt at using the polynomial interpolator
// class on a 2 dimensional grid (the number of grid points 
// determines the type of polynomial used to interpolate
// (4 points = linear, 6 points = quadratic etc)

#include "PolynomialInterpolator.h"

namespace MathLib
{

class BiPolynomialInterpolator : public Interpolator 
{
 public:
  BiPolynomialInterpolator();
  BiPolynomialInterpolator(const BiPolynomialInterpolator& rhs);
  virtual ~BiPolynomialInterpolator(); 
  
  // These functions sets the points (x, y, z and
  // expects the data points to be arranaged in a REGULAR
  // RECTANGULAR GRID with at LEAST 4 points in 
  // the following convention: Sequential Rows from 
  // from max y to min y with each row ordered from
  // min x to max x.
  bool setPoints(const std::vector<Point>& inPoints) throw();
  bool setPoints(const Point* inPoints, const int& insize) throw(); 

  // Interpolate point function function is similar to the the
  // 1d analog but takes a x, y point to determine what function
  Point interpolatePoint(const Point& in) throw(MathException);
  
  // returns copys of the point set used by the interpolator
  bool getPoints(std::vector<Point>& in) const throw();

  // setOrder sets the order of the 1D interpolators used
  // Note: this is a speed up function, the Interpolator will
  // determine the order from the points if this is not set prior.
  // However, if this is used then the caller must pass the 
  // appropriate number of points for it to work (order^2 points).
  // Set the order to 0 to make the interpolator determine the
  // order
  void setOrder(int inorder) throw();

  // Operator overloads
  bool operator==(const Interpolator& rhs) const throw();
  bool operator!=(const Interpolator& rhs) const throw();
  

protected:
  PolynomialInterpolator * Interpolators; // the interpolator used
  int numinterpolators;                   // the number of interpolators
  int order;                              // the order to use                  
};

} //Namespace

#endif
