/*
 * IMF.h
 *
 *  Created on: Aug 11, 2008
 *      Author: dberrios
 */
#ifdef __cplusplus
#ifndef SHUE_H_
#define SHUE_H_
#include <string>
#include <vector>
#include <time.h>
#include <ctime>
using namespace std;

struct IMFdata
{
	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;
	int msec;
	float bx;
	float by;
	float bz;
	float vx;
	float vy;
	float vz;
	float n;
	float t;
	time_t epoch;
	float shue_r0;
	float d_p;


};

class IMF
{

	public:
		IMF();
		void open(std::string filename);
		void interpolate(time_t targetTime, float xPosition, float& radius, bool& isInside);
		int size();
		IMFdata& operator[] (int i);
		virtual ~IMF();
		void calculateMagnetopause(const time_t& targetTime, vector<float>& xPositions, vector<float>& radii,
				float nightSideExtent, int slices);




	private:
		IMFdata parse(const std::string &line);
		vector<IMFdata> imfdata;
		IMFdata interpolateIMFData(int & index, time_t & transitTime);
		int parseInt(const std::string &str);
		float parseFloat(const std::string &str);
		int exactDateComparison ( const time_t& targetTime, int mid);
		int betweenDateComparison (const time_t& targetTime, int test);
		int bSearchTime(const time_t& targetTime, int low, int high);
		float bSearchMagnetopauseStart(const time_t& targetTime, const float& low, const float& high);
		int magnetopauseComparison( const time_t& targetTime, const float& mid);
		float interpolateFloat(float before, float after, float percentage);

};

class Shue
{
	public:
		Shue();
		void open(std::string filename);
};
#endif //__cplusplus

#endif /* SHUE_H_ */
