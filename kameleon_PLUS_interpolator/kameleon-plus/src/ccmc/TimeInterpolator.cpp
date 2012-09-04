/*
 * TimeInterpolator.cpp
 *
 *  Created on: Aug 24, 2012
 *      Author: dberrios
 */

#include "TimeInterpolator.h"
#include "GeneralFileReader.h"
#include "Interpolator.h"
#include "Utils.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/functional/hash.hpp>

namespace ccmc
{
	TimeInterpolator::TimeInterpolator()
	{
		// TODO Auto-generated constructor stub
		isSorted = false;

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

		int index = Utils<boost::posix_time::ptime>::binary_search(this->timesteps, 0, this->timesteps.size(), time);

		//found the timestep(s)
		if (index >-1)
		{

			//if it is an exact time match and the file is already one of the current_kameleon_files, load just one file
			if (this->timesteps[index] == time && this->current_kameleon_files.find(this->timesteps[index]) != this->current_kameleon_files.end())
			{
				//just load in one file
				Kameleon * found = this->current_kameleon_files[time];
				found->loadVariable(variable);
			} else if (this->current_kameleon_files.find(this->timesteps[index]) != this->current_kameleon_files.end() &&
					   this->current_kameleon_files.find(this->timesteps[index+1]) != this->current_kameleon_files.end())
			{
				//load the variables in the files
				Kameleon * before = this->current_kameleon_files[this->timesteps[index]];
				Kameleon * after = this->current_kameleon_files[this->timesteps[index]];
				before->loadVariable(variable);
				after->loadVariable(variable);
			} else
			{
				this->current_kameleon_files.clear();
				Kameleon * kameleon = new Kameleon();
				kameleon->open(this->timesteps_map[this->timesteps[index]]);
				kameleon->loadVariable(variable);
			}
			return true;
		} else
		{
			this->current_kameleon_files.clear();
			//do nothing
			return false;
		}
	}

	bool TimeInterpolator::unloadVariable(const boost::posix_time::ptime& time, const std::string& variable)
	{
		return true;
	}

	float TimeInterpolator::interpolate(const boost::posix_time::ptime& time, const std::string& variable, const float& c0, const float& c1, const float& c2)
	{
		int index = Utils<boost::posix_time::ptime>::binary_search(this->timesteps, 0, this->timesteps.size(), time);
		if (index > -1)
		{
			if (time == this->timesteps[index])
			{
				Interpolator * a = this->current_kameleon_files[time]->createNewInterpolator();
				float value = a->interpolate(variable, c0, c1, c2);
				delete a;
				return value;
			} else
			{
				boost::posix_time::ptime atime = this->timesteps[index];
				boost::posix_time::ptime btime = this->timesteps[index+1];
				Interpolator * a = this->current_kameleon_files[atime]->createNewInterpolator();
				Interpolator * b = this->current_kameleon_files[btime]->createNewInterpolator();
				boost::posix_time::time_duration duration = btime-atime;
				long durationdelta = duration.total_nanoseconds();
				long deltatime = (time - atime).total_nanoseconds();

				double r = (double) deltatime / (double) durationdelta;
				float avalue = a->interpolate(variable, c0, c1, c2);
				float bvalue = b->interpolate(variable, c0, c1, c2);
				float deltaValue = bvalue - avalue;
				return avalue + deltaValue * r;
			}
		} else
			return
				-1;
	}

	float TimeInterpolator::interpolate(const boost::posix_time::ptime& time, int variable, const float& c0, const float& c1, const float& c2)
	{
		int index = Utils<boost::posix_time::ptime>::binary_search(this->timesteps, 0, this->timesteps.size(), time);
		if (index > -1)
		{
			if (time == this->timesteps[index])
			{
				Interpolator * a = this->current_kameleon_files[time]->createNewInterpolator();
				float value = a->interpolate(variable, c0, c1, c2);
				delete a;
				return value;
			} else
			{
				boost::posix_time::ptime atime = this->timesteps[index];
				boost::posix_time::ptime btime = this->timesteps[index+1];
				Interpolator * a = this->current_kameleon_files[atime]->createNewInterpolator();
				Interpolator * b = this->current_kameleon_files[btime]->createNewInterpolator();
				boost::posix_time::time_duration duration = btime-atime;
				long durationdelta = duration.total_nanoseconds();
				long deltatime = (time - atime).total_nanoseconds();

				double r = (double) deltatime / (double) durationdelta;
				float avalue = a->interpolate(variable, c0, c1, c2);
				float bvalue = b->interpolate(variable, c0, c1, c2);
				float deltaValue = bvalue - avalue;
				return avalue + deltaValue * r;
			}
		} else
			return
				-1;
	}



	void TimeInterpolator::clearAll()
	{

		this->timesteps_map.clear();
		this->timesteps.clear();
		this->current_kameleon_files.clear();


	}



};

namespace boost
{
	inline std::size_t hash_value(const boost::posix_time::ptime& ptime)
	{
	  std::size_t seed = 0;
	  boost::gregorian::date date = ptime.date();
	  boost::posix_time::time_duration tod = ptime.time_of_day();
	  boost::hash_combine(seed, date.modjulian_day());
	  boost::hash_combine(seed, tod.hours());
	  boost::hash_combine(seed, tod.minutes());
	  boost::hash_combine(seed, tod.seconds());
	  boost::hash_combine(seed, tod.fractional_seconds());
	  return seed;
	}

//	bool operator==(const boost::posix_time::ptime& a, const boost::posix_time::ptime& b)
//	{
//		  if (a.date() == b.date() && a.time_of_day() == b.time_of_day())
//			  return true;
//		  else
//			  return false;
//	}
}
