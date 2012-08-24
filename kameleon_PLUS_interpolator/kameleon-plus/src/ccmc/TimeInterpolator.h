/*
 * TimeInterpolator.h
 *
 *  Created on: Aug 24, 2012
 *      Author: dberrios
 */

#ifndef TIMEINTERPOLATOR_H_
#define TIMEINTERPOLATOR_H_

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/unordered_map.hpp>
#include <vector>
#include "Kameleon.h"

namespace ccmc
{
	/*
	 *
	 */
	class TimeInterpolator
	{
		public:
			TimeInterpolator();
			virtual ~TimeInterpolator();
			void addTimestep(const std::string& filename);
			void removeTimestep(const boost::posix_time::ptime& time);
			std::vector<boost::posix_time::ptime> getTimesteps();
			bool loadVariable(const boost::posix_time::ptime& time, const std::string& variable);
			bool unloadVariable(const boost::posix_time::ptime& time, const std::string& variable);
			float interpolate(const boost::posix_time::ptime& time, const std::string& variable, const float& c0, const float& c1, const float& c2);
			float interpolate(const boost::posix_time::ptime& time, int variable, const float& c0, const float& c1, const float& c2);

		private:
			std::vector<boost::posix_time::ptime> timesteps;
			boost::unordered_map<boost::posix_time::ptime, Kameleon> timesteps_map;
	};
}

#endif /* TIMEINTERPOLATOR_H_ */
