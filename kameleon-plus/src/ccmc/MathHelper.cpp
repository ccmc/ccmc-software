/*
 * Math.cpp
 *
 *  Created on: Nov 3, 2010
 *      Author: dberrios
 */

#include "MathHelper.h"

namespace ccmc
{
	int Math::iminloc1d(int array[], long n, int mask[])
	{
	   long i,loc;
	   int min;
	   min = 999999999; //if highest value possible in array is 99999999999
	   for (i=0; i<n; i++)
		   {
				  if( (array[i] < min) && (mask[i] > 0)) {
					 min=array[i];
					 loc=i;
				  }
		   }
	   return loc;
	}


	int Math::imaxloc1d(int array[], long n, int mask[])
	{
	   long i,loc;
	   int max;
	   max = -999999999; //if lowest value possible in array is -99999999999
	   for (i=0; i<n; i++)
		   {
				  if( (array[i] > max) && (mask[i] > 0)) {
					 max=array[i];
					 loc=i;
				  }
		   }
	   return loc;
	}

	int Math::fminloc1d(float array[], long n, int mask[])
	{
	   long i,loc;
	   float min;
	   min = 999999999.; //if highest value possible in array is 99999999999
	   for (i=0; i<n; i++)
		   {
				  if( (array[i] < min) && (mask[i] > 0)) {
					 min=array[i];
					 loc=i;
				  }
		   }
	   return loc;
	}


	int Math::fmaxloc1d(float array[], long n, int mask[])
	{
	   long i,loc;
	   float max;
	   max = -999999999.; //if lowest value possible in array is -99999999999
	   for (i=0; i<n; i++)
		   {
				  if( (array[i] > max) && (mask[i] > 0)) {
					 max=array[i];
					 loc=i;
				  }
		   }
	   return loc;
	}

	int Math::dminloc1d(double array[], long n, int mask[])
	{
	   long i,loc;
	   double min;
	   min = 999999999.; //if highest value possible in array is 99999999999
	   for (i=0; i<n; i++)
		   {
				  if( (array[i] < min) && (mask[i] > 0)) {
					 min=array[i];
					 loc=i;
				  }
		   }
	   return loc;
	}


	int Math::dmaxloc1d(double array[], long n, int mask[])
	{
	   long i,loc;
	   double max;
	   max = -999999999.; //if lowest value possible in array is -99999999999
	   for (i=0; i<n; i++)
		   {
				  if( (array[i] > max) && (mask[i] > 0)) {
					 max=array[i];
					 loc=i;
				  }
		   }
	   return loc;
	}


	int Math::ifindmin(int array[], long n)
	{
	   long i;
	   int min;
	   min = 999999999; //if highest value possible in array is 99999999999
	   for (i=0; i<n; i++)
		   {
				  if(array[i] < min)
					 min=array[i];
		   }
	   return min;
	}

	int Math::ifindmax(int array[], long n)
	{
	   long i;
	   int max;
	   max = -999999999; //if lowest value possible in array is -99999999999
	   for (i=0; i<n; i++)
		   {
				  if(array[i] > max)
					 max=array[i];
		   }
	   return max;
	}

	long Math::lfindmin(long array[], long n)
	{
	   long i;
	   long min;
	   min = 99999999999999; //if highest value possible in array is 99999999999
	   for (i=0; i<n; i++)
		   {
				  if(array[i] < min)
					 min=array[i];
		   }
	   return min;
	}

	long Math::lfindmax(long array[], long n)
	{
	   long i;
	   long max;
	   max = -99999999999999; //if lowest value possible in array is -99999999999
	   for (i=0; i<n; i++)
		   {
				  if(array[i] > max)
					 max=array[i];
		   }
	   return max;
	}

	float Math::ffindmin(float array[], long n)
	{
	   long i;
	   float min;
	   min = 1.e13; //if highest value possible in array is 1.e13
	   for (i=0; i<n; i++)
		   {
				  if(array[i] < min)
					 min=array[i];
		   }
	   return min;
	}

	float Math::ffindmax(float array[], long n)
	{
	   long i;
	   float max;
	   max = -1.e13; //if lowest value possible in array is -1.e13
	   for (i=0; i<n; i++)
		   {
				  if(array[i] > max)
					 max=array[i];
		   }
	   return max;
	}

	double Math::dfindmin(double array[], long n)
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

	double Math::dfindmax(double array[], long n)
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
