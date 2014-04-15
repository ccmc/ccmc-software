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

			Time(const std::string& epochString);
			Time(double epoch);
			Time();
			~Time();

			short getDay() const;
			void setDay(short day);
			void setEpoch(double epoch);
			short getHour() const;
			void setHour(short hour);
			short getMilliseconds() const;
			void setMilliseconds(short milliseconds);
			short getMinute() const;
			void setMinute(short minute);
			short getMonth() const;
			void setMonth(short month);
			short getSeconds() const;
			void setSeconds(double seconds);
			short getYear() const;
			void setYear(short year);
			std::string toString() const;
			double getEpoch() const; //we use CDF_EPOCH as the underlying data structure

		private:
			double epoch;
			short year;
			short month;
			short day;
			short hour;
			short minute;
			short seconds;
			short milliseconds;
			void updateEpoch();

	};

	std::ostream& operator<<(std::ostream& out, const Time time);
	double operator-(const Time& time1, const Time& time2);
	bool operator==(const Time& time1, const Time& time2);
	bool operator<(const Time& time1, const Time& time2);
	bool operator>(const Time& time1, const Time& time2);
	bool operator>=(const Time& time1, const Time& time2);
}

#endif
