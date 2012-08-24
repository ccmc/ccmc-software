/*
 * TimeInterpolator.cpp
 *
 *  Created on: Aug 24, 2012
 *      Author: dberrios
 */

#include "TimeInterpolator.h"
#include "GeneralFileReader.h"

namespace ccmc
{
	TimeInterpolator::TimeInterpolator()
	{
		// TODO Auto-generated constructor stub

	}

	TimeInterpolator::~TimeInterpolator()
	{
		// TODO Auto-generated destructor stub
	}

	void TimeInterpolator::addTimestep(const std::string& filename)
	{

	}

	void TimeInterpolator::removeTimestep(const boost::posix_time::ptime& time)
	{

	}

	std::vector<boost::posix_time::ptime> TimeInterpolator::getTimesteps()
	{
		std::vector<boost::posix_time::ptime> timesteps;
		return timesteps;
	}

	bool TimeInterpolator::loadVariable(const boost::posix_time::ptime& time, const std::string& variable)
	{
		return true;
	}

	bool TimeInterpolator::unloadVariable(const boost::posix_time::ptime& time, const std::string& variable)
	{
		return true;
	}

	float TimeInterpolator::interpolate(const boost::posix_time::ptime& time, const std::string& variable, const float& c0, const float& c1, const float& c2)
	{
		return 0.0f;
	}

	float TimeInterpolator::interpolate(const boost::posix_time::ptime& time, int variable, const float& c0, const float& c1, const float& c2)
	{
		return 0.0f;
	}

};
