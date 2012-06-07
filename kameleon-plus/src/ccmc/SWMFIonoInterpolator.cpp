/*
 * SWMFIonoInterpolator.cpp
 *
 *  Created on: May 25, 2012
 *      Author: David Berrios
 */

#include "SWMFIonoInterpolator.h"
#include "SWMFIono.h"
#include "StringConstants.h"
#include "Constants.h"
#include "Utils.h"
#include <math.h>

namespace ccmc {

	SWMFIonoInterpolator::SWMFIonoInterpolator(Model * model)
	{

		this->modelReader = model;
		modelReader->setMissingValue(this->modelReader->getMissingValue());

		//latitude
		th_string = ccmc::strings::variables::theta_;

		//longitude
		psi_string = ccmc::strings::variables::psi_;
		psi_size = modelReader->getGlobalAttribute("psi_size").getAttributeInt();
		theta_size = modelReader->getGlobalAttribute("theta_size").getAttributeInt();

		th_data = modelReader->getVariableFromMap(th_string);
		psi_data = modelReader->getVariableFromMap(psi_string);
		float missingValue = this->modelReader->getMissingValue();
		previous_th = missingValue;
		previous_psi = missingValue;
		//previous_phi = missingValue;

	}

	float SWMFIonoInterpolator::interpolate(const std::string& variable, const float& r, const float& lat, const float& lon){

		float dr, dlat, dlon;
		long variable_id = modelReader->getVariableID(variable);

		return interpolate(variable_id, r, lat, lon, dr, dlat, dlon);
	}

	float SWMFIonoInterpolator::interpolate(const std::string& variable, const float& r, const float& lat, const float& lon, float& dr, float& dlat, float& dlon){

		long variable_id = modelReader->getVariableID(variable);
		return interpolate(variable_id, r, lat, lon, dr, dlat, dlon);
	}

	/**
	 * @param r This parameter is unused, since the grid is 2D.
	 * @param lat The latitude component of the position.
	 * @param lon The longitude component of the position.
	 */
	float SWMFIonoInterpolator::interpolate(const long& variableID, const float& r, const float& lat, const float& lon){

		//r   = r
		//lat = theta (up to down)
		//lon = phi (left to right)


		float dc0, dc1, dc2;
		return interpolate(variableID, r, lat, lon, dc0, dc1, dc2);



	}

	float SWMFIonoInterpolator::interpolate(const long& variableID, const float& r, const float& lat, const float& lon, float& dr, float& dlat, float& dlon){
		//here I am taking a point and estimating its value linearly using the
		//confirmed values of the four points existing at its bottom left, top left, top right, and bottom right, in a square


		//so... assume the longitude is in magnetic local time, since that's what they want!
		float longitude = fmod((lon * 15. + 180.), 360.);
		const std::vector<float> * data = modelReader->getVariable(variableID);
//		std::cerr << "variable id: " << variableID << std::endl;
		/*
		So for each point I will have a co-latitude and a longitude. The radius I can assume is some constant.
		Then, the value that I need will come from basically any another variable, for example e-flux.

		Point 0: (psi <= lon, theta <= lat)
		Point 1: (psi >= lon, theta <= lat)
		Point 2: (psi >= lon, theta >= lat)
		Point 3: (psi <= lon, theta >= lat)

		       <--
		    3--------2
		    |        |^
		    |        ||
		    0--------1
		       -->

		*/


		//psi_size
		//theta_size

		//to find a position, find the longitude first, then the latitude
		//longitude is steps of psi_size

		int ilon = Utils<float>::binary_search(*psi_data, 0, (*psi_data).size() - 1, longitude);
		int ilat = Utils<float>::binary_search(*th_data, 0, (*th_data).size() - 1, lat);

		//the longitude and colatitude were not found!
		if (ilon < 0 || ilat < 0)
		{
			return this->modelReader->getMissingValue();
		}

//		std::cout << "ilon: " << ilon << std::endl;
//		std::cout << "ilat: " << ilat << std::endl;
		//p0: ilon, ilat
		//p1: ilon+1, ilat
		//p2: ilon+1, ilat+1
		//p3: ilon, ilat+1
//		std::cerr << "lon: " << lon << " ilon: " << ilon << " value: " << (*psi_data)[ilon] << std::endl;
//		std::cerr << "lat: " << lat << " ilat: " << ilat << " value: " << (*th_data)[ilat] << std::endl;
		float lon1 = (*psi_data)[ilon];
		float lon2 = (*psi_data)[ilon+1];

		float lat1 = (*th_data)[ilat];
		float lat2 = (*th_data)[ilat+1];

		float lonP = (longitude - lon1)/(lon2 - lon1); //the percentage along the ray between lon1 and lon2
		float latP = (lat - lat1)/(lat2 - lat1);


		const std::vector<float> * vData = modelReader->getVariableFromMap(variableID);
//		if (vData == NULL)
//			std::cout << "data map was null!!!!" << std::endl;
		int index0 = index_2D_to_1D(ilat, ilon);
		int index1 = index_2D_to_1D(ilat, ilon+1);
		int index2 = index_2D_to_1D(ilat+1, ilon+1);
		int index3 = index_2D_to_1D(ilat+1, ilon);
		float d0 = (*vData)[index0];
		float d1 = (*vData)[index1];
		float d2 = (*vData)[index2];
		float d3 = (*vData)[index3];

//		for (int i = 0; i < 10; i++)
//		{
//			std::cout << "v[" << i << "]: " << (*vData)[i] << std::endl;
//		}

//		std::cout << "index0: " << index0 << " index1: " << index1 << " index2: " << index2 << " index3: " << index3 << std::endl;
//		std::cout << "d0: " << d0 << " d1: " << d1 << " d2: " << d2 << " d3: " << d3 << std::endl;
//		std::cout << "lonP: " << lonP << " latP: " << latP << std::endl;
//		std::cout << "lon: " << longitude << " lon1: " << lon1 << " lon2: " << lon2 << std::endl;
		float p01 = d0*(1.0-lonP) + d1*(lonP);
		float p32 = d3*(1.0-lonP) + d2*(lonP);

		float value = p01*(1.0-latP) + p32*(latP);


		return value;
	}

	int SWMFIonoInterpolator::index_2D_to_1D(const int& ilat, const int& ilon)
	{
		int index = ilon*this->th_data->size() + ilat;
		return index;
	}

	SWMFIonoInterpolator::~SWMFIonoInterpolator()
	{

	}







}
