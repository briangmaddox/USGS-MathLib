// $Id: MathException.h,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $
// Originally by Brian Maddox

#ifndef _MATHEXCEPTION_H_
#define _MATHEXCEPTION_H_

// This class is my standard wrapper for any exceptions which may come out of
// the math class.
#include <string>

namespace MathLib
{

#define INVALID_ARGUMENT_ERROR 0;
#define DIVIDE_BY_ZERO_ERROR   1;
#define MULTIPLICATION_ERROR   2;
#define OUT_OF_BOUNDS_ERROR    3;
#define UNDERFLOW_ERROR        4;
#define OVERFLOW_ERROR         5;
#define INPUT_ERROR            6;
#define UNKNOWN_MATH_ERROR   255;

class MathException  
{
 public:
  MathException(); // default constructor, assume unknown error
  MathException(const int& inerror); // user can set the error
  virtual ~MathException();

  int  getException()                   const throw();
  void setException(const int& inerror)       throw();

  virtual std::string getString() const throw(); 

protected:
  int _exception;
};

// **************************************************************************
inline int MathException::getException() const throw()
{
  return _exception;
}

// **************************************************************************
inline void MathException::setException(const int& inerror) throw()
{
  _exception = inerror;
}

} // namespace MathLib

#endif // #ifndef _MATHEXCEPTION_H_
