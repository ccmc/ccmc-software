/*
 * TimeInterpolator.cpp
 *
 *  Created on: Aug 24, 2012
 *      Author: dberrios
 */

#include "TimeInterpolator.h"
#include "GeneralFileReader.h"
#include "FileReader.h"
#include "Interpolator.h"
#include "Utils.h"
#include "Kameleon.h"
#include "CCMCTime.h"
#include <algorithm>
#include <boost/functional/hash.hpp>

using namespace ccmc;
namespace boost
{


//	bool operator==(const boost::posix_time::ptime& a, const boost::posix_time::ptime& b)
//	{
//		  if (a.date() == b.date() && a.time_of_day() == b.time_of_day())
//			  return true;
//		  else
//			  return false;
//	}
}


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
		ccmc::Kameleon * kameleon = new Kameleon();
		int status = kameleon->open(filename);
		if (status == ccmc::FileReader::OK)
		{
			Time time = kameleon->getCurrentTime();
			this->timesteps.push_back(time);
			std::sort(timesteps.begin(), timesteps.end());
			//std::reverse(timesteps.begin(), timesteps.end());
			this->current_kameleon_files[time] = kameleon;
		}
		std::cout << "array: " << std::endl;
		for (int i = 0; i < this->timesteps.size(); i++)
		{
			std::cout << "time[" << i << "]: " << timesteps[i] << std::endl;
		}

	}

	void TimeInterpolator::removeTimestep(const Time& time)
	{
		this->current_kameleon_files.erase(time);
	}

	std::vector<Time> TimeInterpolator::getTimesteps()
	{
		std::vector<Time> timesteps;
		return timesteps;
	}

	/**
	 * Manages the variables loaded into memory.
	 * @variable variables The vector of variables
	 */
	void TimeInterpolator::manageVariables(
			const std::vector<std::string>& variables, Kameleon* a) {
		std::vector<std::string> loadedVariables = a->getLoadedVariables();
		//unload any unnecessary variables
		for (int i = 0; i < loadedVariables.size(); i++) {
			if (std::find(variables.begin(), variables.end(), loadedVariables[i])
					== variables.end()) {
				a->unloadVariable(loadedVariables[i]);
			}
		}
		for (int i = 0; i < variables.size(); i++) {
			std::cout << "loading variable " << variables[i] << std::endl;
			a->loadVariable(variables[i]);
		}
	}

	/**
	 * Manages the variables loaded into memory.
	 * @variable variables The vector of variables
	 */
	void TimeInterpolator::manageVariables(
			const char * variables[], int numVars, Kameleon* a) {
		std::vector<std::string> loadedVariables = a->getLoadedVariables();
		//unload any unnecessary variables
		for (int i = 0; i < loadedVariables.size(); i++) {
			int count = 0;
			for (int j = 0; j < numVars; j++)
			{
				if (loadedVariables[i] != variables[j])
					count ++;

			}
			if (count == numVars)
				a->unloadVariable(loadedVariables[i]);
		}
		for (int i = 0; i < numVars; i++) {
			std::cout << "loading variable " << variables[i] << std::endl;
			a->loadVariable(variables[i]);
		}
	}

	/**
	 * Manages the memory and the opened files based on the time and variable specified.
	 */
	bool TimeInterpolator::manageMemory(const Time& time, const std::vector<std::string>& variables)
	{
		std::cout << "time: " << time.toString() << ": " << time.getEpoch() << std::endl;
		std::cout << "timesteps: " << TimeInterpolator::toString(timesteps[0]) << ": " << timesteps[0].getEpoch();
		std::cout << " " << TimeInterpolator::toString(timesteps[1]) << ": " << timesteps[1].getEpoch() << std::endl;
		int index = Utils<Time>::binary_search(this->timesteps, 0, this->timesteps.size(), time);
std::cout << "manageMemory. index: " << index << std::endl;
		//found the timestep(s)
		if (index >-1)
		{

			//if it is an exact time match and the file is already one of the current_kameleon_files, load just one file
			if (this->timesteps[index] == time && this->current_kameleon_files.find(this->timesteps[index]) != this->current_kameleon_files.end())
			{
				//just load in one file
				Kameleon * found = this->current_kameleon_files[time];
				manageVariables(variables, found);

			}
			//the timesteps are both within the list
			else if (index + 1 < this->timesteps.size() &&
					this->current_kameleon_files.find(this->timesteps[index]) != this->current_kameleon_files.end() &&
					this->current_kameleon_files.find(this->timesteps[index+1]) != this->current_kameleon_files.end())
			{
				//load the variables in the files
				Kameleon * before = this->current_kameleon_files[this->timesteps[index]];
				Kameleon * after = this->current_kameleon_files[this->timesteps[index+1]];

				manageVariables(variables, before);
				manageVariables(variables, after);
			}
//			else
//			{
//				//this->current_kameleon_files.clear();
//				Kameleon * kameleon = new Kameleon();
//				//kameleon->open(this->timesteps_map[this->timesteps[index]]);
//				kameleon->loadVariable(variable);
//			}
			return true;
		} else
		{
			//this->current_kameleon_files.clear();
			//do nothing
			std::cout << "index was not > -1. returning" << std::endl;
			return false;
		}
	}

	/**
		 * Manages the memory and the opened files based on the time and variable specified.
		 */
		bool TimeInterpolator::manageMemory(const Time& time, const char * variables[], int numVars)
		{

			std::cout << "time: " << time.toString() << " timesteps: " << TimeInterpolator::toString(timesteps[0]) << " " << TimeInterpolator::toString(timesteps[1]) << std::endl;
			int index = Utils<Time>::binary_search(this->timesteps, 0, this->timesteps.size(), time);

			//found the timestep(s)
			if (index >-1)
			{

				//if it is an exact time match and the file is already one of the current_kameleon_files, load just one file
				if (this->timesteps[index] == time && this->current_kameleon_files.find(this->timesteps[index]) != this->current_kameleon_files.end())
				{
					//just load in one file
					Kameleon * found = this->current_kameleon_files[time];
					manageVariables(variables, numVars, found);

				}
				//the timesteps are both within the list
				else if (index + 1 < this->timesteps.size() &&
						this->current_kameleon_files.find(this->timesteps[index]) != this->current_kameleon_files.end() &&
						this->current_kameleon_files.find(this->timesteps[index+1]) != this->current_kameleon_files.end())
				{
					//load the variables in the files
					Kameleon * before = this->current_kameleon_files[this->timesteps[index]];
					Kameleon * after = this->current_kameleon_files[this->timesteps[index+1]];

					manageVariables(variables, numVars, before);
					manageVariables(variables, numVars, after);
				}
	//			else
	//			{
	//				//this->current_kameleon_files.clear();
	//				Kameleon * kameleon = new Kameleon();
	//				//kameleon->open(this->timesteps_map[this->timesteps[index]]);
	//				kameleon->loadVariable(variable);
	//			}
				return true;
			} else
			{
				//this->current_kameleon_files.clear();
				//do nothing
				std::cout << "index was not > -1. returning" << std::endl;
				return false;
			}
		}



	float TimeInterpolator::interpolate(const Time& time, const std::string& variable, const float& c0, const float& c1, const float& c2)
	{
		int index = Utils<Time>::binary_search(this->timesteps, 0, this->timesteps.size(), time);
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
				Time atime = this->timesteps[index];
				Time btime = this->timesteps[index+1];
				Interpolator * a = this->current_kameleon_files[atime]->createNewInterpolator();
				Interpolator * b = this->current_kameleon_files[btime]->createNewInterpolator();
				double duration = btime-atime;
				double deltatime = (time - atime);

				double r = deltatime / duration;
				float avalue = a->interpolate(variable, c0, c1, c2);
				float bvalue = b->interpolate(variable, c0, c1, c2);

				float deltaValue = bvalue - avalue;
				float value = avalue + deltaValue * r;
				std::cout << "inside. interpolated value: " << value << std::endl;
				return value;
			}
		} else
			return
				-1.f;
	}

	float TimeInterpolator::interpolate(const Time& time, int variable, const float& c0, const float& c1, const float& c2)
	{
		int index = Utils<Time>::binary_search(this->timesteps, 0, this->timesteps.size(), time);
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
				Time atime = this->timesteps[index];
				Time btime = this->timesteps[index+1];
				Interpolator * a = this->current_kameleon_files[atime]->createNewInterpolator();
				Interpolator * b = this->current_kameleon_files[btime]->createNewInterpolator();
				double duration = btime-atime;
				double deltatime = (time - atime);

				double r = deltatime / duration;
				float avalue = a->interpolate(variable, c0, c1, c2);
				float bvalue = b->interpolate(variable, c0, c1, c2);
				float deltaValue = bvalue - avalue;
				float value = avalue + deltaValue * r;
				std::cout << "inside. interpolated value: " << value << std::endl;
				return value;
			}
		} else
			return
				-1.f;
	}



	void TimeInterpolator::clearAll()
	{

		this->timesteps_map.clear();
		this->timesteps.clear();
		this->current_kameleon_files.clear();


	}

	inline std::size_t hash_value(Time const& time)
	{
	  std::size_t seed = 0;
	  boost::hash_combine(seed, time.getYear());
	  boost::hash_combine(seed, time.getMonth());
	  boost::hash_combine(seed, time.getDay());
	  boost::hash_combine(seed, time.getHour());
	  boost::hash_combine(seed, time.getMinute());
	  boost::hash_combine(seed, time.getSeconds());
	  boost::hash_combine(seed, time.getMilliseconds());
	  //boost::hash_combine(seed, tod.hours());
	  //boost::hash_combine(seed, tod.minutes());
	  //boost::hash_combine(seed, tod.seconds());
	  //boost::hash_combine(seed, tod.fractional_seconds());
	  return seed;
	}

	Time TimeInterpolator::parseTime(const std::string& timeString)
	{
std::cout << "timeString inside parseTime: " << timeString << std::endl;
		std::cout << "before declaring string" << std::endl;
		char timeString_c_str[EPOCH3_STRING_LEN+1];
		std::cout << "before string copy" << std::endl;
		strncpy(timeString_c_str, (char *)timeString.c_str(), EPOCH_STRING_LEN);
		std::cout << "after string copy" << std::endl;
		timeString_c_str[4] = '-';
		timeString_c_str[7] = '-';
		std::cout << "before creating epoch time with parseEPOCH3" << std::endl;
		double epoch = parseEPOCH3(timeString_c_str);
		std::cout << "epoch value in parseTime: " << epoch << std::endl;
		long year, month, day, hour, minute, second, msec;
		year = month = day = hour = minute = second = msec = 1;
		EPOCHbreakdown(epoch, &year, &month, &day, &hour, &minute, &second, &msec);
		Time time;
		time.setYear((short)year);
		time.setMonth((short)month);
		time.setDay((short)day);
		time.setHour((short)hour);
		time.setMinute((short)minute);
		time.setSeconds((short)second);
		time.setMilliseconds((short)msec);
		return time;

	}

	std::string TimeInterpolator::toString(const Time& time)
	{
		char epochString[EPOCH3_STRING_LEN+1];
		std::cout << "time: " << time.toString() << " " << time.getEpoch() << std::endl;
		encodeEPOCH3(time.getEpoch(), epochString);
		return epochString;
	}

	void TimeInterpolator::closeAndCleanupMemory()
	{

		for (int i = 0; i < this->timesteps.size(); i++)
		{
			this->current_kameleon_files[timesteps[i]]->close();
		}
		this->timesteps.clear();
		this->current_kameleon_files.clear();
	}


};

