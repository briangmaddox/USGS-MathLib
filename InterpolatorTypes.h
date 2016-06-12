

#ifndef _INTERPOLATORTYPES_H_
#define _INTERPOLATORTYPES_H_


// This defines the type of interpolator for the 
// get interpolator type function. If you need
// to, add a entry for your class

namespace MathLib
{

enum InterpolatorType
{
  BaseInterpolator,    //Base is 0
  Polynomial,          //Polynomial is 1
  Rational,            //Rational is 2
  LinearLeastSquares,  //LinearLeastSquares is 3
  CubicSpline,         //CubicSpline is 4
  BiPolynomial,        //BiPolynomial is 5
  LeastSquaresPlane,   //LeastSquaresPlane is 6
  DlgViewer,           //DlgViewer is 7
  BiLinear,            //BiLinear is 8
  BiCubic,             //BiCubic is 9
  BiCubicSpline        //BiCubicSpline is 10 
};

}//namespace

#endif
