/*
 * HDF5Reader.cpp
 *
 *  Created on: Jul 22, 2011
 *      Author: dberrios
 */

#include "H5Cpp.h"
#include <vector>
#include <string>
#include <iostream>

int main (int argc, char * argv[])
{

	long status = -1L;
	H5::H5File * current_file = NULL;

	try
	{
		current_file = new H5::H5File(argv[1], H5F_ACC_RDONLY);
	}
	catch(H5::Exception const& ex)
	{
		exit(1);
	}
	std::cout << "Opened file " << argv[1] << std::endl;

	long counts[1];
	std::vector<float>* variableData = new std::vector<float>();

	std::string variable = "rho";

	H5::Group group = current_file->openGroup("/");
	H5::Attribute attribute = group.openAttribute();
	/*H5::DataSet * dataset = new H5::DataSet(group.openDataSet(variable));

	H5::DataSpace dataspace = dataset->getSpace();
	int rank = dataspace.getSimpleExtentNdims(); //should be 1
	hsize_t count[1];
	hsize_t dims_out[1];
	hsize_t offset[1] = {0};
	int ndims = dataspace.getSimpleExtentDims(dims_out, NULL);
	std::cout << "dims_out[0]: " << dims_out[0] << " num dimensions: " << ndims << std::endl;
	float * buffer = new float[dims_out[0]];



	dataspace.selectHyperslab(H5S_SELECT_SET, dims_out, offset);

	hsize_t dim[] = {dims_out[0]};
	H5::DataSpace memspace( rank, dim);
	memspace.selectHyperslab(H5S_SELECT_SET, dim, offset);

	dataset->read(buffer, H5::PredType::NATIVE_FLOAT, memspace, dataspace);


	//add data to vector type, and delete original array
	variableData->reserve(dims_out[0]);
	for (int i = 0; i < dims_out[0]; i++)
	{
		variableData->push_back(buffer[i]);
	}


	delete[] buffer;
	delete dataset;
	std::cout << "finished reading " << variable << std::endl;
	std::cout << "size of variable: " << variableData->size() << std::endl;
	std::cout << "dims_out[0]: " << dims_out[0] << std::endl;
*/
	return 0;
}
