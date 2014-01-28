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
		phi_string = ccmc::strings::variables::phi_;
		//psi_size = modelReader->getGlobalAttribute("psi_size").getAttributeInt();

		th_data = modelReader->getVariableFromMap(th_string);
		phi_data = modelReader->getVariableFromMap(phi_string);
		theta_size = th_data->size();

		float missingValue = this->modelReader->getMissingValue();
		previous_th = missingValue;
		previous_phi = missingValue;
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

		float dc0, dc1, dc2;
		return interpolate(variableID, r, lat, lon, dc0, dc1, dc2);



	}

	float SWMFIonoInterpolator::interpolate(const long& variableID, const float& r, const float& lat, const float& lon, float& dr, float& dlat, float& dlon){

		//so... assume the longitude is in magnetic local time, since that's what they want!
		float longitude = fmod((lon * 15. + 180.), 360.);
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

		int ilon = Utils<float>::binary_search(*phi_data, 0, (*phi_data).size() - 1, longitude);
		int ilat = Utils<float>::binary_search(*th_data, 0, (*th_data).size() - 1, lat);

		//the longitude and latitude were not found!
		if (ilon < 0 || ilat < 0)
		{
			return this->modelReader->getMissingValue();
		}

		float lon1 = (*phi_data)[ilon];
		float lon2 = (*phi_data)[ilon+1];

		float lat1 = (*th_data)[ilat];
		float lat2 = (*th_data)[ilat+1];

		float lonP = (longitude - lon1)/(lon2 - lon1); //the percentage along the ray between lon1 and lon2
		float latP = (lat - lat1)/(lat2 - lat1);

		//grab a pointer to the variable data so we can index into it.
		const std::vector<float> * vData = modelReader->getVariableFromMap(variableID);
		float d0 = (*vData)[index_2D_to_1D(ilat, ilon)];
		float d1 = (*vData)[index_2D_to_1D(ilat, ilon+1)];
		float d2 = (*vData)[index_2D_to_1D(ilat+1, ilon+1)];
		float d3 = (*vData)[index_2D_to_1D(ilat+1, ilon)];


		float p01 = d0*(1.0-lonP) + d1*(lonP);
		float p32 = d3*(1.0-lonP) + d2*(lonP);

		return p01*(1.0-latP) + p32*(latP);

	}

	int SWMFIonoInterpolator::index_2D_to_1D(const int& ilat, const int& ilon)
	{
		int index = ilon*theta_size + ilat;
		return index;
	}

	SWMFIonoInterpolator::~SWMFIonoInterpolator()
	{

	}







}
