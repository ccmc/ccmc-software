#ifndef CCMC_TIME_H
#define CCMC_TIME_H

#include <string>
#include <iostream>
#include <ostream>

namespace ccmc
{
	class Time
	{
		public:
			int year;
			int month;
			int day;
			int hour;
			int minute;
			float second;

			friend std::ostream& operator<<(std::ostream& out, const Time time);

			std::string toString() const;

	};
}

#endif
