/*
 * OpenGGCMInterpolator.h
 *
 *  Created on: Jun 30, 2009
 *      Author: dberrios
 */

#ifndef OPENGGCMINTERPOLATOR_H_
#define OPENGGCMINTERPOLATOR_H_

#include "Interpolator.h"
#include "Model.h"

class OpenGGCMInterpolator: public Interpolator
{
	public:
		OpenGGCMInterpolator(Model * modelReader);
		float interpolate(const std::string& variable, const float& c0, const float& c1, const float& c2);
		float interpolate(const std::string& variable, const float& c0, const float& c1, const float& c2, float& dc0,
				float& dc1, float& dc2);
		float interpolate(long variable_id, const float & c0, const float& c1, const float& c2);
		float interpolate(long variable_id, const float& c0, const float& c1, const float& c2, float& dc0, float& dc1,
				float& dc2);
		template<typename T>
				friend int binary_search(const std::vector<T>& vec, unsigned int start, unsigned int end, const T& key);
		virtual ~OpenGGCMInterpolator();

	private:
		Model * modelReader;
		boost::unordered_map<std::string, float> conversionFactors;
		boost::unordered_map<long, float> conversionFactorsByID;
		float getConversionFactor(const std::string&);
		float getConversionFactor(const long& variable_id);
		std::string x_string;
		std::string y_string;
		std::string z_string;
		int nx;
		int ny;
		int nz;
		const std::vector<float> * x_array;
		const std::vector<float> * y_array;
		const std::vector<float> * z_array;
		std::string previousVariable;
		long previousVariableID;
		float previousConversionFactor;
		float previous_x;
		float previous_y;
		float previous_z;
		int previous_ix;
		int previous_iy;
		int previous_iz;
};

#endif /* OPENGGCMINTERPOLATOR_H_ */
