//implementation for the Bicubicspline interpolator

#ifndef _BICUBICSPLINEINTERPOLATOR_CPP_
#define _BICUBICSPLINEINTERPOLATOR_CPP_


#include "BiCubicSplineInterpolator.h"
#include "math.h"


using namespace MathLib;


//************************************************************************
BiCubicSplineInterpolator::BiCubicSplineInterpolator() : Interpolator(),
SegmentsPerSpline(0), NumberOfSplines(0), Splines(NULL),
Parabolic(false), Natural(true), SetDerivatives(false), 
PointsPerSpline(0), Ders(NULL)
{
  try
    {
      interpolatortype = 10;
      isvalid = false;

      //setup for natural fit
      setNatural();
    }
  catch(...)
    {
      //nothing to be done
    }
}


//***********************************************************************
BiCubicSplineInterpolator::
BiCubicSplineInterpolator(const BiCubicSplineInterpolator & rhs)
{
  std::vector<Point> temp;
  int counter;
  try
    {
      interpolatortype = 10;
      rhs.getPoints(temp);
      Natural = rhs.Natural;
      Parabolic = rhs.Parabolic;
      SetDerivatives = rhs.SetDerivatives;
      NumberOfSplines = rhs.NumberOfSplines;

      if (SetDerivatives)
        {
          if (!(Ders = new (std::nothrow) double * [NumberOfSplines]))
            throw std::bad_alloc();
          
          for (counter = 0; counter < NumberOfSplines; counter++)
            {
              if (!(Ders[counter] = new (std::nothrow) double [2]))
                throw std::bad_alloc();
              
              Ders[counter][0] = rhs.Ders[counter][0];
              Ders[counter][1] = rhs.Ders[counter][1];
            }
        }

      setPoints(temp);
      isvalid = rhs.isvalid;
  }
  catch(...)
    {
    
    }
}


//*********************************************************************
BiCubicSplineInterpolator::~BiCubicSplineInterpolator()
{
  int counter;

  if (Splines)
    {
      for (counter = 0; counter < NumberOfSplines; counter++)
        delete [] Splines[counter];
      delete [] Splines;
    }
  if (Ders)
    {
      for (counter = 0; counter < NumberOfSplines; counter++)
        delete [] Ders[counter];
      delete [] Ders;
    }
}


//***********************************************************************
bool BiCubicSplineInterpolator::
setPoints(const std::vector<Point>& inPoints) throw()
{
  int counter;
  int size;
  Point * temppoints = NULL;
  bool success;
  try
    { 
      //create the temp points
      if (!(temppoints = new (std::nothrow) Point[inPoints.size()]))
        throw std::bad_alloc();

      size = static_cast<int>(inPoints.size());

      for (counter = 0; counter < size; counter++)
        temppoints[counter] = inPoints[counter];

      //call the overloaded set points
      success = setPoints(temppoints, size);
      delete [] temppoints;
      isvalid = success;
      return success;
    }
  catch(...)
    {
      delete [] temppoints;
      return false;
    }
}
      

//*************************************************************************
bool BiCubicSplineInterpolator::
setPoints(const Point * inPoints, const int& insize) throw() 
{
  int counter, bcounter;
  TNT::Matrix<double> temp; //for factorization
  TNT::Vector<double> Bvec;  
  TNT::Vector<double> solver; //solution vector
  TNT::Vector<double> C, D; //for QR
  TNT::Vector<TNT::Subscript> index; //for LU
  bool samegrid, qr, lu;
  int tempsqroot;
  double h, hsqr;
  int splinetopos;
  int xcounter;


  try
    {
      if (!Points && (insize == numpoints))
        {
          samegrid = true;
          for(counter = 0; counter < numpoints; counter++)
            if(Points[counter] != inPoints[counter])
              {
                samegrid = false;
                break;
              }
          if (samegrid)
            return true;
        }
      
      delete [] Points;
      numpoints = insize;
      if (!(Points = new (std::nothrow) Point[numpoints]))
        throw std::bad_alloc();
      
      for (counter = 0; counter < numpoints; counter++)
        Points[counter] = inPoints[counter];
      
            //figure out the number of segments and splines
      tempsqroot = static_cast<int>(sqrt(numpoints));//should be an even sqr
      PointsPerSpline = tempsqroot;
      SegmentsPerSpline = tempsqroot - 1;   
      NumberOfSplines = tempsqroot; 
      
      //resize the bvector
      Bvec.newsize(PointsPerSpline);
      
      //setup the matrix
      if (Natural)
        {
          MCoef.newsize(PointsPerSpline, PointsPerSpline);
          fillNaturalMatrix();
          
        }
      if (Parabolic)
        {
          MCoef.newsize(PointsPerSpline, PointsPerSpline);
          fillParabolicMatrix();
        }
      if (SetDerivatives)
        {
          MCoef.newsize(PointsPerSpline, PointsPerSpline);
          fillNaturalMatrix();
          //take care of the ders in the Bvec later
        }

      //compute h
      h = Points[1].x - Points[0].x;
      
      //do a divide by zero check
      if (h <= 0)
        {
          isvalid = false;
          return false;
        }
      //get hsqr
      hsqr = h*h;
      
      
      //create the splines
      if (Splines)
        {
          for (counter = 0; counter < NumberOfSplines; counter++)
            delete [] Splines[counter];
          
          delete [] Splines;
        }
      
      if (!(Splines = new (std::nothrow) double * [NumberOfSplines]))
        throw std::bad_alloc();
      
      for (counter = 0; counter < NumberOfSplines; counter++)
        {
          if (!(Splines[counter] = new (std::nothrow) 
                double [SegmentsPerSpline*4]))
            throw std::bad_alloc();
          
          //find this spline's array value in the points array
          splinetopos = counter * PointsPerSpline;
          
          //check to see if the ders were set
          if (SetDerivatives)
            {
              Bvec[0] = Ders[counter][0];
              Bvec[PointsPerSpline-1] = Ders[counter][1];
            }
          else
            {
              Bvec[0] = 0;
              Bvec[PointsPerSpline-1] = 0;
            }

          for(bcounter = 1; bcounter < PointsPerSpline-1; bcounter++)
            {
              Bvec[bcounter] = 6 * (Points[splinetopos].z 
                                    - 2*Points[splinetopos+1].z +
                                    Points[splinetopos + 2].z)/hsqr;
              splinetopos++;
            }

          
          //solve
          qr = true;
          lu = true; 
         
          temp = MCoef;
          index.newsize(PointsPerSpline);

          //try the lu first
          if (TNT::LU_factor(temp, index) != 0)
            {
              lu = false;
              C.newsize(PointsPerSpline);
              D.newsize(PointsPerSpline);
              if (TNT::QR_factor(temp, C, D) != 0)
               {
                 qr = false;
                 isvalid = false;
                 return false;
               }
           }
          
          if (!lu)
            { 
              if (TNT::QR_solve(temp, C, D, Bvec) != 0)
                {
                  isvalid = false;
                  return false;
                }
            }
          else
            if (TNT::LU_solve(temp, index, Bvec))
              {
                isvalid = false;
                return false;
              }
          
            
          //find this spline's array value in the points array
          splinetopos = counter * PointsPerSpline;
          xcounter = -1;
          for (bcounter = 0; bcounter < SegmentsPerSpline*4; bcounter++)
            {
              switch(bcounter%4)
                {
                case 0:
                  xcounter++;
                  Splines[counter][bcounter] = (Bvec[xcounter+1] 
                                               - Bvec[xcounter])/(6*h);
                  break;
                
                case 1:
                  Splines[counter][bcounter] = (Bvec[xcounter])/2;
                  break;
                 
                case 2:
                  Splines[counter][bcounter] = 
                    (Points[(xcounter+1)+splinetopos].z -
                     Points[(xcounter)+splinetopos].z)/h  -
                    h*(Bvec[xcounter+1]+ 2*Bvec[xcounter])/6;
                  break;
                case 3:
                  Splines[counter][bcounter] = 
                    Points[(xcounter)+splinetopos].z;
                  break;
                }
            }
        }
      
      //finished 
      isvalid = true;
      
      return true;
    }
  catch(...)
    {
      delete [] Points;
      Points = NULL;
      isvalid = false;
      return false;
    }
}
   




//**************************************************************************
bool BiCubicSplineInterpolator::
getPoints(std::vector<Point>& in) const throw()
{
  int counter;
  
  try
    {
      if(!numpoints)
      return false;
      
      in.resize(numpoints);
      
      for (counter= 0; counter < numpoints; counter++)
        in[counter] = Points[counter];
      
      return true;
    }
  catch(...)
    {
      return false;
    }
}


//***********************************************************************
Point BiCubicSplineInterpolator:: 
interpolatePoint(const Point& in) throw(MathException)
{
  double h, hsqr;
  int counter, bcounter;
  TNT::Matrix<double> temp;          //for factorization
  TNT::Vector<double> Bvec;  
  TNT::Vector<double> solver;        //solution vector
  TNT::Vector<double> C, D;          //for QR
  TNT::Vector<TNT::Subscript> index; //for LU
  double x, xsqr, xcub, y, ysqr, ycub;
  Point * Ypoints;
  int which;
  bool qr, lu;
  Point out;
 
  x = Points[0].x -1;
  for (counter = 1; counter < PointsPerSpline; counter++)
    {
      //find which spline segment we are in
      if (in.x <= Points[counter].x)
        {
          x = Points[counter-1].x;
          which = counter;
          break;
        }
    }

  //point out of bounds
  if (x == Points[0].x -1)
   throw MathException(3);//for some reason it does notlike OUT_OF_BOUNDS_ERROR
  
  x = (in.x - x);
  xsqr = x*x;
  xcub = xsqr*x;

  if (!(Ypoints = new (std::nothrow) Point[NumberOfSplines]))
    throw MathException();


  for (counter = 0; counter < NumberOfSplines; counter++)
    {
      Ypoints[counter].y = Points[counter*PointsPerSpline].y;
      Ypoints[counter].z = Splines[counter][(which-1)*4]
        *xcub;
      Ypoints[counter].z += Splines[counter]
        [(which-1)*4 + 1]*xsqr;
      Ypoints[counter].z += Splines[counter]
        [(which-1)*4 + 2]*x;
      Ypoints[counter].z += Splines[counter]
        [(which-1)*4 + 3];
    }

  //we should now have a set of y points to interpolate
  //so set up the Bvec
  //check to see if the ders were set
  Bvec.newsize(NumberOfSplines);
  if (SetDerivatives)
    {
      Bvec[0] = Ders[counter][0];
      Bvec[NumberOfSplines-1] = Ders[counter][1];
    }
  else
    {
      Bvec[0] = 0;
      Bvec[NumberOfSplines-1] = 0;
    }
  
  //recomput h
  h = Ypoints[1].y - Ypoints[0].y;
  hsqr = h*h;

  //setup the rest of the bvec
  for(bcounter = 1; bcounter < NumberOfSplines-1; bcounter++)
    {
      Bvec[bcounter] = 6 * (Ypoints[bcounter-1].z 
                            - 2*Ypoints[bcounter].z +
                            Ypoints[bcounter+1].z)/hsqr;
    }
  
  //solve
  qr = true;
  lu = true; 
         
  temp = MCoef;
  index.newsize(NumberOfSplines);

  //try the lu first
  if (TNT::LU_factor(temp, index) != 0)
    {
      lu = false;
      C.newsize(NumberOfSplines);
      D.newsize(NumberOfSplines);
      if (TNT::QR_factor(temp, C, D) != 0)
        {
          qr = false;
          isvalid = false;
          throw MathException(6);
        }
    }
  
  if (!lu)
    { 
      if (TNT::QR_solve(temp, C, D, Bvec) != 0)
        {
          isvalid = false;
          throw MathException(6);
        }
    }
  else
    if (TNT::LU_solve(temp, index, Bvec))
      {
        isvalid = false;
        throw MathException(2);
      }
  
  //figure out which y segment I am in
  y = Ypoints[0].y -1;
  for (counter = 1; counter < NumberOfSplines; counter++)
    {
      //find which spline segment we are in
      if (in.y >= Ypoints[counter].y)
        {
          y = Ypoints[counter].y;
          which = counter;
          break;
        }
    }

  //point out of bounds
  if (y == Ypoints[0].y -1)
    throw MathException(3);
  
  y = (in.y - y);
  ysqr = y*y;
  ycub = ysqr*y;

  //obtain final value
  out.z = ((Bvec[which] - Bvec[which-1])/(h*6))*ycub;
  out.z += ((Bvec[which-1])/2)*ysqr;
  out.z += ((Ypoints[which].z - Ypoints[which-1].z)/h - h*(Bvec[which] + 
            2*Bvec[which-1])/6)*y;
  out.z += Ypoints[which-1].z;
  out.x = in.x;
  out.y = in.y;
  return out;//done
}


//***********************************************************************
bool BiCubicSplineInterpolator::
operator==(const Interpolator& rhs) 
  const throw()
{
  int counter;
  bool samegrid = false;
  const BiCubicSplineInterpolator *temp =  
    dynamic_cast<const BiCubicSplineInterpolator*>(&rhs);
  
  if (!temp)
    return 0;
  
  if (Points && (numpoints == temp->numpoints))
  {
    if (SetDerivatives != temp->SetDerivatives)
      return false;
    else
      for (counter = 0; counter < 16; counter++)
        if (Ders[counter] != temp->Ders[counter])
           return false;
      
    samegrid = true; //assume true
    for (counter = 0; counter < numpoints; counter++)
      if (Points[counter] != temp->Points[counter])
      {
        samegrid = false; //reset flag
        break;
      }
  }
  
  return samegrid; //return results
}




bool BiCubicSplineInterpolator::
operator!=(const Interpolator& rhs) const throw()
{
  return !((*this) == rhs);
}

//***********************************************************************
void BiCubicSplineInterpolator::
setNatural() throw()
{
  Natural = true;
  SetDerivatives = false;
  Parabolic = false;
}
  
//************************************************************************
void BiCubicSplineInterpolator::
setParabolic() throw()
{
  Natural = false;
  SetDerivatives = false;
  Parabolic = true;
}

//************************************************************************
bool BiCubicSplineInterpolator::
setSecondDers(const std::vector<double>& inDers) throw()
{
  int counter;
  int which;
  try
    {
      //check for preexisting ders
      if (Ders)
        {
          for (counter = 0; counter < NumberOfSplines; counter++)
            delete [] Ders[counter];
          
          delete [] Ders;
        }
      
      NumberOfSplines = inDers.size();
      
      if (!(Ders = new (std::nothrow) double * [NumberOfSplines]))
        throw std::bad_alloc();
      
      which = 0; 
      for (counter = 0; counter < NumberOfSplines; counter++)
        {
          if (!(Ders[counter] = new (std::nothrow) double[2]))
            throw std::bad_alloc();
          
          Ders[counter][0] =inDers[which];
          Ders[counter][1] = inDers[which+1];
          which+=2;
        }
    
      SetDerivatives = true;
      return true;
      
    }
  catch(...)
    {
      SetDerivatives = false;
      return false;
    }
}
  
//*********************************************************************
bool BiCubicSplineInterpolator::
setSecondDers(const double * inDers, const int& insize) throw()
{
   int counter;
  int which;
  try
    {
      //check for preexisting ders
      if (Ders)
        {
          for (counter = 0; counter < NumberOfSplines; counter++)
            delete [] Ders[counter];
          
          delete [] Ders;
        }
      
      NumberOfSplines = insize;
      
      if (!(Ders = new (std::nothrow) double * [NumberOfSplines]))
        throw std::bad_alloc();
      
      which = 0; 
      for (counter = 0; counter < NumberOfSplines; counter++)
        {
          if (!(Ders[counter] = new (std::nothrow) double[2]))
            throw std::bad_alloc();
          
          Ders[counter][0] =inDers[which];
          Ders[counter][1] = inDers[which+1];
          which+=2;
        }
    
      SetDerivatives = true;
      return true;
      
    }
  catch(...)
    {
      SetDerivatives = false;
      return false;
    }
}


//*********************************************************************
void BiCubicSplineInterpolator::
fillNaturalMatrix() throw()
{
  int counter, counter2;
  int place;

  place = 0;
  //fill in the matrix, Yes I know what this looks like...
  for (counter = 0; counter < PointsPerSpline; counter++)
    {
      if (counter == 0)
        {
          //M0 = 0
          MCoef[0][0] = 1;
          //Fill the row out with zeros
          for(counter2 = 1; counter2 < PointsPerSpline; counter2++)
            MCoef[0][counter2] = 0;
        }
      
      if (counter == PointsPerSpline-1)
        {
          MCoef[counter][counter] = 1; //MN = 0
          //fill in zeros
          for(counter2 = PointsPerSpline-2; counter2 >= 0; counter2--)
            MCoef[counter][counter2] = 0;
        }
      
      if (counter != 0 && counter != PointsPerSpline-1)
        {
          for (counter2 = 0; counter2 < PointsPerSpline;  counter2++)
           {
             if (counter2 == place)
               {
                 MCoef[counter][counter2] = 1;
                 MCoef[counter][counter2+1] = 4;
                 MCoef[counter][counter2+2] = 1;
                 counter2 += 2; //move the counter to miss these
               }
             else
               MCoef[counter][counter2] = 0; 
        
           }
          place++;
        }
    }
  
}
 

//***************************************************************************
void BiCubicSplineInterpolator::
fillParabolicMatrix() throw()
{
  int counter, counter2;
  int place;

  place = 0;
  
  //fill in the matrix, Yes I know what this looks like...
  for (counter = 0; counter < PointsPerSpline; counter++)
    {
      if (counter == 0)
        {
          //M0 = MN or M0 - MN = 0
          MCoef[0][0] = 1;
          MCoef[0][1] = -1;
          //Fill the row out with zeros
          for(counter2 = 1; counter2 < PointsPerSpline; counter2++)
            MCoef[0][counter2] = 0;
        }
      
      if (counter == PointsPerSpline-1)
        {
          MCoef[counter][counter] = 1; //MN = M(N-1)
          MCoef[counter][counter-1] = -1; 
          //fill in zeros
          for(counter2 = PointsPerSpline-3; counter2 >= 0; counter2--)
            MCoef[counter][counter2] = 0;
        }
      
      if (counter != 0 && counter != PointsPerSpline-1)
        {
          for (counter2 = 0; counter2 < PointsPerSpline;  counter2++)
            {
              if (counter2 == place)
                {
                  MCoef[counter][counter2] = 1;
                  MCoef[counter][counter2+1] = 4;
                  MCoef[counter][counter2+2] = 1;
                  counter2 += 2; //move the counter to miss these
                }
              else
                MCoef[counter][counter2] = 0; 
              
            }
          place++;
        }
    }
  
}

#endif



















