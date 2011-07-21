#include "CCMCTime.h"
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

namespace ccmc
{
	std::string Time::toString() const
	{
		std::string timeString = "";
		std::string monthStr = "";
		std::string dayStr = "";
		std::string hourStr = "";
		std::string minuteStr = "";
		std::string secondStr = "";
		std::string yearStr = "";
		yearStr = (boost::format("%04d") % boost::lexical_cast<std::string>(year)).str();
		monthStr = (boost::format("%02d") % boost::lexical_cast<std::string>(month)).str();
		dayStr = (boost::format("%02d") % boost::lexical_cast<std::string>(day)).str();
		hourStr = (boost::format("%02d") % boost::lexical_cast<std::string>(hour)).str();
		minuteStr = (boost::format("%02d") % boost::lexical_cast<std::string>(minute)).str();
		secondStr = (boost::format("%02d") % boost::lexical_cast<std::string>(second)).str();

		timeString  = yearStr  +  "/" + monthStr + "/" + dayStr + " " + hourStr + ":" + minuteStr + ":" + secondStr;
		return timeString;
	}


	std::ostream& operator<<(std::ostream& out, const Time time)
	{
		out << time.toString();
		return out;
	}

}
