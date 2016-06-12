// $Id: MathException.cpp,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Originally by Brian Maddox

#ifndef _MATHEXCEPTION_CPP_
#define _MATHEXCEPTION_CPP_

#include "MathException.h"

using namespace MathLib;

// **************************************************************************
MathException::MathException()
{
  _exception = UNKNOWN_MATH_ERROR;
}

// **************************************************************************
MathException::MathException(const int& inerror)
  : _exception(inerror)
{
}

// **************************************************************************
MathException::~MathException()
{
}


// **************************************************************************
std::string MathException::getString() const throw()
{
  std::string retstring;
  
  switch(_exception)
  {
  case 0:
    retstring = std::string("INVALID_ARGUMENT_ERROR");
    break;
  case 1:
    retstring = std::string("DIVIDE_BY_ZERO_ERROR");
    break;
  case 2:
    retstring = std::string("MULTIPLICATION_ERROR");
    break;
  case 3:
    retstring = std::string("OUT_OF_BOUNDS_ERROR");
    break;
  case 4:
    retstring = std::string("UNDERFLOW_ERROR");
    break;
  case 5:
    retstring = std::string("OVERFLOW_ERROR");
    break;
  case 6:
    retstring = std::string("INPUT ERROR");
    break;
  default:
    retstring = std::string("UNKNOWN_MATH_ERROR");
  }

  return retstring;
}

#endif // #ifndef _MATHEXCEPTION_H_
