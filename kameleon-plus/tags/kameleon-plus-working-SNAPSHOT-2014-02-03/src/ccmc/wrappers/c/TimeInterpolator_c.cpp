#include "TimeInterpolator_c.h"
#include <ccmc/Kameleon.h>
#include <ccmc/CCMCTime.h>

using namespace ccmc;
typedef boost::unordered_map<int, ccmc::TimeInterpolator*> map_i_T;

int TimeInterp_create()
{
	int maxObjects = 4096;
	int id = 0;
	while (timeInterpolatorObjects.find(id) != timeInterpolatorObjects.end())
	{
		id++;
		if (id > maxObjects)
			return -1;
	}
	//first check if the id exists.  if so, delete it and create a new one.
	timeInterpolatorObjects[id] = new TimeInterpolator();
	return id;
}

int TimeInterp_addTimestep(int tid, const char * filename)
{
	//TODO: error checking
	int status;
	map_i_T::iterator iter = ccmc::timeInterpolatorObjects.find(tid);

	if (iter != ccmc::timeInterpolatorObjects.end())
	{
		//first, open the file

		ccmc::TimeInterpolator * timeInterp = (*iter).second;
//		std::cout << "before calling kameleon->open" << std::endl;
		timeInterp->addTimestep(filename);
		status = true;
//		std::cout << "filename: " << filename << " status: " << status << " after" << std::endl;
		//kameleonObjects.erase(iter);
	} else
	{
		return false;
		//should never happen
		std::cerr << "this should never happen!" << std::endl;

	}
//	std::cout << "new size of kameleonObjects: " << ccmc::kameleonObjects.size() << std::endl;
	return status;
}

void TimeInterp_manageMem(int tid, double epoch, const char * variables[], int numVars)
{
        //TODO: error checking
        int status;
        map_i_T::iterator iter = ccmc::timeInterpolatorObjects.find(tid);
        std::cout << "found id: " << tid << std::endl;
        if (iter != ccmc::timeInterpolatorObjects.end())
        {
                //first, open the file

                ccmc::TimeInterpolator * timeInterp = (*iter).second;
                Time time(epoch);
                timeInterp->manageMemory(time, variables, numVars);
        } else
        {
                //should never happen
                std::cerr << "this should never happen!" << std::endl;

        }
}

float TimeInterp_interpolate(int tid, double epoch, const char * variable, float c0, float c1, float c2)
{
        //TODO: error checking
        int status;
        map_i_T::iterator iter = ccmc::timeInterpolatorObjects.find(tid);

        float value = -1.f;
        if (iter != ccmc::timeInterpolatorObjects.end())
        {
                //first, open the file

                ccmc::TimeInterpolator * timeInterp = (*iter).second;
                Time time(epoch);
                value = timeInterp->interpolate(time, variable, c0, c1, c2);
        } else
        {
                //should never happen
                std::cerr << "this should never happen!" << std::endl;

        }
        std::cout << "returning value: " << value << std::endl;
        return value;
}

double TimeInterp_TimeToEpoch(int year, int month, int day, int hour, int minute, int second, int millisecond)
{
	Time time;
	time.setYear((short)year);
	time.setMonth((short)month);
	time.setDay((short)day);
	time.setHour(hour);
	time.setMinute(minute);
	time.setSeconds(second);
	time.setMilliseconds(millisecond);
	return time.getEpoch();
	

}

double TimeInterp_TimeStrToEpoch(const char * time_str)
{
	std::cout << "original time_str: " << time_str << std::endl;
	Time time = ccmc::TimeInterpolator::parseTime(time_str);
	std::cout << "new time_str: " << time.toString() << std::endl;
	return time.getEpoch();
}

void TimeInterp_closeAndCleanupMemory(int tid)
{
        //TODO: error checking
        map_i_T::iterator iter = ccmc::timeInterpolatorObjects.find(tid);

        if (iter != ccmc::timeInterpolatorObjects.end())
        {
                //first, open the file

                ccmc::TimeInterpolator * timeInterp = (*iter).second;
                timeInterp->closeAndCleanupMemory();
        } else
        {
                //should never happen
                std::cerr << "this should never happen!" << std::endl;

        }	
	ccmc::timeInterpolatorObjects.erase(tid);
}
