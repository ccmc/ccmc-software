/*
 * ENLILInterpolator.h
 *
 *  Created on: Jul 9, 2009
 *      Author: dberrios
 */

#ifndef ENLILINTERPOLATOR_H_
#define ENLILINTERPOLATOR_H_

#include "Interpolator.h"
#include "Model.h"

namespace ccmc
{
	/**
	 * @class ENLILInterpolator ENLILInterpolator.h ccmc/ENLILInterpolator.h
	 * @brief TODO: Brief description of ENLILInterpolator class
	 *
	 * TODO: Full description of ENLILInteprolator class
	 */
	class ENLILInterpolator: public Interpolator
	{
		public:
			ENLILInterpolator(Model * model);
			float interpolate(const std::string& variable, const float& r, const float& lon, const float& lat);
			float interpolate(const std::string& variable, const float& r, const float& lon, const float& lat, float& dr, float& dlon, float& dlat);
			float interpolate(long variableID, const float& r, const float& lon, const float& lat);
			float interpolate(long variableID, const float& r, const float& lon, const float& lat, float& dr, float& dlon, float& dlat);


			virtual ~ENLILInterpolator();

		private:
			const std::vector<float> * r_data;
			const std::vector<float> * lat_data; //phi
			const std::vector<float> * lon_data; //theta
			int nr;
			int nlat;
			int nlon;
			float interpolate_in_block_enlil(float r, float lon, float lat, int ir, int ilon, int ilat,
								long variableID, float& dr, float& dlon, float& dlat);
	};
}

#endif /* ENLILINTERPOLATOR_H_ */
