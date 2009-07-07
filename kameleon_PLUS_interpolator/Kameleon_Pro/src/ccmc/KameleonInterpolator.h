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

/**
 * TODO: KameleonInterpolator documentation
 */
class KameleonInterpolator: public Interpolator
{
	public:
		KameleonInterpolator(Model * modelReader);
		float interpolate(const std::string&, const float& c0, const float& c1, const float& c2);
		float interpolate(const std::string&, const float& c0, const float& c1, const float& c2, float& dc0, float& dc1, float& dc2);
		float interpolate(long, const float& c0, const float& c1, const float& c2);
		float interpolate(long, const float& c0, const float& c1, const float& c2, float& dc0, float& dc1, float& dc2);

	//	float interpolateSimple(std::string, const float& c0, const float& c1, const float& c2);
	//	float interpolateSimple(std::string, const float& c0, const float& c1, const float& c2, float& dc0, float& dc1, float& dc2);
		virtual ~KameleonInterpolator();
	private:
		std::string modelName;
		Model * modelReader;
		Interpolator * interpolator;
		boost::unordered_map<std::string, std::vector <std::string> > listOfRequiredVariablesForComponents;
		boost::unordered_map<std::string, std::vector <std::string> > listOfRequiredVariablesForVectors;
		typedef float (KameleonInterpolator::*CalculationMethod)(const std::string&, const float& positionComponent1, const float& positionComponent2, const float& positionComponent3,
		                         float& dComponent1, float& dComponent2, float& dComponent3);

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
		float compute_jparComponent1(const std::string& variable, const float& c0, const float& c1, const float& c2);
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
		float compute_etajComponent1(const std::string& variable, const float& c0, const float& c1, const float& c2);
		float compute_etajComponent2(const std::string& variable, const float& c0, const float& c1, const float& c2);
		float compute_etajComponent3(const std::string& variable, const float& c0, const float& c1, const float& c2);

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
		float compute_ey(const std::string& variable, const float& c0, const float& c1, const float& c2,
				  float& d0, float& d1, float& d2);
		float compute_ez(const std::string& variable, const float& c0, const float& c1, const float& c2,
				  float& d0, float& d1, float& d2);
		float compute_jparComponent1(const std::string& variable, const float& c0, const float& c1, const float& c2,
				  float& d0, float& d1, float& d2);
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
		float compute_s(const std::string& variable, const float& c0, const float& c1, const float& c2,
				  float& d0, float& d1, float& d2);
		float compute_n(const std::string& variable, const float& c0, const float& c1, const float& c2,
				  float& d0, float& d1, float& d2);
		float compute_nv(const std::string& variable, const float& c0, const float& c1, const float& c2,
				  float& d0, float& d1, float& d2);
		float compute_nvComponent1(const std::string& variable, const float& c0, const float& c1, const float& c2,
				  float& d0, float& d1, float& d2);
		float compute_nvy(const std::string& variable, const float& c0, const float& c1, const float& c2,
				  float& d0, float& d1, float& d2);
		float compute_nvz(const std::string& variable, const float& c0, const float& c1, const float& c2,
				  float& d0, float& d1, float& d2);
		float compute_p(const std::string& variable, const float& c0, const float& c1, const float& c2,
				  float& d0, float& d1, float& d2);
		float compute_beta(const std::string& variable, const float& c0, const float& c1, const float& c2,
				  float& d0, float& d1, float& d2);
		float compute_en(const std::string& variable, const float& c0, const float& c1, const float& c2,
				  float& d0, float& d1, float& d2);
		float interpolateSimple(const std::string& variable, const float& c0, const float& c1, const float& c2);
		float interpolateSimple(const std::string& variable, const float& c0, const float& c1, const float& c2,
				  float& d0, float& d1, float& d2);
		float compute_scaleByRadius(const std::string& variable, const float& c0, const float& c1, const float& c2,
				  float& d0, float& d1, float& d2);
		float compute_polb(const std::string& variable, const float& c0, const float& c1, const float& c2,
				  float& d0, float& d1, float& d2);
		float compute_etaj(const std::string& variable, const float& c0, const float& c1, const float& c2,
				  float& d0, float& d1, float& d2);
		float compute_etajComponent1(const std::string& variable, const float& c0, const float& c1, const float& c2,
				  float& d0, float& d1, float& d2);
		float compute_etajComponent2(const std::string& variable, const float& c0, const float& c1, const float& c2,
				  float& d0, float& d1, float& d2);
		float compute_etajComponent3(const std::string& variable, const float& c0, const float& c1, const float& c2,
				  float& d0, float& d1, float& d2);


		//declare strings that will be used frequently during interpolation calls.  Definition occurs in the constructor
		std::string j_,b_,jx_,jy_,jz_,bx_,by_,bz_,ux_,uy_,uz_,ex_,ey_,ez_,n_, rho_, p_;
		std::string batsrus_;
		std::string open_ggcm_;
		std::string ucla_ggcm_;
		std::string mas_;
		std::string enlil_;
};

#endif /* KAMELEONINTERPOLATOR_H_ */
