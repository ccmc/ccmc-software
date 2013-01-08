/*
 * IMFTest.cpp
 *
 *  Created on: Aug 11, 2008
 *      Author: dberrios
 */

#include <ccmc/Shue.h>
#include <iostream>
#include <sstream>
#include <ctime>

using namespace std;

int parseInt(const std::string& token)
{
	cout << "token: " << token << endl;
	std::istringstream iss(token);
	int value;
	iss >> value;
	return value;


}

time_t parseTime(std::string cdfEpochString)
{
	//string should be in the form yyyy-mm-ddThh:mm:ss:mmmZ
	std::string delimiters = "-T:Z.";
	vector<std::string> tokens;
	IMFdata imfdataline;

	//find the position of the character that does not match any of the delimiters
	//cout << "here" << endl;
	if (cdfEpochString.length() > 0)
	{
		size_t lastPosition = cdfEpochString.find_first_not_of(delimiters, 0);
		// Find first delimiter after the lastPosition
		size_t currentPosition = cdfEpochString.find_first_of(delimiters, lastPosition);

		while (currentPosition != std::string::npos || lastPosition != std::string::npos)
		{
			tokens.push_back(cdfEpochString.substr(lastPosition, currentPosition - lastPosition));
			vector<std::string> tokens;
			IMFdata imfdataline;

			//find the position of the character that does not match any of the delimiters
			//cout << "here" << endl;
			if (cdfEpochString.length() > 0)
			{
				size_t lastPosition = cdfEpochString.find_first_not_of(delimiters, 0);
				// Find first delimiter after the lastPosition
				size_t currentPosition = cdfEpochString.find_first_of(delimiters, lastPosition);

				while (currentPosition != std::string::npos || lastPosition != std::string::npos)
				{
					tokens.push_back(cdfEpochString.substr(lastPosition, currentPosition - lastPosition));

					// Find next character position that does not match any of the delimiters
					lastPosition = cdfEpochString.find_first_not_of(delimiters, currentPosition);

					// Find next delimiter
					currentPosition = cdfEpochString.find_first_of(delimiters, lastPosition);
				}

			}
			// Find next character position that does not match any of the delimiters
			lastPosition = cdfEpochString.find_first_not_of(delimiters, currentPosition);

			// Find next delimiter
			currentPosition = cdfEpochString.find_first_of(delimiters, lastPosition);
		}

	}

	tm time;
	time.tm_year = parseInt(tokens[0]) - 1900;
	time.tm_mon = parseInt(tokens[1]);
	time.tm_mday = parseInt(tokens[2]);
	time.tm_hour = parseInt(tokens[3]);
	time.tm_min = parseInt(tokens[4]);
	time.tm_sec = parseInt(tokens[5]);

//	setenv("TZ","UTC",1); //need to ensure the local timezone is UTC to convert the tm struct to
						  //a time_t epoch correctly.
	time_t timeEpoch = mktime(&time);
	return timeEpoch;


}


int main(int argc, char * argv[])
{
	IMF data;
	data.open("/Users/dberrios/data/IMF2.dat");
	tm targetTime;
	targetTime.tm_year = 2002-1900;
	targetTime.tm_mon = 9;
	targetTime.tm_mday = 8;
	targetTime.tm_hour = 4;
	targetTime.tm_min = 0;
	targetTime.tm_sec = 0;
	float xPosition;
	istringstream iss(argv[1]);
	iss >> xPosition;
//	targetTime.tm_zone = "GMT";
	time_t targetTimeEpoch = mktime(&targetTime);
	float radius;
	bool isInside;
	data.interpolate(targetTimeEpoch, xPosition, radius, isInside);
	vector<float> xpositions;
	vector<float> radii;
	string timeString = "2001-08-03T15:09:00.000Z";
	targetTimeEpoch = parseTime(timeString);
	data.calculateMagnetopause(targetTimeEpoch, xpositions, radii, -200.f, 20);
	cout << "-----returned values.  Radius: " << radius << " isInside: " << isInside << endl;


	return 0;
}
