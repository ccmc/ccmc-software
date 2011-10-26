/**
*	attempts
*/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <ctime>
#include <ccmc/Kameleon.h>
//#include <ccmc/Kameleon_compatibility.h>
#include <boost/lexical_cast.hpp>
//#include <google/profiler.h>
//#include <ccmc/kameleon_adjusted.h>
#define LENGTH 500;
using namespace ccmc;

void Tokenize(const std::string& str,
                      std::vector<std::string>& tokens,
                      const std::string& delimiters = " ")
{
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

int main (int argc, char * argv[])
{

	std::string filename;
	std::string variable;
	int iterations = 10;



	std::string test_line = "03/04/21 09:12:00   29.36423    9.44891  -26.16772   29.36280    9.42723  -26.17715   26.49590   15.79579  -26.16772   -1.85945   30.11916  -26.93823   24.31326  -16.55745  -27.76699   24.31326  -11.03383  -30.38765   28.08693  -11.03383  -26.93823";

   /** Time                   GEI (RE)                      GEI/J2000 (RE)                      GEO (RE)                          GM (RE)                         GSE (RE)                         GSM (RE)                          SM (RE)
yy/mm/dd hh:mm:ss      X          Y          Z          X          Y          Z          X          Y          Z          X          Y          Z          X          Y          Z          X          Y          Z          X          Y          Z

00/07/18 00:00:00    7.68129    1.67608    5.01664    7.68176    1.67515    5.01623    1.87559    7.63503    5.01664   -7.45039    4.18915    3.73107   -0.14490   -8.45372    3.93597   -0.14490   -8.44147    3.96216   -1.34120   -8.44147    3.73107
00/07/18 00:03:00    7.66237    1.67695    5.05177    7.66284    1.67601    5.05137    1.96631    7.59326    5.05177   -7.38960    4.26207    3.77797   -0.12368   -8.44306    3.96787   -0.12368   -8.42716    4.00152   -1.32455   -8.42716    3.77797
00/07/18 00:06:00    7.64297    1.67771    5.08660    7.64345    1.67678    5.08619    2.05615    7.54997    5.08660   -7.32761    4.33364    3.82477   -0.10246   -8.43188    3.99951   -0.10246   -8.41214    4.04086   -1.30773   -8.41214    3.82477
...
**/
	//UNKNOWN, J2000, GEI, GEO, MAG, GSE, GSM, SM, RTN, GSEQ, HEE, HAE, HEEQ
	//int cxform(const char *from,const char *to,const double et,Vec v_in,Vec v_out);

	std::string date;
	std::string time;
	std::string GEI[3];
	std::string GEIJ2000[3];
	std::string GEO[3];
	std::string GM[3];
	std::string GSE[3];
	std::string GSM[3];
	std::string SM[3];

	std::string buf; // Have a buffer string
	std::stringstream ss(test_line); // Insert the string into a stream

	std::vector<std::string> tokens; // Create vector to hold our words

	while (ss >> buf)
		tokens.push_back(buf);
	date = tokens[0];
	time = tokens[1];
	GEI[0] = tokens[2];
	GEI[1] = tokens[3];
	GEI[2] = tokens[4];
	GEIJ2000[0] = tokens[5];
	GEIJ2000[1] = tokens[6];
	GEIJ2000[2] = tokens[7];
	GEO[0] = tokens[8];
	GEO[1] = tokens[9];
	GEO[2] = tokens[10];
	GM[0] = tokens[11];
	GM[1] = tokens[12];
	GM[2] = tokens[13];
	GSE[0] = tokens[14];
	GSE[1] = tokens[15];
	GSE[2] = tokens[16];
	GSM[0] = tokens[17];
	GSM[1] = tokens[18];
	GSM[2] = tokens[19];
	SM[0] = tokens[20];
	SM[1] = tokens[21];
	SM[2] = tokens[22];
/*
	std::vector<std::string> time_tokens;
	std::vector<std::string> date_tokens;
	Tokenize(time, time_tokens, " :");
	Tokenize(date, date_tokens, " /");
	std::cout << "date: " + date << std::endl;
	//long Kameleon::date2es(int yyyy, int mm, int dd, int hh, int mm2, int ss)
	std::cout << "date_tokens[0]: " << date_tokens[0] << std::endl;
	int years = boost::lexical_cast<int>(date_tokens[0]);
	if (years < 50)
		years = years + 2000;
	else if (years < 1900)
		 		years = years + 1900;
	long time_et = Kameleon::_date2es(years,
			boost::lexical_cast<int>(date_tokens[1]),
			boost::lexical_cast<int>(date_tokens[2]),
			boost::lexical_cast<int>(time_tokens[0]),
			boost::lexical_cast<int>(time_tokens[1]),
			boost::lexical_cast<int>(time_tokens[2]));

	std::cout << "GSE: " << GSE[0] << " " << GSE[1] << " " << GSE[2] << std::endl;
	Position GSE_d;
	GSE_d.c0 = boost::lexical_cast<double>(GSE[0]);
	GSE_d.c1 = boost::lexical_cast<double>(GSE[1]);
	GSE_d.c2 = boost::lexical_cast<double>(GSE[2]);

	Position SM_d;
	Kameleon::_cxform("GSE","SM", time_et, &GSE_d, &SM_d);
	std::cout << "Input: " << GSE_d.c0 << "," << GSE_d.c1 << "," << GSE_d.c2 << std::endl;
	std::cout << "Output: " << SM_d.c0 << "," << SM_d.c1 << "," << SM_d.c2 << std::endl;
*/
	return 0;
}


