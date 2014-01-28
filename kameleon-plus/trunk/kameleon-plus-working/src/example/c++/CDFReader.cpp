/*
 * HDF5Reader.cpp
 *
 *  Created on: Jul 22, 2011
 *      Author: dberrios
 */

#include <ccmc/CDFFileReader.h>
#include <vector>
#include <string>
#include <ccmc/cdf.h>
#include <iostream>
//#include <iterator>
//#include <algorithm>
//#include <boost/filesystem.hpp>


int main (int argc, char * argv[])
{

	if (argc < 2)
	{
		std::cout << "Usage: cdfreader path\n";
		return 1;
	}
	ccmc::CDFFileReader fileReader;
	int status = fileReader.open(argv[1]);
	//std::vector<int> * intmat = fileReader.getVariableInt("intmat");
	//std::vector<float> * coord = fileReader.getVariable("coord");
	//std::vector<float> * x = fileReader.getVariable("x");
	//std::vector<float> * y = fileReader.getVariable("y");
	if (status != ccmc::CDFFileReader::OK)
	{
		std::cout << argv[1] << " was not a valid CDF file" << std::endl;
		exit(1);
	}

	fileReader.close();
	CDFid current_file_id;
	status = CDFopenCDF(argv[1], &current_file_id);
	CDFsetReadOnlyMode(current_file_id, READONLYoff);
	std::string r_body = "r_body";
	std::string new_value = "2.5";
	long attrNum = CDFgetAttrNum(current_file_id, (char *)r_body.c_str());
	CDFputAttrgEntry(current_file_id, attrNum, 0,  CDF_CHAR, new_value.length(), (char *)new_value.c_str());
	CDFcloseCDF(current_file_id);
//	boost::filesystem::path path = argv[1];
//
//
//
//	try {
//		if (boost::filesystem::exists(path)) // does p actually exist?
//		{
//			if (boost::filesystem::is_regular_file(path)) // is p a regular file?
//				std::cout << path << " size is " << boost::filesystem::file_size(path) << '\n';
//
//			else if (boost::filesystem::is_directory(path)) // is p a directory?
//			{
//				std::cout << path << " is a directory containing:\n";
//
//				typedef std::vector<boost::filesystem::path> vec; // store paths,
//				vec v; // so we can sort them later
//
//				std::copy(boost::filesystem::directory_iterator(path), boost::filesystem::directory_iterator(),
//						std::back_inserter(v));
//
//				std::sort(v.begin(), v.end()); // sort, since directory iteration
//				// is not ordered on some file systems
//
//				for (vec::const_iterator it(v.begin()), it_end(v.end());
//						it != it_end; ++it) {
//					std::cout << "   " << *it << '\n';
//				}
//			} else
//				std::cout << path
//				<< " exists, but is neither a regular file nor a directory\n";
//		} else
//			std::cout << path << " does not exist\n";
//	}
//
//	catch (const boost::filesystem::filesystem_error& ex) {
//		std::cout << ex.what() << '\n';
//	}
//




	return 0;
}
