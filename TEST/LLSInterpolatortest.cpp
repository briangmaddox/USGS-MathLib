
//test program for the linear least squares


#include "LinearLeastSquaresInterpolator.h"
#include <vector>
#include <iostream>

int main(int argc, char* argv[])
{
  MathLib::LinearLeastSquaresInterpolator test;
  std::vector<double> x, y;

  try
  {
    x.resize(5);
    y.resize(5);
    
    x[0] = y[0] = 1;
    x[1] = y[1] = 2;
    x[2] = y[2] = 3;
    x[3] = y[3] = 4;
    x[4] = y[4] = 5;

    test.setXYVectors(x, y);
    
    //check the linear value of the slope and intercept
    std::cout << "y = " << test.getSlope() 
              << "x + " << test.getIntercept() << std::endl;

  }
  catch(...)
  {
    return 0;
  }
  return 0;
}






