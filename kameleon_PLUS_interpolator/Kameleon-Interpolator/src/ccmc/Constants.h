/*
 * Constants.h
 *
 *  Created on: Jul 2, 2009
 *      Author: dberrios
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

//TODO: Finish adding constants.
//TODO: Put \mainpage in one of hte source files
//TODO: Fix the conversionFactors
//TODO: Figure out some way of threading the problem.  Also figure out
//      How to improve the performance if the previous variable always changes.
//TODO: Keep previously requested variable, and a queue of the base variables!
namespace ccmc
{
	namespace constants
	{
		float Boltzmann = 1.3806503e-23f;
		float Avogadro = 6.0221415e23f;
		float Pi = 3.14159265;
		float AU_in_meters = 149598.e6;
		float Radian_in_degrees = 57.2957795;
		float Degree_in_radians = 0.0174532925;

	}
}


#endif /* CONSTANTS_H_ */
