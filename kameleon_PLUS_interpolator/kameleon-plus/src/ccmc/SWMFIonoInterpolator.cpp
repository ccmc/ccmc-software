//
//  SWMFInterpolator.cpp
//  
//
//  Created by Nitesh Donti on 1/11/12.
//  Copyright 2012 Cornell University. All rights reserved.
//

#include "SWMFIonoInterpolator.h"
#include "SWMFIono.h"
#include "StringConstants.h"
#include "Constants.h"
#include "Utils.h"

namespace ccmc {

SWMFInterpolator::SWMFInterpolator(Model * model){

	this->modelReader = model;
	this->setMissingValue(this->modelReader->getMissingValue());

	r_string = ccmc::strings::variables::r_;
	th_string = ccmc::strings::variables::theta_;
	phi_string = ccmc::strings::variables::phi_;

	r_data = modelReader->getVariableData(r_string);
	th_data = modelReader->getVariableData(th_string);
	phi_data = modelReader->getVariableData(phi_string);

	previous_r = missingValue;
	previous_th = missingValue;
	previous_phi = missingValue;

}

float SWMFInterpolator::interpolate(const std::string& variable, const float& r, const float& lat, const float& lon){

	float dr, dlat, dlon;
	long variable_id = modelReader->getVariableID(variable);

	return interpolate(variable_id, r, lat, lon, dr, dlat, dlon);
}

float SWMFInterpolator::interpolate(const std::string& variable, const float& r, const float& lat, const float& lon, float& dr, float& dlat, float& dlon){

	long variable_id = modelReader->getVariableID(variable);
	return interpolate(variable_id, r, lat, lon, dr, dlat, dlon);
}

float SWMFInterpolator::interpolate(const long& variable_id, const float& r, const float& lat, const float& lon){

	//r   = r
	//lat = theta (up to down)
	//lon = phi (left to right)


	float dc0, dc1, dc2;
	return interpolate(variableID, r, lat, lon, dc0, dc1, dc2);

	//here I am taking a point and estimating its value linearly using the
	//confirmed values of the four points existing at its bottom left, top left, top right, and bottom right, in a square
	const std::vector<float> * data = modelReader->getVariableByID(variable_id);

	float missingvalue;

	/*
	So for each point I will have a co-latitude and a longitude. The radius I can assume is some constant.
	Then, the value that I need will come from basically any another variable, for example e-flux.

	Point 1: the point with the closest phi value that is less than LON
	Point 2: the point with the close phi value that is greater than LON
	Point 3: the point with the closest theta value that is less than LAT
	Point 4: the point with the close theta value that is greater than LAT
	*/

	float v1,t1,p1;
	float v2,t2,p2;
	float v3,t3,p3;
	float v4,t4,p4;


	float * pt1 = {3,4,15};
	float * pt2 = {9,7,25};
	float * pt3 = {23,14,5};
	float * pt4 = {7,24,25};




	14 (r,theta,phi)  15

	   *

			               7
 4
	 */

	return missingvalue;

}
float interpolate(const long& variable_id, const float& r, const float& lat, const float& lon, float& dr, float& dlat, float& dlon){

}



}
