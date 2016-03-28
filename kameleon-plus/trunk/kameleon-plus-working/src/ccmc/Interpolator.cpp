/*
 * Interpolator.cpp
 *
 *  Created on: Jun 30, 2009
 *      Author: dberrios
 */

#include "Kameleon.h" //should be guarded by ifdef
#include "Interpolator.h"
#include <iostream>
using namespace std;
namespace ccmc
{
	/**
	 * Default constructor
	 */
	Interpolator::Interpolator()
	{
		// TODO Auto-generated constructor stub

	}

	/**
	 * @param variable
	 * @return
	 */
	float Interpolator::getConversionFactor(const std::string& variable)
	{
		//cout << "Inside Interpolator::getConversionFactor" << endl;
		return 1.0f;
	}

	/**
	 * @param variable_id
	 * @return
	 */
	float Interpolator::getConversionFactor(const long& variable_id)
	{
		return 1.0f;
	}

	std::string Interpolator::getModelName()
	{
		return this->modelReader->getModelName();
	}

	/*
	 * Sets the model reader for the interpolator. This allows the user to use the same
	 * interpolator on different input, since interpolator initialization can be expensive
	 */
	void Interpolator::setModelReader(Model * reader)
	{
		this->modelReader = reader;
	}


	void Interpolator::convertCoordinates(const std::string& source, const std::string& dest, 
				const long time_et,
				const float& c0, const float& c1, const float& c2, 
				float& dc0, float& dc1, float& dc2)
	{
		// std::cout << "Interpolator::convertCoordinates called" << std::endl;
		Position preferred_p = {c0, c1, c2};
		Position target_p;

		if (source == "NATIVE"){ // use input position as output
			target_p = preferred_p; 
		} else if (dest == "UNKNOWN"){
			// do nothing		
		} else { 
			Kameleon::_cxform(source.c_str(), dest.c_str(), time_et, &preferred_p, &target_p);
		}
		dc0 = target_p.c0;
		dc1 = target_p.c1;
		dc2 = target_p.c2;	


		// move below to Enlil interpolator
		// 	} else if (dest == "HNM"){ //Enlil
		// // convert to cartesian HEEQ
		// Kameleon::_cxform(source.c_str(), "HEEQ", time_et, v_in, v_out);

		// //convert to spherical (r,theta, phi) 
		
		// // convert to HNM (r0,lat_grid,lon_grid,)

	}

	/**
	 * Destructor
	 */
	Interpolator::~Interpolator()
	{
		// TODO Auto-generated destructor stub
	}
}
