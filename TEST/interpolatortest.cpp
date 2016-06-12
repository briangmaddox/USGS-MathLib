// $Id: interpolatortest.cpp,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $

// Test program for the interpolator classes.

#include "RationalInterpolator.h"
#include "CubicSplineInterpolator.h"
#include "LinearLeastSquaresInterpolator.h"
#include "PolynomialInterpolator.h"
#include "BiPolynomialInterpolator.h"
#include "LeastSquaresPlaneInterpolator.h"
#include "DlgViewerInterpolator.h"
#include "BiLinearInterpolator.h"
#include "BiCubicInterpolator.h"
#include "BiCubicSplineInterpolator.h"
#include <vector>
#include <iostream>

int main(int argc, char* argv[])
{
  MathLib::Interpolator * it;
  MathLib::RationalInterpolator rat;
  MathLib::CubicSplineInterpolator cub;
  MathLib::LinearLeastSquaresInterpolator llsqr;
  MathLib::PolynomialInterpolator poly;
  MathLib::BiPolynomialInterpolator bipoly;
  MathLib::BiPolynomialInterpolator *bip2;
  MathLib::LeastSquaresPlaneInterpolator lsplane;
  MathLib::DlgViewerInterpolator dlgv;
  MathLib::BiLinearInterpolator  biln;
  MathLib::BiCubicInterpolator  bicub;
  MathLib::BiCubicSplineInterpolator bicubspl;
  std::vector<MathLib::Point> input;

  MathLib::Point output;
  MathLib::Point temp;
  MathLib::Point out[4];
  try
  {
    input.resize(3);
    //y = x^2 - 3x + 9
    input[0].x = 0; input[0].y = 9;
    input[1].x = 1; input[1].y = 7;
    input[2].x = 2; input[2].y = 7;
    temp.x = 1.5;
    //now try the poly interpolator
    std::cout << "Polynomial Interpolator on x^2 -3x + 9"
              << std::endl;
    poly.setPoints(input);
    output = poly.interpolatePoint(temp);
    //should be 6.25
    std::cout << "Interpolated value: " << output.y 
              << " Actual value: 6.75" << std::endl;
    output.x = output.y = 0;
    std::cout << std::endl << "Rational Interpolator on x^2 -3x +9"
              << " using 5 points" << std::endl;
    
    input.resize(5);
    // input[0].x = 0; input[0].y = 9;
    input[0].x = 1; input[0].y = 7;
    input[1].x = 2; input[1].y = 7;
    input[2].x = 3; input[2].y = 9;
    input[3].x = 4; input[3].y = 13;
    input[4].x = 5; input[4].y = 19;
    rat.setPoints(input);
    output = rat.interpolatePoint(temp);
    std::cout << "Interpolated value: " << output.y 
              << " Actual value: 6.75" << std::endl;

    //now try a cubic spline from a book
    input.resize(4);
    input[0].x = 0; input[0].y = 0;
    input[1].x = 1; input[1].y = .5;
    input[2].x = 2; input[2].y = 2.0;
    input[3].x = 3; input[3].y = 1.5;
    
    std::cout << std::endl << "Cubic spline "
              << " using natural clamp" << std::endl;
    cub.setPoints(input);
    output = cub.interpolatePoint(temp);
    std::cout << "Interpolated Spline value: " << output.y 
              << " Actual Spline value: 1.325" << std::endl;

    //try a linear least square from a book
    std::cout << std::endl << "Linear Least Squares "
              << std::endl;

    input.resize(8);
    input[0].x = -1; input[0].y = 10;
    input[1].x = 0; input[1].y = 9;
    input[2].x = 1; input[2].y = 7;
    input[3].x = 2; input[3].y = 5;
    input[4].x = 3; input[4].y = 4;
    input[5].x = 4; input[5].y = 3;
    input[6].x = 5; input[6].y = 0;
    input[7].x = 6; input[7].y = -1;
   
    llsqr.setPoints(input);
    output = llsqr.interpolatePoint(temp);
    std::cout << "Interpolated Linear Least Squares value: " << output.y 
              << std::endl
              << "Actual Linear Least Squares value: " 
              << "6.23214" << std::endl;
 


    std::cout << std::endl << "BiPolynomial z = x + y "
              << std::endl;
    
    input.resize(4);
    input[0].x = 1; input[0].y = 1; input[0].z = 2;
    input[1].x = 2; input[1].y = 1; input[1].z = 3;
    input[2].x = 1; input[2].y = 0; input[2].z = 1;
    input[3].x = 2; input[3].y = 0; input[3].z = 2;
    
    bipoly.setPoints(input);
    temp.x = .5;
    temp.y = .2;
    output = bipoly.interpolatePoint(temp);
    std::cout << "Bipolynomial value: " << output.z 
              << std::endl
              << "Actual value: " 
              << " .7" << std::endl;
    
    //try a higher order
    bipoly.setOrder(2);
    
    std::cout << std::endl << "BiPolynomial z = x^2 + y "
              << std::endl;
    
    input.resize(9);
    input[0].x = 1; input[0].y = 1; input[0].z = 2;
    input[1].x = 2; input[1].y = 1; input[1].z = 5;
    input[2].x = 3; input[2].y = 1; input[2].z = 10;
    input[3].x = 1; input[3].y = 0; input[3].z = 1;
    input[4].x = 2; input[4].y = 0; input[4].z = 4;
    input[5].x = 3; input[5].y = 0; input[5].z = 9;
    input[6].x = 1; input[6].y = -1; input[6].z = 0;
    input[7].x = 2; input[7].y = -1; input[7].z = 3;
    input[8].x = 3; input[8].y = -1; input[8].z = 8;
    bipoly.setPoints(input);
    temp.x = .5;
    temp.y = .2;
    output = bipoly.interpolatePoint(temp);
    std::cout << "Bipolynomial value: " << output.z 
              << std::endl
              << "Actual value: " 
              << " .45" << std::endl;
    
    //try the copy constructor
    if (!(bip2 = new (std::nothrow) MathLib::BiPolynomialInterpolator(bipoly)))
      throw std::bad_alloc();
    
    //now check the equality
    if (bipoly == (*bip2))
      std::cout << "equality of bipoly accepted" << std::endl;

    output.x = 0;
    output.y = 0;
    output.z = 0;
    output = bip2->interpolatePoint(temp);
    
    std::cout << "The copy of bipoly interpolated value " << output.z
              << std::endl;

    std::cout << std::endl << "LeastSquaresPlane  z = 2x + y "
              << std::endl;
    
    input.resize(4);
    input[0].x = 1; input[0].y = 1; input[0].z = 3;
    input[1].x = 2; input[1].y = 1; input[1].z = 5;
    input[2].x = 0; input[2].y = 0; input[2].z = 0;
    input[3].x = 2; input[3].y = 0; input[3].z = 4;
    
    lsplane.setPoints(input);
    temp.x = .5;
    temp.y = .2;
    output = lsplane.interpolatePoint(temp);
    std::cout << "LeastSquaresPlane value: " << output.z 
              << std::endl
              << "Actual value: " 
              << " 1.2" << std::endl;
    
    std::cout << std::endl << "LeastSquaresPlane  z = x + 4y "
              << std::endl;
    
    input.resize(4);
    input[0].x = 1; input[0].y = 1; input[0].z = 5;
    input[1].x = 2; input[1].y = 1; input[1].z = 6;
    input[2].x = 1; input[2].y = 0; input[2].z = 1;
    input[3].x = 2; input[3].y = 0; input[3].z = 2;
    
    lsplane.setPoints(input);
    temp.x = .5;
    temp.y = .2;
    output = lsplane.interpolatePoint(temp);
    std::cout << "LeastSquaresPlane value: " << output.z 
              << std::endl
              << "Actual value: " 
              << " 1.3" << std::endl;
    




    std::cout << std::endl << "Dlgviewer interpolator using "
              <<  " z = x + y and w = x - y " << std::endl;
    
    input.resize(4);
    input[0].x = 1; input[0].y = 1; input[0].z = 2; input[0].w = 0;
    input[1].x = 2; input[1].y = 1; input[1].z = 3; input[1].w = 1;
    input[2].x = 0; input[2].y = 0; input[2].z = 0; input[2].w = 0;
    input[3].x = 2; input[3].y = 0; input[3].z = 2; input[3].w = 2; 
    
    dlgv.setPoints(input);
    temp.x = .5;
    temp.y = .2;
    output = dlgv.interpolatePoint(temp);
    std::cout << "Dlgviewer value: " << output.z 
              << ", " << output.w
              << std::endl
              << "Actual value: " 
              << " .7, " << " .3 " << std::endl;

    //bilinear
    std::cout << std::endl << "BiLinear z = x + y "
              << std::endl;
    
    input.resize(4);
    input[0].x = 1; input[0].y = 1; input[0].z = 2;
    input[1].x = 2; input[1].y = 1; input[1].z = 3;
    input[2].x = 1; input[2].y = 0; input[2].z = 1;
    input[3].x = 2; input[3].y = 0; input[3].z = 2;
    
    biln.setPoints(input);
    temp.x = .5;
    temp.y = .2;
    output = biln.interpolatePoint(temp);
    std::cout << "BiLinear value: " << output.z 
              << std::endl
              << "Actual value: " 
              << " .7" << std::endl;


    //bicubic
    std::cout << std::endl << "BiCubic z = x^3 + y^3"
              << std::endl;
    input.resize(16);
    input[12].x = 1; input[12].y = 1; input[12].z = 2;
    input[13].x = 2; input[13].y = 1; input[13].z = 9;
    input[14].x = 3; input[14].y = 1; input[14].z = 28;
    input[15].x = 4; input[15].y = 1; input[15].z = 65;
    
    input[8].x = 1; input[8].y = 2; input[8].z = 9;
    input[9].x = 2; input[9].y = 2; input[9].z = 16;
    input[10].x = 3; input[10].y = 2; input[10].z = 35;
    input[11].x = 4; input[11].y = 2; input[11].z = 72;
    
    input[4].x = 1; input[4].y = 3; input[4].z = 28;
    input[5].x = 2; input[5].y = 3; input[5].z = 35;
    input[6].x = 3; input[6].y = 3; input[6].z = 54;
    input[7].x = 4; input[7].y = 3; input[7].z = 91;
    
    input[0].x = 1; input[0].y = 4; input[0].z = 65;
    input[1].x = 2; input[1].y = 4; input[1].z = 72;
    input[2].x = 3; input[2].y = 4; input[2].z = 91;
    input[3].x = 4; input[3].y = 4; input[3].z = 128;
    
    bicub.setPoints(input);
    temp.x = 1.5;
    temp.y = 1.5;
    output = bicub.interpolatePoint(temp);
    std::cout << "BiCubic value: " << output.z 
              << std::endl
              << "Actual value: " 
              << " 6.75" << std::endl;

    //try it with just drivetives
    std::cout << std::endl << "BiCubic z = x^3 + y^3"
              << std::endl;
    input.resize(4);
   
  
    input[0].x = 3;  input[0].y = 3; input[0].z = 0;
    input[1].x = 12; input[1].y = 3; input[1].z = 0;
    input[2].x = 3;  input[2].y = 0; input[2].z = 0;
    input[3].x = 12; input[3].y = 0; input[3].z = 0;
    
    bicub.setDerivatives(input);

    input[0].x = 1; input[0].y = 1; input[0].z = 2;
    input[1].x = 2; input[1].y = 1; input[1].z = 9;
    input[2].x = 1; input[2].y = 0; input[2].z = 1;
    input[3].x = 2; input[3].y = 0; input[3].z = 8;

    if (!bicub.setPoints(input))
      std::cout << "Not working" << std::endl;
    temp.x = 1.5;
    temp.y = 1.5;
    output = bicub.interpolatePoint(temp);
    std::cout << "BiCubic value: " << output.z 
              << std::endl
              << "Actual value: " 
              << " 6.75" << std::endl;


    //Bicubicspline
    std::cout << std::endl << "BiCubicSpline z = x^3 + y^3"
              << std::endl;

    input.resize(16);
    
    input[12].x = 1; input[12].y = 1; input[12].z = 2;
    input[13].x = 2; input[13].y = 1; input[13].z = 9;
    input[14].x = 3; input[14].y = 1; input[14].z = 28;
    input[15].x = 4; input[15].y = 1; input[15].z = 65;
    
    input[8].x = 1; input[8].y = 2; input[8].z = 9;
    input[9].x = 2; input[9].y = 2; input[9].z = 16;
    input[10].x = 3; input[10].y = 2; input[10].z = 35;
    input[11].x = 4; input[11].y = 2; input[11].z = 72;
    
    input[4].x = 1; input[4].y = 3; input[4].z = 28;
    input[5].x = 2; input[5].y = 3; input[5].z = 35;
    input[6].x = 3; input[6].y = 3; input[6].z = 54;
    input[7].x = 4; input[7].y = 3; input[7].z = 91;
    
    input[0].x = 1; input[0].y = 4; input[0].z = 65;
    input[1].x = 2; input[1].y = 4; input[1].z = 72;
    input[2].x = 3; input[2].y = 4; input[2].z = 91;
    input[3].x = 4; input[3].y = 4; input[3].z = 128;


    
    bicubspl.setNatural();
    bicubspl.setPoints(input);
    temp.x = 1.5;
    temp.y = 1.5;
    output = bicubspl.interpolatePoint(temp);
    std::cout << "BiCubic spline value: " << output.z 
              << std::endl
              << "Actual value: " 
              << " 6.75" << std::endl;

    //try and check with linear cubic spline
    input.resize(4);
    input[0].x = 1;  input[0].y = 2;
    input[1].x = 2;  input[1].y = 9;
    input[2].x = 3;  input[2].y = 28;
    input[3].x = 4;  input[3].y  = 65;

    cub.setNatural();
    cub.setPoints(input);
    
    temp.x = 1.5;
    
    out[0] = cub.interpolatePoint(temp);
    out[0].x = 1; //reset the x to y

    input[0].x = 1; input[0].y = 9;
    input[1].x = 2; input[1].y = 16;
    input[2].x = 3; input[2].y = 35;
    input[3].x = 4; input[3].y = 72;

    cub.setNatural();
    cub.setPoints(input);
    
    temp.x = 1.5;
    out[1] = cub.interpolatePoint(temp);
    out[1].x = 2; //reset the x to y

    input[0].x = 1; input[1].y = 28;
    input[1].x = 2; input[2].y = 35;
    input[2].x = 3; input[3].y = 54;
    input[3].x = 4; input[4].y = 91;
    
    
    cub.setNatural();
    cub.setPoints(input);
    
    temp.x = 1.5;
    out[2] = cub.interpolatePoint(temp);
    out[2].x = 3; //reset the x to y

    input[0].x = 1; input[0].y = 65;
    input[1].x = 2; input[1].y = 72;
    input[2].x = 3; input[2].y = 91;
    input[3].x = 4; input[3].y = 128;

     
    cub.setNatural();
    cub.setPoints(input);
    
    temp.x = 1.5;
    out[3] = cub.interpolatePoint(temp);
    out[3].x = 4; //reset the x to y
    
    input[0].y = out[0].y;
    input[1].y = out[1].y;
    input[2].y = out[2].y;
    input[3].y = out[3].y;
    
    cub.setNatural();
    cub.setPoints(input);
    
    temp.x = 1.5;
    output = cub.interpolatePoint(temp);
        
    std::cout << output.y << std::endl;


    return 0;
  }
  catch (...)
  {
    std::cout << "An exception has been thrown! Exiting..." << std::endl;
    exit (-1);
  }
}













