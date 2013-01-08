/*
 * Math.cpp
 *
 *  Created on: Nov 3, 2010
 *      Author: dberrios
 */

#include "MathHelper.h"
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/limits.hpp>
#include <cmath>
#include <algorithm>

namespace ccmc
{
	int Math::iminloc1d(int * array, long n, int * mask)
	{
	   long i,loc;
	   int min;
	   min = std::numeric_limits<int>::max( ); //if highest value possible in array is 99999999999
	   for (i=0; i<n; i++)
		   {
				  if( (array[i] < min) && (mask[i] > 0)) {
					 min=array[i];
					 loc=i;
				  }
		   }
	   return loc;
	}


	int Math::imaxloc1d(int * array, long n, int * mask)
	{
	   long i,loc;
	   int max;
	   max = boost::numeric::bounds<int>::lowest(); //if lowest value possible in array is -99999999999
	   for (i=0; i<n; i++)
		   {
				  if( (array[i] > max) && (mask[i] > 0)) {
					 max=array[i];
					 loc=i;
				  }
		   }
	   return loc;
	}

	int Math::fminloc1d(float * array, long n, int * mask)
	{
		long i,loc;
		float min;
		min = std::numeric_limits<float>::max( ); //if highest value possible in array is 99999999999
		for (i=0; i<n; i++)
		{
			if( (array[i] < min) && (mask[i] > 0))
			{
				min=array[i];
				loc=i;
			}
		}
		return loc;
	}


	int Math::fmaxloc1d(float * array, long n, int * mask)
	{
	   long i,loc;
	   float max;
	   max = boost::numeric::bounds<float>::lowest(); //if lowest value possible in array is -99999999999
	   for (i=0; i<n; i++)
		   {
				  if( (array[i] > max) && (mask[i] > 0)) {
					 max=array[i];
					 loc=i;
				  }
		   }
	   return loc;
	}

	int Math::dminloc1d(double * array, long n, int * mask)
	{
	   long i,loc;
	   double min;
	   min = std::numeric_limits<double>::max( ); //if highest value possible in array is 99999999999
	   for (i=0; i<n; i++)
		   {
				  if( (array[i] < min) && (mask[i] > 0)) {
					 min=array[i];
					 loc=i;
				  }
		   }
	   return loc;
	}


	int Math::dmaxloc1d(double * array, long n, int * mask)
	{
	   long i,loc;
	   double max;
	   max = boost::numeric::bounds<double>::lowest(); //if lowest value possible in array is -99999999999
	   for (i=0; i<n; i++)
		   {
				  if( (array[i] > max) && (mask[i] > 0)) {
					 max=array[i];
					 loc=i;
				  }
		   }
	   return loc;
	}


	int Math::ifindmin(int * array, long n)
	{
	   long i;
	   int min;
	   min = std::numeric_limits<int>::max( ); //if highest value possible in array is 99999999999
	   for (i=0; i<n; i++)
		   {
				  if(array[i] < min)
					 min=array[i];
		   }
	   return min;
	}

	int Math::ifindmax(int * array, long n)
	{
	   long i;
	   int max;
	   max = boost::numeric::bounds<int>::lowest(); //if lowest value possible in array is -99999999999
	   for (i=0; i<n; i++)
		   {
				  if(array[i] > max)
					 max=array[i];
		   }
	   return max;
	}

	long Math::lfindmin(long * array, long n)
	{
	   long i;
	   long min;
	   min = std::numeric_limits<long>::max( ); //if highest value possible in array is 99999999999
	   for (i=0; i<n; i++)
		   {
				  if(array[i] < min)
					 min=array[i];
		   }
	   return min;
	}

	long Math::lfindmax(long * array, long n)
	{
	   long i;
	   long max;
	   max = boost::numeric::bounds<long>::lowest(); //if lowest value possible in array is -99999999999
	   for (i=0; i<n; i++)
		   {
				  if(array[i] > max)
					 max=array[i];
		   }
	   return max;
	}

	float Math::ffindmin(float * array, long n)
	{
	   long i;
	   float min = std::numeric_limits<float>::max(); //if highest value possible in array is 1.e13
	   for (i=0; i<n; i++)
	   {
			  min = std::min(array[i], min);

	   }
	   return min;
	}

	float Math::ffindmax(float * array, long n)
	{
	   long i;
	   float max = -1.e20;
	   max = boost::numeric::bounds<float>::lowest(); //if lowest value possible in array is -1.e13
	   for (i=0; i<n; i++)
	   {
		   max = std::max((double)array[i] , (double)max);

	   }
	   return max;
	}

	double Math::dfindmin(double * array, long n)
	{
	   long i;
	   double min;
	   min = 1.e13; //if highest value possible in array is 1.e13
	   for (i=0; i<n; i++)
		   {
				  if(array[i] < min)
					 min=array[i];
		   }
	   return min;
	}

	double Math::dfindmax(double * array, long n)
	{
	   long i;
	   double max;
	   max = -1.e13; //if lowest value possible in array is -1.e13
	   for (i=0; i<n; i++)
		   {
				  if(array[i] > max)
					 max=array[i];
		   }
	   return max;
	}

	void Math::convert_xyz_to_rthetaphi(const float& x, const float& y, const float& z,
			float * r, float * t, float * p)
	{
	/*
	 Converts 3D cartesian coords (x,y,z) into spherical coords (r,theta,phi).
	*/

	      float  piov2,r2,zr,yx;

	      piov2 = std::acos(0.);

	      r2 = x*x+y*y+z*z;
	      if(r2 > 0.) {
	        *r  = std::sqrt(r2);
	        zr=z/ (*r);
	        *t = acos(zr);
	        if(x > 0.) {
	          yx=y/x;
	          *p = std::atan(yx);
	        } else if(x < 0.) {
	          yx=y/std::fabs(x);
	          *p = 2.*piov2 - atan(yx);
	        } else {
	          *p = piov2*sign(y);
	        }
	      } else {
	        *r = 0.;
	        *t = 0.;
	        *p = 0.;
	      }
	      if(*p < 0.) *p=*p+4.*piov2;               /* pmn testing */
	}

	void Math::convert_rthetaphi_to_xyz(const float& r, const float& t, const float& p,
			float * x, float * y, float * z)
	{
	/*
	  Converts spherical coords (r,theta,phi) into 3D cartesian coords (x,y,z)
	*/

	      float  sint,cost,sinp,cosp;

	      sint = std::sin(t);
	      cost = std::cos(t);
	      sinp = std::sin(p);
	      cosp = std::cos(p);

	      *x = r*sint*cosp;
	      *y = r*sint*sinp;
	      *z = r*cost;
	}

	int Math::sign(int v)
	{
	   return v > 0 ? 1 : (v < 0 ? -1 : 0);
	}

}
