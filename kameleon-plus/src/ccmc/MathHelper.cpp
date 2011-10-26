/*
 * Math.cpp
 *
 *  Created on: Nov 3, 2010
 *      Author: dberrios
 */

#include "MathHelper.h"
#include <limits>
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
	   max = std::numeric_limits<int>::min( ); //if lowest value possible in array is -99999999999
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
	   max = std::numeric_limits<float>::min( ); //if lowest value possible in array is -99999999999
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
	   max = std::numeric_limits<double>::min( ); //if lowest value possible in array is -99999999999
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
	   max = std::numeric_limits<int>::min( ); //if lowest value possible in array is -99999999999
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
	   max = std::numeric_limits<long>::min( ); //if lowest value possible in array is -99999999999
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
	   float max;
	   max = std::numeric_limits<float>::min(); //if lowest value possible in array is -1.e13
	   for (i=0; i<n; i++)
	   {
			  max = std::max(array[i] , max);

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

}
