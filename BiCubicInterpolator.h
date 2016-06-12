



#ifndef BICUBICINTERPOLATOR_H_
#define BICUBICINTERPOLATOR_H_


//This is a basic Bicubic Interpolator.  It either expects 16 points or 
//4 points (which it then either uses specified  derivatives for each points 
//or calculates the derivative using the four points 
//(inaccurate linear slope method so the cross derivative = 0).
//This class also uses the tnt matrix libary which can be found at
//http://math.nist.gov/tnt/ and is free under the gpl
//BiCubicInterpolator class originally by Chris Bilderback.

#include "Interpolator.h"

#include "tnt/0.9.3/tnt.h"  //tnt stuff..
#include "tnt/0.9.3/vec.h"
#include "tnt/0.9.3/cmat.h"
#include "tnt/0.9.3/qr.h"   //qr method for solving
#include "tnt/0.9.3/lu.h"   //lu method for solving



namespace MathLib
{

class BiCubicInterpolator : public Interpolator
{
public:
  //construction and destruction
  BiCubicInterpolator();
  BiCubicInterpolator(const BiCubicInterpolator & rhs);
  virtual ~BiCubicInterpolator();

  //These functions expect either 16 points (no duplicates) in any 
  //order or a set of 4 points.  If the caller wishes to have the 
  //derivatives calculated (or set) then he/she should put the points into
  //this format.
  // x0y0     x1y1
  //
  //
  // x3y3     x4y4
  bool setPoints(const std::vector<Point>& inPoints) throw();
  bool setPoints(const Point * inPoints, const int& insize) throw(); 

  //Returns the points stored
  bool getPoints(std::vector<Point>& in) const throw();

  //Call this to actually interpolate a point over the bicubic surface
  Point interpolatePoint(const Point& in) throw(MathException);

  // Operator overloads
  bool operator==(const Interpolator& rhs) const throw();
  bool operator!=(const Interpolator& rhs) const throw();

  //These function allows the user to specify derivatives for each point
  //For this function the derivative with respect to x is specified
  //in the x value of each point, with respect to y in the y value of each
  //point and the cross derivate in the z value of each point.
  //There should also be only four points passed in.
  //The derivatives should be specifed for these points
  // x0y0     x1y1
  //
  //
  // x3y3     x4y4
  bool setDerivatives(const std::vector<Point>& inDerivatives) throw();
  bool setDerivatives(const Point *inDerivatives) throw();

  //setMinimize function allows the user to determine whether
  //to compare methods between the QR and LU and determine whether
  //which has a smaller error then use that method (warning this is 
  //slow)
  void setMinimize() throw();

 protected:
  //getMin function actually computes and compares vectors
  //by both the LU and the QR methods. Use setMinimize to enable this 
  //feature
  void getMin(const TNT::Matrix<double>& inMat, const TNT::Vector<double> B,
              TNT::Vector<double>& coefs) throw (MathException);

  Point Derivatives[4];  //the dervitives vector;
  bool DerivativesSet;    //are the derivatives set?
  bool Minimize;          //minizmize error?
  double  Coef[16];
};
}//namespace

#endif






