/*
 * KameleonInterpolator.h
 *
 *  Created on: Jul 1, 2009
 *      Author: David Berrios
 */

#ifndef KAMELEONINTERPOLATOR_H_
#define KAMELEONINTERPOLATOR_H_
#include <boost/unordered_map.hpp>

#include "Interpolator.h"

namespace ccmc
{
	/**
	 * @class KameleonInterpolator KameleonInterpolator.h ccmc/KameleonInterpolator.h
	 * @brief TODO: Brief description of KameleonInterpolator class
	 *
	 * TODO: Full description of KameleonInterpolator class
	 */
	class KameleonInterpolator: public Interpolator
	{
		public:
			KameleonInterpolator(Model * modelReader);

			/**
			 * OpenGGCM and BATSRUS: c0,c1,c2 corresponds to x,y,z, respectively.  ENLIL and MAS: c0,c1,c2 corresponds
			 * to r,phi(latitude), theta(longitude), respectively
			 * @param variable
			 * @param c0
			 * @param c1
			 * @param c2
			 */
			float interpolate(const std::string& variable, const float& c0, const float& c1, const float& c2);

			/**
			 * OpenGGCM and BATSRUS: c0,c1,c2 corresponds to x,y,z, respectively.  ENLIL and MAS: c0,c1,c2 corresponds
			 * to r,phi(latitude), theta(longitude), respectively
			 * @param variable
			 * @param c0
			 * @param c1
			 * @param c2
			 * @param dc0
			 * @param dc1
			 * @param dc2
			 */
			float interpolate(const std::string& variable, const float& c0, const float& c1, const float& c2, float& dc0,
					float& dc1, float& dc2);

			/**
			 * OpenGGCM and BATSRUS: c0,c1,c2 corresponds to x,y,z, respectively.  ENLIL and MAS: c0,c1,c2 corresponds
			 * to r,phi(latitude), theta(longitude), respectively
			 * @param variable_id
			 * @param c0
			 * @param c1
			 * @param c2
			 */
			float interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2);
			float interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2, float& dc0, float& dc1,
					float& dc2);


			virtual ~KameleonInterpolator();
		private:
			typedef float (KameleonInterpolator::*CalculationMethod)(const std::string&,
											const float& positionComponent1, const float& positionComponent2, const float& positionComponent3,
											float& dComponent1, float& dComponent2, float& dComponent3);
			std::string modelName;
			Model * modelReader;
			Interpolator * interpolator;
			boost::unordered_map<std::string, std::vector<std::string> > listOfRequiredVariablesForComponents;
			boost::unordered_map<std::string, std::vector<std::string> > listOfRequiredVariablesForVectors;


			boost::unordered_map<std::string, CalculationMethod> calculationMethod;
			boost::unordered_map<std::string, float> conversionFactorsToSI;
			boost::unordered_map<std::string, float> conversionFactorsToVis;
			boost::unordered_map<std::string, std::string> variableAliases;

			//std::vector<std::string> getListOfRequiredVariablesForComponents(std::string variable);
			//std::vector<std::string> getListOfRequiredVariablesForVectors(std::string variable);
			void initializeCalculationMethods();
			float compute_magnitude(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_temp(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_pram(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_edotj(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_eComponent1(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_ey(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_ez(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float
					compute_jparComponent1(const std::string& variable, const float& c0, const float& c1,
							const float& c2);
			float compute_jpary(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_jparz(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_jpar(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_jxbComponent1(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_jxby(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_jxbz(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_jxb(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_exbComponent1(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_exby(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_exbz(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_exb(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_s(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_n(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_nv(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_nvComponent1(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_nvy(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_nvz(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_p(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_beta(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_en(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_scaleByRadius(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_polb(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float compute_etaj(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float
					compute_etajComponent1(const std::string& variable, const float& c0, const float& c1,
							const float& c2);
			float
					compute_etajComponent2(const std::string& variable, const float& c0, const float& c1,
							const float& c2);
			float
					compute_etajComponent3(const std::string& variable, const float& c0, const float& c1,
							const float& c2);
			float
					compute_gradient(const std::string& variable, const float& c0, const float& c1,
							const float& c2);

			float compute_magnitude(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_temp(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_pram(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_edotj(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_eComponent1(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_ey(const std::string& variable, const float& c0, const float& c1, const float& c2, float& d0,
					float& d1, float& d2);
			float compute_ez(const std::string& variable, const float& c0, const float& c1, const float& c2, float& d0,
					float& d1, float& d2);
			float compute_jparComponent1(const std::string& variable, const float& c0, const float& c1,
					const float& c2, float& d0, float& d1, float& d2);
			float compute_jpary(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_jparz(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_jpar(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_jxbComponent1(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_jxby(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_jxbz(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_jxb(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_exbComponent1(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_exby(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_exbz(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_exb(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_s(const std::string& variable, const float& c0, const float& c1, const float& c2, float& d0,
					float& d1, float& d2);
			float compute_n(const std::string& variable, const float& c0, const float& c1, const float& c2, float& d0,
					float& d1, float& d2);
			float compute_nv(const std::string& variable, const float& c0, const float& c1, const float& c2, float& d0,
					float& d1, float& d2);
			float compute_nvComponent1(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_nvy(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_nvz(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_p(const std::string& variable, const float& c0, const float& c1, const float& c2, float& d0,
					float& d1, float& d2);
			float compute_beta(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_en(const std::string& variable, const float& c0, const float& c1, const float& c2, float& d0,
					float& d1, float& d2);
			float interpolateSimple(const std::string& variable, const float& c0, const float& c1, const float& c2);
			float interpolateSimple(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float interpolateSimple(const long& variable, const float& c0, const float& c1, const float& c2);
			float interpolateSimple(const long& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_scaleByRadius(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_polb(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_etaj(const std::string& variable, const float& c0, const float& c1, const float& c2,
					float& d0, float& d1, float& d2);
			float compute_etajComponent1(const std::string& variable, const float& c0, const float& c1,
					const float& c2, float& d0, float& d1, float& d2);
			float compute_etajComponent2(const std::string& variable, const float& c0, const float& c1,
					const float& c2, float& d0, float& d1, float& d2);
			float compute_etajComponent3(const std::string& variable, const float& c0, const float& c1,
					const float& c2, float& d0, float& d1, float& d2);
			float compute_gradient(const std::string& variable, const float& c0, const float& c1,
					const float& c2, float& d0, float& d1, float& d2);
			//declare strings that will be used frequently during interpolation calls.  Definition occurs in the constructor
			std::vector<float>* interpolateSimple(std::vector<std::string>& variables, const float& c0, const float& c1, const float& c2);
			std::vector<float>* interpolateSimple(std::vector<long>& variables, const float& c0, const float& c1, const float& c2);
			void initializeConversionFactorsToVis();
			float getConversionFactorToVis(const std::string& variable);
	};
}

#endif /* KAMELEONINTERPOLATOR_H_ */
