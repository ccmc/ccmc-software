/*
 * MASInterpolator.h
 *
 *  Created on: Jul 9, 2009
 *      Author: dberrios
 */

#ifndef MASINTERPOLATOR_H_
#define MASINTERPOLATOR_H_

#include "Interpolator.h"
#include "Model.h"

namespace ccmc
{
	/**
	 * @class MASInterpolator MASInterpolator.h ccmc/MASInterpolator.h
	 * @brief TODO: Brief description of MASInterpolator class
	 *
	 * TODO: Full description of MASInteprolator class
	 */
	class MASInterpolator: public Interpolator
	{
		public:
			MASInterpolator(Model * model);
			float interpolate(const std::string& variable, const float& r, const float& lat, const float& lon);
			float interpolate(const std::string& variable, const float& r, const float& lat, const float& lon, float& dr, float& dlat, float& dlon);
			float interpolate(const long& variable_id, const float& r, const float& lat, const float& lon);
			float interpolate(const long& variable_id, const float& r, const float& lat, const float& lon, float& dr, float& dlat, float& dlon);
			virtual ~MASInterpolator();

		private:
			//TODO: fix these coordinate names. phi should be longitude, theta should be latitude
			const std::vector<float> * r_data;
			const std::vector<float> * r1_data;//r1
			const std::vector<float> * lat_data; //theta
			const std::vector<float> * lon_data; //phi
			const std::vector<float> * lat1_data; //theta1
			std::string r_string;
			std::string r1_string;
			std::string lat_string;
			std::string lon_string;
			std::string lat1_string;
			int nr;
			int nlat;
			int nlon;

			int nr_plus1;
			int nlat_plus1;
			int nlon_plus1;
			float previous_r;
			float previous_lon;
			float previous_lat;
			int previous_ir;
			int previous_ilon;
			int previous_ilat;

			float interpolate_in_block_mas(
				      float r,
				      float lat,
				      float lon,
				      int ir,
				      int ilat,
				      int ilon,
				      const long& variable_id,
				      float& dr,
				      float& dlat,
				      float& dlon);




	};
}
#endif /* MASINTERPOLATOR_H_ */
