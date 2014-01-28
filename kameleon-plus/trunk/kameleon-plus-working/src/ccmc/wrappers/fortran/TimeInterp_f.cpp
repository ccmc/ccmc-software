#include "TimeInterp_f.h"
#include <ccmc/wrappers/c/TimeInterpolator_c.h>
#include <ccmc/Kameleon.h>
#include <ccmc/CCMCTime.h>
#include <string.h>

using namespace ccmc;
typedef boost::unordered_map<int, ccmc::TimeInterpolator*> map_i_T;

void f_timeinterp_create_(int * tid)
{
	int maxObjects = 4096;
	int id = 0;
	while (timeInterpolatorObjects.find(id) != timeInterpolatorObjects.end())
	{
		id++;
		if (id > maxObjects)
			*tid = -1;
	}
	//first check if the id exists.  if so, delete it and create a new one.
	timeInterpolatorObjects[id] = new TimeInterpolator();
	*tid = id;
}

void f_timeinterp_addtimestep_(int * tid, const char * filename)
{
	std::string filename_str = std::string(filename);
	int length = filename_str.size();
	char filename_corrected[length+1];

	strcpy(filename_corrected, filename_str.c_str());
	//std::cout << "string length: " << length << std::endl;
	for (int i = 0; i< length; i++)
	{
		if (filename_corrected[i] == ' ')
		{
	//		std::cout << "add the character at index: " << i << std::endl;
			filename_corrected[i] = '\0';
			break;
		}

	}
	//TODO: error checking
	int status;
	map_i_T::iterator iter = ccmc::timeInterpolatorObjects.find(*tid);

	if (iter != ccmc::timeInterpolatorObjects.end())
	{
		//first, open the file

		ccmc::TimeInterpolator * timeInterp = (*iter).second;
//		std::cout << "before calling kameleon->open" << std::endl;
		timeInterp->addTimestep(filename_corrected);
		//status = true;
//		std::cout << "filename: " << filename << " status: " << status << " after" << std::endl;
		//kameleonObjects.erase(iter);
	} else
	{
		//return false;
		//should never happen
		std::cerr << "this should never happen!" << std::endl;

	}
//	std::cout << "new size of kameleonObjects: " << ccmc::kameleonObjects.size() << std::endl;
	//return status;
}

void f_timeinterp_managemem_(int *tid, double *epoch, const char * variable)
{
	std::string variable_str = std::string(variable);
	int length = variable_str.size();
	char variable_corrected[length+1];

	strcpy(variable_corrected, variable_str.c_str());
	//std::cout << "string length: " << length << std::endl;
	for (int i = 0; i< length; i++)
	{
		if (variable_corrected[i] == ' ')
		{
	//		std::cout << "add the character at index: " << i << std::endl;
			variable_corrected[i] = '\0';
			break;
		}

	}
        //TODO: error checking
        int status;
        map_i_T::iterator iter = ccmc::timeInterpolatorObjects.find(*tid);

        if (iter != ccmc::timeInterpolatorObjects.end())
        {
                //first, open the file

                ccmc::TimeInterpolator * timeInterp = (*iter).second;
                Time time(*epoch);
                std::vector<std::string> variables;
                variables.push_back(variable_corrected);
                timeInterp->manageMemory(time, variables);
        } else
        {
                //should never happen
                std::cerr << "this should never happen!" << std::endl;

        }
}

void f_timeinterp_interpolate_(int * tid, double * epoch, const char * variable, float * c0, float * c1, float * c2, float * value)
{
	std::string variable_str = std::string(variable);
	int length = variable_str.size();
	char variable_corrected[length+1];

	strcpy(variable_corrected, variable_str.c_str());
	//std::cout << "string length: " << length << std::endl;
	for (int i = 0; i< length; i++)
	{
		if (variable_corrected[i] == ' ')
		{
	//		std::cout << "add the character at index: " << i << std::endl;
			variable_corrected[i] = '\0';
			break;
		}

	}
        //TODO: error checking
        int status;
        map_i_T::iterator iter = ccmc::timeInterpolatorObjects.find(*tid);

        if (iter != ccmc::timeInterpolatorObjects.end())
        {
                //first, open the file

                ccmc::TimeInterpolator * timeInterp = (*iter).second;
                Time time(*epoch);
                *value = timeInterp->interpolate(time, variable_corrected, *c0, *c1, *c2);
        } else
        {
        	*value = -1.f;
                //should never happen
                std::cerr << "this should never happen!" << std::endl;

        }
}

void f_timeinterp_timetoepoch_(int * year, int * month, int * day, int * hour, int * minute, int * second, int * millisecond, double * epoch)
{
	Time time;
	time.setYear(*year);
	time.setMonth(*month);
	time.setDay(*day);
	time.setHour(*hour);
	time.setMinute(*minute);
	time.setSeconds(*second);
	time.setMilliseconds(*millisecond);
	*epoch = time.getEpoch();
	

}

void f_timeinterp_timestrtoepoch_(const char * time_str, double * epoch)
{
	char time_str_copy[25];
	strncpy(time_str_copy, time_str, 25);
	time_str_copy[24] = '\0';
	Time time = TimeInterpolator::parseTime(time_str_copy);
	std::cout << "epoch time after creating time object: " << time.getEpoch();
	*epoch = time.getEpoch();
}

void f_timeinterp_closeandcleanupmemory_(int * tid)
{
        //TODO: error checking
        map_i_T::iterator iter = ccmc::timeInterpolatorObjects.find(*tid);

        if (iter != ccmc::timeInterpolatorObjects.end())
        {
                //first, open the file

                ccmc::TimeInterpolator * timeInterp = (*iter).second;
                timeInterp->closeAndCleanupMemory();
                delete timeInterp;
        } else
        {
                //should never happen
                std::cerr << "this should never happen!" << std::endl;

        }	
	ccmc::timeInterpolatorObjects.erase(*tid);
}
