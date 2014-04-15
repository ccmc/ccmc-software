/*
 * TimeInterpolator.h
 *
 *  Created on: Aug 24, 2012
 *      Author: dberrios
 */

#ifndef TIMEINTERPOLATOR_H_
#define TIMEINTERPOLATOR_H_

#include <boost/unordered_map.hpp>
#include <vector>
#include "Kameleon.h"
#include "Interpolator.h"
#include "CCMCTime.h"

namespace ccmc
{


	class Kameleon;

	/*
	 *
	 */
	class TimeInterpolator
	{
		public:
			TimeInterpolator();
			virtual ~TimeInterpolator();
			void addTimestep(const std::string& filename);
			void removeTimestep(const Time& time);
			std::vector<Time> getTimesteps();
			Interpolator * getInterpolator(const Time& time);
			Kameleon * getKameleon(const Time& time);
			bool manageMemory(const Time& time, const std::vector<std::string>& variable);
			bool manageMemory(const Time& time, const char * variable[], int numVars);
			void closeAndCleanupMemory();
			// bool unloadVariable(const Time& time, const std::string& variable); //Not implemented yet
			float interpolate(const Time& time, const std::string& variable, const float& c0, const float& c1, const float& c2);
			float interpolate(const Time& time, int variable, const float& c0, const float& c1, const float& c2);
			void clearAll();
			static Time parseTime(const std::string& timeString);
			static std::string toString(const Time& time);

		private:
			std::vector<Time> timesteps;
			boost::unordered_map<Time, const std::string> timesteps_map;
			boost::unordered_map<Time, Kameleon *> current_kameleon_files;
			boost::unordered_map<Time, Interpolator *> current_kameleon_interpolators;
			bool isSorted;

			void manageVariables(const std::vector<std::string>& variables, Kameleon* a);
			void manageVariables(const char * variables[], int numVars, Kameleon* a);
	};
}

#endif /* TIMEINTERPOLATOR_H_ */
