// $Id: RandomGenerator.h,v 1.1.1.1 2002-02-20 13:08:20 cbilderback Exp $
// Brian Maddox - USGS MCMC SES - bmaddox@usgs.gov
// Last modified by $Author: cbilderback $ on $Date: 2002-02-20 13:08:20 $

// This is the base class for the random number generators.  It also defines
// the interface all descendents should have

#ifndef _RANDOMGENERATOR_H_
#define _RANDOMGENERATOR_H_


namespace MathLib
{

class RandomGenerator
{
 public:
  RandomGenerator() {};
  virtual ~RandomGenerator() {};

  virtual void reset() throw() =0;                  // reinit the seed values
  virtual void reset(long int& inseed) throw() =0;

  virtual float getRandom() throw() = 0;
};

} // namespace MathLib


#endif // #ifndef _RANDOMGENERATOR_H_

