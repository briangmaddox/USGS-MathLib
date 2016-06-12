
#ifndef _BICUBICSPLINEINTERPOLATOR_H_
#define _BICUBICSPLINEINTERPOLATOR_H_


//Basic Bicubic Spline interpolator which construcs m 1d splines in the x
//direction and then does a spline in the y direction 
//(BiLinear except with cubic splines).  This does not use the 
//Cubic Spline class for speed reasons (it still is probably slow).
//By Chris Bilderback.


#include "Interpolator.h"

#include "tnt/0.9.3/tnt.h"  //tnt stuff..
#include "tnt/0.9.3/vec.h"
#include "tnt/0.9.3/cmat.h"
#include "tnt/0.9.3/qr.h"   //qr method for solving
#include "tnt/0.9.3/lu.h"   //lu method for solving


namespace MathLib
{


class BiCubicSplineInterpolator : public Interpolator
{
public:
  //Construction and Destruction
  BiCubicSplineInterpolator();
  BiCubicSplineInterpolator(const BiCubicSplineInterpolator & rhs);
  virtual ~BiCubicSplineInterpolator();

  //These functions expect a regular rectanglar NxN array
  //of points like the following
  //x5y5       x6y6     x7y7      x8y8
  //
  //
  //x1y1       x1y1     x1y1      x1y1 etc..
  //Need at least a 3x3 grid
  bool setPoints(const std::vector<Point>& inPoints) throw();
  bool setPoints(const Point * inPoints, const int& insize) throw(); 

  //Returns the points stored
  bool getPoints(std::vector<Point>& in) const throw();

  //Call this to actually interpolate a point over the bicubic surface
  Point interpolatePoint(const Point& in) throw(MathException);

  // Operator overloads
  bool operator==(const Interpolator& rhs) const throw();
  bool operator!=(const Interpolator& rhs) const throw();

  // Set a natural cubic spline fit, ie M0 = MN = 0
  // This is the DEFAULT
  void setNatural() throw();
  
  // Set a parabolic cubic spline fit, ie M0 = M1, MN = M(N-1)
  void setParabolic() throw();

  // Allows user to set custom second ders
  // This should be specified as follows
  // M0 MN (first spline)
  // M0 MN (second spline) etc...
  bool setSecondDers(const std::vector<double>& inDers) throw();
  bool setSecondDers(const double * inDers, const int& insize) throw();

 protected:
  //fill natural matrix fills in a natural coef matrix
  void fillNaturalMatrix() throw();

  //fillParabolic Matrix fills in a parabolic coef matrix
  void fillParabolicMatrix() throw();
  
  int SegmentsPerSpline;    //the number of cubic segments in each spline
  int NumberOfSplines;      //the number of splines
  double ** Splines;        //the splines (by their coeffients)
  
  bool Parabolic;           //use a clamped fit
  bool Natural;             //use a natural fit
  bool SetDerivatives;      //use specified derivatives
  int PointsPerSpline;      //the number points per Spline
  double ** Ders;           //the derv
  TNT::Matrix<double> MCoef;//the coef matrix
 
};

}

#endif  



