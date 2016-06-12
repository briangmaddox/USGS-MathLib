
#ifndef _BICUBICINTERPOLATOR_CPP_
#define _BICUBICINTERPOLATOR_CPP_

#include "BiCubicInterpolator.h"


using namespace MathLib;
//***************************************************************************
BiCubicInterpolator::BiCubicInterpolator() : Interpolator()
{
  DerivativesSet = false; 
  Minimize = false;
  interpolatortype = 9;
  isvalid = true;
}

//**************************************************************************** 
BiCubicInterpolator::BiCubicInterpolator(const BiCubicInterpolator & rhs)
{
  std::vector<Point> temp;
  int counter;
  try
    {
      interpolatortype = 9;
      rhs.getPoints(temp);
      
      setPoints(temp);
      DerivativesSet = rhs.DerivativesSet;
      Minimize = rhs.Minimize;
      isvalid = rhs.isvalid;
      for (counter = 0; counter < 4; counter++)
        Derivatives[counter] = rhs.Derivatives[counter];
  }
  catch(...)
    {
    
    }
}

//**************************************************************************
BiCubicInterpolator::~BiCubicInterpolator()
{
}
    
//**************************************************************************
bool BiCubicInterpolator::
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



//***************************************************************************
bool BiCubicInterpolator::
setPoints(const Point * inPoints, const int& insize) throw()
{
  TNT::Matrix<double> A;
  TNT::Vector<double> B;
  TNT::Vector<TNT::Subscript> index;
  TNT::Vector<double> D;
  TNT::Vector<double> C;
  TNT::Vector<double> solver;
  double x, y, xsqr, ysqr, ycub, xcub;
  bool qr, lu;
  int counter;
  bool samegrid;
  
  try
  {
    if (Points && (insize == numpoints))
      {
        samegrid = true;
        for (counter = 0; counter < numpoints; counter++)
          if (Points[counter] != inPoints[counter])
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
  
       //resize the matrix, and create the index
      A.newsize(16, 16);
      B.newsize(16);
      index.newsize(16);
      //check to see if we need to calc the derivatives
      if (numpoints == 4)
        {
          //fill in the first four lines
          for (counter = 0; counter < 4; counter++)
            {
           
              x = Points[counter].x;
              y = Points[counter].y;
              xsqr = x*x;
              ysqr = y*y;
              ycub = y*ysqr;
              xcub = x*xsqr;
              A[counter][0] = 1;          //a00
              A[counter][1] = x;          //a10
              A[counter][2] = y;          //a01
              A[counter][3] = x*y;        //a11
              A[counter][4] = xsqr;       //a20
              A[counter][5] = ysqr;       //a02
              A[counter][6] = xsqr*y;     //a21
              A[counter][7] = x*ysqr;     //a12
              A[counter][8] = xsqr*ysqr;  //a22
              A[counter][9] = xcub;       //a30
              A[counter][10]= ycub;       //a03
              A[counter][11]= xcub*y;     //a31
              A[counter][12]= x*ycub;     //a13
              A[counter][13]= xcub*ysqr;  //a32
              A[counter][14]= xsqr*ycub;  //a23
              A[counter][15]= xcub*ycub;  //a33
              B[counter] = Points[counter].z;
            }
          
          //now see if the derivates have been set
          if (!DerivativesSet)
            {
              //calculate the  (Dz/dx)
              Derivatives[0].x = (Points[1].z - Points[0].z)/
                (Points[1].x - Points[0].x);
              
              Derivatives[1].x = Derivatives[0].x;
              
              Derivatives[2].x = (Points[3].z - Points[2].z)/
                (Points[3].x - Points[2].x);
              
              Derivatives[3].x = Derivatives[2].x;
              
              //calculate the  (Dx/dz)
              Derivatives[0].y = (Points[0].z - Points[2].z)/
                (Points[0].y - Points[2].y);
              
              Derivatives[1].y = (Points[1].z - Points[3].z)/
                (Points[1].y - Points[3].y);
              
              Derivatives[2].y = Derivatives[0].y;
              
              Derivatives[3].y = Derivatives[1].y;
              
              //fill the last
              for (counter = 0; counter < 4; counter++)
                Derivatives[counter].z = 0;
              
              DerivativesSet = true;
            }
          
          //copy the derivatives
          for (counter = 4; counter < 8; counter++)
            {
              B[counter] = Derivatives[counter-4].x;
              B[counter+4] = Derivatives[counter-4].y;
              B[counter+8] = Derivatives[counter-4].z;
            }
          //now fill in the rest of coefficent matrix
          for (counter = 4; counter < 8; counter++)
            {
              x = Points[counter-4].x;
              y = Points[counter-4].y;
              xsqr = x*x;
              ysqr = y*y;
              ycub = y*ysqr;
              xcub = x*xsqr;
              
              //dz/dxs
              A[counter][0] = 0;          //a00
              A[counter][1] = 1;          //a10
              A[counter][2] = 0;          //a01
              A[counter][3] = y;          //a11
              A[counter][4] = 2*x;        //a20
              A[counter][5] = 0;          //a02
              A[counter][6] = 2*x*y;      //a21
              A[counter][7] = ysqr;       //a12
              A[counter][8] = 2*x*ysqr;   //a22
              A[counter][9] = 3*xsqr;     //a30
              A[counter][10]= 0;          //a03
              A[counter][11]= 3*xsqr*y;   //a31
              A[counter][12]= ycub;       //a13
              A[counter][13]= 3*xsqr*ysqr;//a32
              A[counter][14]= 2*x*ycub;   //a23
              A[counter][15]= 3*xsqr*ycub;//a33
             
              //dz/dys
              A[counter+4][0] = 0;          //a00
              A[counter+4][1] = 0;          //a10
              A[counter+4][2] = 1;          //a01
              A[counter+4][3] = x;          //a11
              A[counter+4][4] = 0;          //a20
              A[counter+4][5] = 2*y;        //a02
              A[counter+4][6] = xsqr;       //a21
              A[counter+4][7] = x*2*y;      //a12
              A[counter+4][8] = xsqr*2*y;   //a22
              A[counter+4][9] = 0;          //a30
              A[counter+4][10]= 3*ysqr;     //a03
              A[counter+4][11]= xcub;       //a31
              A[counter+4][12]= x*3*ysqr;   //a13
              A[counter+4][13]= xcub*2*y;   //a32
              A[counter+4][14]= xsqr*3*ysqr;//a23
              A[counter+4][15]= xcub*3*ysqr;//a33
              
              //dz^2/dxdy
              A[counter+8][0] = 0;          //a00
              A[counter+8][1] = 0;          //a10
              A[counter+8][2] = 0;          //a01
              A[counter+8][3] = 1;          //a11
              A[counter+8][4] = 0;          //a20
              A[counter+8][5] = 0;          //a02
              A[counter+8][6] = 2*x;        //a21
              A[counter+8][7] = 2*y;        //a12
              A[counter+8][8] = 4*x*y;      //a22
              A[counter+8][9] = 0;          //a30
              A[counter+8][10]= 0;          //a03
              A[counter+8][11]= 3*xsqr;     //a31
              A[counter+8][12]= 3*ysqr;     //a13
              A[counter+8][13]= 6*xsqr*y;   //a32
              A[counter+8][14]= 6*x*ysqr;   //a23
              A[counter+8][15]= 9*xsqr*ysqr;//a33
            }
        }
      else
        {
          if (numpoints != 16)
            {
              DerivativesSet = false;
              isvalid = false;
              return false;
            }
          
          //fillin the array
            //fill in the first four lines
          for (counter = 0; counter < 16; counter++)
            {
           
              x = Points[counter].x;
              y = Points[counter].y;
  
              xsqr = x*x;
              ysqr = y*y;
              ycub = y*ysqr;
              xcub = x*xsqr;
              A[counter][0] = 1;          //a00
              A[counter][1] = x;          //a10
              A[counter][2] = y;          //a01
              A[counter][3] = x*y;        //a11
              A[counter][4] = xsqr;       //a20
              A[counter][5] = ysqr;       //a02
              A[counter][6] = xsqr*y;     //a21
              A[counter][7] = x*ysqr;     //a12
              A[counter][8] = xsqr*ysqr;  //a22
              A[counter][9] = xcub;       //a30
              A[counter][10]= ycub;       //a03
              A[counter][11]= xcub*y;     //a31
              A[counter][12]= x*ycub;     //a13
              A[counter][13]= xcub*ysqr;  //a32
              A[counter][14]= xsqr*ycub;  //a23
              A[counter][15]= xcub*ycub;  //a33
              B[counter] = Points[counter].z;
            }
        }

      //okay we should have came out of that mess with a 16x16 matrix
      //and a apporiate B vector
      TNT::Matrix<double> Temp(A); //temp for solving
      D.newsize(16);
      C.newsize(16);
      qr = true;
      lu = true; // This will be for minimization when I get to it
      solver = B;
      
      //check minimize
      if (Minimize)
        {
          getMin(A, B, solver);
          for (counter = 0; counter < 16; counter++)
            Coef[counter] = solver[counter];
          return true;
        }


      //try the lu first
      if (TNT::LU_factor(Temp, index) != 0)
        {
          lu = false;
          if (TNT::QR_factor(Temp, C, D) != 0)
            {
              qr = false;
              DerivativesSet = false;
              isvalid = false;
              return false;
            }
        }
      
      if (!lu)
        { 
          if (TNT::QR_solve(Temp, C, D, solver) != 0)
            {
              DerivativesSet = false;
              isvalid = false;
              return false;
            }
        }
      else
        if (TNT::LU_solve(Temp, index,  solver))
          {
             DerivativesSet = false;
             isvalid = false;
             return false;
          }
      
      for (counter =0; counter < 16; counter++)
        Coef[counter] = solver[counter];

      //if we made it this far it must be all good
      return true;

    }

  catch(...)
    {
      delete [] Points;
      Points = NULL;
      isvalid = false;
      DerivativesSet = false;
      return false;
    }
}


//***************************************************************************
bool BiCubicInterpolator::getPoints(std::vector<Point>& in) const throw()
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

//************************************************************************
Point BiCubicInterpolator::
interpolatePoint(const Point& in) throw(MathException)
{
  Point temp;
  double x,y,xsqr, xcub, sub;
  try
    {/* Want
     	               		               3
         ( y( y( a33 y  + a32)  + a31) + a30) x

	 	  		        2	      	      
 + ( y( y(a23 y  + a22)  + a21) + a20) x  + (y(y(a13 y  + a12)  + a11) + a10) x

		 
 + y (y (a03 y  + a02)  + a01) + a00 = z
     */
      x = in.x;
      y = in.y;
      xsqr = x*x;
      xcub = xsqr*x;

      //do the x^3 term
      sub = Coef[15]*y + Coef[13];  //a33 y + a32
      sub = y*sub + Coef[11];       //y*(a33*y + a32) + a31
      sub = y*sub + Coef[9];        //( y( y( a33 y  + a32)  + a31) + a30)
      sub = xcub*sub;
      temp.z = sub;

      //do the x^2 term
      sub = Coef[14]*y + Coef[8]; //a23 y  + a22
      sub = y*sub + Coef[6];      // (y(a23 y  + a22)  + a21)
      sub = y*sub + Coef[4];      //( y( y(a23 y  + a22)  + a21) + a20)
      sub = xsqr * sub;
      temp.z += sub;

      //do the x term (y(y(a13 y  + a12)  + a11) + a10) x
      sub = Coef[12]*y + Coef[7];
      sub = y*sub + Coef[3];
      sub = y*sub + Coef[1];
      sub = x*sub;
      temp.z += sub;

      //do the _ term  y (y (a03 y  + a02)  + a01) + a00
      sub = Coef[10]*y + Coef[5];
      sub = y*sub + Coef[2];
      sub = y*sub + Coef[0];
      temp.z += sub;
      /*
      temp.z =  Coef[0];              //a00
      temp.z += Coef[1]*x;            //a10
      temp.z += Coef[2]*y;            //a01
      temp.z += Coef[3]*x*y;          //a11
      temp.z += Coef[4]*xsqr;         //a20
      temp.z += Coef[5]*ysqr;         //a02
      temp.z += Coef[6]*xsqr*y;       //a21
      temp.z += Coef[7]*x*yqr;        //a12
      temp.z += Coef[8]*xsqr*ysqr;    //a22
      temp.z += Coef[9]*xcub;         //a30
      temp.z += Coef[10]*ycub;        //a03
      temp.z += Coef[11]*xcub*y;      //a31
      temp.z += Coef[12]*x*ycub;      //a13
      temp.z += Coef[13]*xcub*ysqr;   //a32
      temp.z += Coef[14]*xsqr*ycub;   //a23
      temp.z += Coef[15]*xcub*ycub;   //a33
      */
      temp.x = in.x;
      temp.y = in.y;
      return temp;
    }
  catch(...)
    {
      throw MathException();
    }
}
            
//***************************************************************************
bool BiCubicInterpolator::
operator==(const Interpolator& rhs) const throw()
{
  int counter;
  bool samegrid = false;
  const BiCubicInterpolator *temp = 
    dynamic_cast<const BiCubicInterpolator*>(&rhs);
  
  if (!temp)
    return 0;
  
  if (Points && (numpoints == temp->numpoints))
  {
    if (DerivativesSet != temp->DerivativesSet)
      return false;
    else
      for (counter = 0; counter < 16; counter++)
        if (Derivatives[counter] != temp->Derivatives[counter])
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

//**********************************************************************
bool BiCubicInterpolator::
operator!=(const Interpolator& rhs) const throw()
{
  return !((*this) == rhs);
}

//**********************************************************************
bool BiCubicInterpolator::
setDerivatives(const std::vector<Point>& inDerivatives) throw()
{
  unsigned int counter;
  if (inDerivatives.size() != 4)
    return false;
  
  for (counter = 0; counter < inDerivatives.size(); counter++)
    Derivatives[counter] = inDerivatives[counter];
  
  DerivativesSet = true;
  return true;
}

//**********************************************************************
bool BiCubicInterpolator::
setDerivatives(const Point *inDerivatives) throw()
{
  int counter;
  
  for (counter = 0; counter < 4; counter++)
    Derivatives[counter] = inDerivatives[counter];
  
  DerivativesSet = true;
  return true;
}

//**********************************************************************
void BiCubicInterpolator::setMinimize() throw()
{
  Minimize = true;
}

//**********************************************************************
void BiCubicInterpolator::
getMin(const TNT::Matrix<double>& inMat, const TNT::Vector<double> B,
              TNT::Vector<double>& coefs) throw (MathException)
{
  TNT::Matrix<double> temp;              //temp for solving
  TNT::Vector<TNT::Subscript> index;     //index for LU
  TNT::Vector<double> D;                 //for QR
  TNT::Vector<double> C;                 //for QR
  TNT::Vector<double> solveLu;           //lu solution
  TNT::Vector<double> solveQr;           //Qr solution
  double LUlength, QRlength;
  int counter;

  try
    {
      //resize temps
      D.newsize(16);
      C.newsize(16);
      solveLu.newsize(16);
      solveQr.newsize(16);
      index.newsize(16);

      //try the LU first
      solveLu = B;
      temp = inMat;
      
      if (TNT::LU_factor(temp, index) != 0)
        {
          //can't compare
          throw MathException();
        }
      
      if (TNT::LU_solve(temp, index, solveLu))
        {
          //can't compare
          throw MathException();
        }
      
      //now try the QR
      solveQr = B;
      temp = inMat;
      if (TNT::QR_factor(temp, C, D) != 0)
        {
          throw MathException();
        }
      
      if (TNT::QR_solve(temp, C, D, solveQr) != 0)
        {
          throw MathException();
        }
      
      LUlength = 0;
      QRlength = 0;
      for (counter = 0; counter < 16; counter++)
        {
          LUlength += solveLu[counter]*solveLu[counter];
          QRlength += solveQr[counter]*solveQr[counter];
        }
      
      //get the smallest, sqrt perserves order so no need for it
      if (LUlength >= QRlength)
        coefs = QRlength;
      else
        coefs = LUlength;
      
    }
  catch(...)
    {
      throw MathException();
    }
}
     
      



#endif







