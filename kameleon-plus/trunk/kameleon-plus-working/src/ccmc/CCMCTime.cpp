#include "CCMCTime.h"
#include "cdf.h"
#include "string.h"
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

namespace ccmc
{

	Time::Time()
	{
		//do nothing
		this->year = 0;
		this->month = 0;
		this->day = 0;
		this->hour = 0;
		this->minute = 0;
		this->seconds = 0;
		this->milliseconds = 0;
		//getEpoch();//update epoch values;
		updateEpoch();
	}
	Time::~Time()
	{
		//destructor for Time object
	}

	Time::Time(const std::string& timeString)
	{
		//extern "C" double parseEPOCH3(char epString[EPOCH3_STRING_LEN+1]);
		//extern "C" void encodeEPOCH3(double epoch, char epString[EPOCH3_STRING_LEN+1]);
		//ensure that it uses '-' instead of '/'
		char timeString_c_str[EPOCH3_STRING_LEN+1];
		strcpy(timeString_c_str, (char *)timeString.c_str());
		timeString_c_str[4] = '-';
		timeString_c_str[7] = '-';
		epoch = parseEPOCH3(timeString_c_str);
		//std::cout << "epoch value in parseTime: " << epoch << std::endl;
		long lyear, lmonth, lday, lhour, lminute, lsecond, lmsec;
		lyear = lmonth = lday = lhour = lminute = lsecond = lmsec = 1;
		EPOCHbreakdown(epoch, &lyear, &lmonth, &lday, &lhour, &lminute, &lsecond, &lmsec);
		year = ((short)lyear);
		month = ((short)lmonth);
		day = ((short)lday);
		hour = ((short)lhour);
		minute = ((short)lminute);
		seconds = ((short)lsecond);
		milliseconds = ((short)lmsec);
	}
	
	Time::Time(double epochTime)
	{
		EPOCHbreakdown(epochTime, (long*)&year, (long *)&month, (long*)&day, (long*)&hour, (long*)&minute, (long*)&seconds, (long*)&milliseconds);
		this->epoch = epochTime;
	}

	void Time::updateEpoch()
	{
		epoch = computeEPOCH(year, month, day, hour, minute, seconds, milliseconds);
		// std::cout << "epoch: " << epoch << std::endl;
	}

	std::string Time::toString() const
	{
		std::string timeString = "";
		std::string monthStr = "";
		std::string dayStr = "";
		std::string hourStr = "";
		std::string minuteStr = "";
		std::string secondStr = "";
		std::string yearStr = "";
		std::string msecStr = "";
		yearStr = (boost::format("%04d") % boost::lexical_cast<std::string>(year)).str();
		monthStr = (boost::format("%02d") % boost::lexical_cast<std::string>(month)).str();
		dayStr = (boost::format("%02d") % boost::lexical_cast<std::string>(day)).str();
		hourStr = (boost::format("%02d") % boost::lexical_cast<std::string>(hour)).str();
		minuteStr = (boost::format("%02d") % boost::lexical_cast<std::string>(minute)).str();
		secondStr = (boost::format("%02d") % boost::lexical_cast<std::string>(seconds)).str();
		msecStr = (boost::format("%03d") % boost::lexical_cast<std::string>(milliseconds)).str();

		timeString  = yearStr  +  "-" + monthStr + "-" + dayStr + "T" + hourStr + ":" + minuteStr + ":" + secondStr + "." + msecStr + "Z";
		return timeString;
	}


	std::ostream& operator<<(std::ostream& out, const Time time)
	{
		out << time.toString();
		return out;
	}

	double Time::getEpoch() const
	{
		return epoch;
	}

	double operator-(const Time& time1, const Time& time2)
	{
		return time1.getEpoch() - time2.getEpoch();

	}

	bool operator==(const Time& time1, const Time& time2)
	{
		if (time1.getYear() == time2.getYear() &&
			time1.getMonth() == time2.getMonth() &&
			time1.getDay() == time2.getDay() &&
			time1.getHour() == time2.getHour() &&
			time1.getMinute() == time2.getMinute() &&
			time1.getSeconds() == time2.getSeconds() &&
			time1.getMilliseconds() == time2.getMilliseconds())
			return true;
		else
		{
			// std::cout << "time1: " << time1.toString() << " time2: " << time2.toString() << std::endl;
			return false;
		}
	}

	bool operator<(const Time& time1, const Time& time2)
	{
		if (time1.getEpoch() < time2.getEpoch())
			return true;
		else
			return false;
	}

	bool operator>(const Time& time1, const Time& time2)
	{
		if (time1.getEpoch() > time2.getEpoch())
			return true;
		else
			return false;
	}

	bool operator>=(const Time& time1, const Time& time2)
	{
		if (time1 > time2 || time1 == time2)
			return true;
		else
			return false;
	}

	short Time::getDay() const {
		return day;
	}

	void Time::setDay(short day) {
		this->day = day;
		updateEpoch();
	}

	void Time::setEpoch(double epoch) {
		this->epoch = epoch;
	}

	short Time::getHour() const {
		return hour;
	}

	void Time::setHour(short hour) {
		this->hour = hour;
		updateEpoch();
	}

	short Time::getMilliseconds() const {
		return milliseconds;
	}

	void Time::setMilliseconds(short milliseconds) {
		this->milliseconds = milliseconds;
		updateEpoch();
	}

	short Time::getMinute() const {
		return minute;
	}

	void Time::setMinute(short minute) {
		this->minute = minute;
		updateEpoch();
	}

	short Time::getMonth() const {
		return month;
	}

	void Time::setMonth(short month) {
		this->month = month;
		updateEpoch();
	}

	short Time::getSeconds() const {
		return seconds;
	}

	void Time::setSeconds(double seconds) {
		this->seconds = seconds;
		updateEpoch();
	}

	short Time::getYear() const {
		return year;
	}

	void Time::setYear(short year) {
		this->year = year;
		updateEpoch();
	}
}
