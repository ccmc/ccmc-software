/*
 * HDFFileReader.cpp
 *
 *  Created on: July 12, 2011
 *      Author: David Berrios
 */
// #include "config.h"
#include "kameleon-plus-Config.h" //controlled by CMakeLists.txt
#ifdef HAVE_HDF5 
#include "HDF5FileReader.h"
#include <string>
#include <vector>
#include <deque>
#include <iostream>
#include <fstream>
#include <queue>
#include <boost/current_function.hpp>

using namespace std;


namespace ccmc
{
	/**
	 * Default constructor. Does nothing.
	 */
	HDF5FileReader::HDF5FileReader()
	{
		this->current_file = NULL;
		this->rootGroup = NULL;
		this->variableGroup = NULL;
	}



	/**
	 * Opens a new file. If the previous file has been opened successfuly, and has the
	 * same filename as the requested filename, nothing will be done.
	 * @param filename
	 * @return The status of the open call.  This method should be called from open().
	 */
	long HDF5FileReader::openFile(const std::string& filename, bool readonly)
	{
		std::cerr << "HDF5FileReader::openFile" << std::endl;
		long status = -1L;
		if (current_file != NULL && filename != current_filename)
		{
			close();
		}

		if(!(ifstream(filename.c_str())))
		{
			status = FILE_DOES_NOT_EXIST;
			std::cerr << "Filename: \"" << filename << "\" does not exist." << std::endl;
		}
		else{
			std::cerr << "about to open HDF5 file" << std::endl;

			try
			{
				current_file = new H5::H5File(filename, H5F_ACC_RDONLY);
				this->rootGroup = new H5::Group(current_file->openGroup("/"));
				this->variableGroup = new H5::Group(current_file->openGroup("Variables"));
				int numVariables = variableGroup->getNumObjs();
				std::cerr << "num variables: " << numVariables << std::endl;
				for (int i = 0; i < numVariables; i++)
				{
					//std::cerr << "variable: " << variableGroup->getObjnameByIdx(i) << std::endl;

				}
				status = OK;
			}
			catch(H5::Exception const& ex)
			{
				status = OPEN_ERROR;
				if (current_file != NULL)
				{
					delete current_file;
					current_file = NULL;
				}
				if (rootGroup != NULL)
				{
					delete rootGroup;
					rootGroup = NULL;
				}
				if (variableGroup != NULL)
				{
					delete variableGroup;
					variableGroup = NULL;
				}
			}



			//file(filename, H5::H5F_ACC_RDONLY);
			//status = CDFopenCDF((char *)filename.c_str(), &current_file_id);


			if (status == OK)
			{
				current_filename = filename;
				FileReader::initializeGlobalAttributes();
				FileReader::initializeVariableAttributes();
				initializeVariableIDs();
				initializeVariableNames();
				//status = OK;
			} else
			{
				status = OPEN_ERROR;
				// std::cerr << "Error opening hdf \"" << filename << "\". Not an hdf file?" << std::endl;
			}

		}
		//cout << "current_file_id: " << current_file_id << endl;
		//cout << "testing open in FileReader class" << endl;

		return status;
	}

	/**
	 * Closes the currently selected file.  Call this from the close() method.
	 * @return Status of close operation.
	 */
	long HDF5FileReader::closeFile()
	{
		if (current_file != NULL)
		{
			current_file->close();
			current_file = NULL;
		}

		return FileReader::OK;
	}

	/**
	 * @brief Returns a pointer to a std::vector<float> containing the values of the selected variable
	 *
	 * This allocates a new std::vector<float> pointer.  Make sure you
	 * delete the contents when you done using it, or you will have a memory leak.
	 *
	 * @param variable
	 * @return std::vector<float> containing the values of the selected variable.
	 */
	std::vector<float>* HDF5FileReader::getVariable(const std::string& variable)
	{
		std::vector<float>* variableData = new std::vector<float>();

		if (this->doesVariableExist(variable))
		{
			//std::cout << "reading " << variable << std::endl;
			//get variable number
//			long variableNum = this->getVariableID(variable);

			//std::cout << "variableNum for " << variable << ": " << variableNum << std::endl;
			//get dim sizes

			H5::Group group = this->current_file->openGroup("Variables");
			//cout << "variable: " << variable << ": " << counts[0] << endl;
			H5::DataSet * dataset = new H5::DataSet(group.openDataSet(variable));
			H5::DataSpace dataspace = dataset->getSpace();
			int rank = dataspace.getSimpleExtentNdims(); //should be 1
			hsize_t count[1];
			hsize_t offset[1] = {0};
//			int ndims = dataspace.getSimpleExtentDims(count, NULL);

			//std::cout << "count[0]: " << count[0] << std::endl;
			float * buffer = new float[count[0]];



			dataspace.selectHyperslab(H5S_SELECT_SET, count, offset);

			H5::DataSpace memspace( rank, count);
			memspace.selectHyperslab(H5S_SELECT_SET, count, offset);

			dataset->read(buffer, H5::PredType::NATIVE_FLOAT, memspace, dataspace);
			//std::cout << "after read" << std::endl;

			//add data to vector type, and delete original array
			variableData->reserve(count[0]);
			for (int i = 0; i < count[0]; i++)
			{
				variableData->push_back(buffer[i]);
			}
			//std::cout << "after adding to variableData vector" << std::endl;

			delete[] buffer;
			delete dataset;
			//std::cout << "finished reading " << variable << std::endl;
			//std::cout << "size of variable: " << variableData.size() << std::endl;
			//std::cout << "dimSizes[0]: " << dimSizes[0] << std::endl;

		}

		return variableData;
	}

	/**
	 * Returns a pointer to a std::vector<float> containing the values of the selected variable
	 * in the range specified by the startIndex and count (the number of records to read) stored in the selected file.
	 * This allocates a new std::vector<float> pointer.  Make sure you
	 * delete the contents when you done using it, or you will have a memory leak.
	 * @param variable
	 * @param startIndex
	 * @param count
	 * @return std::vector<float> containing the values of the selected variable.
	 */
	std::vector<float>* HDF5FileReader::getVariable(const std::string& variable, long indexOffset, long length)
	{

		std::vector<float>* variableData = new std::vector<float>();

		if (this->doesVariableExist(variable))
		{
			//std::cout << "reading " << variable << std::endl;
			//get variable number
//			long variableNum = this->getVariableID(variable);

			//get dim sizes

			H5::Group group = this->current_file->openGroup("Variables");
			//cout << "variable: " << variable << ": " << counts[0] << endl;
			H5::DataSet * dataset = new H5::DataSet(group.openDataSet(variable));
			H5::DataSpace dataspace = dataset->getSpace();
			int rank = dataspace.getSimpleExtentNdims(); //should be 1
			hsize_t count[1] = {static_cast<hsize_t>(length)};
			hsize_t offset[1] = {static_cast<hsize_t>(indexOffset)};
			//int ndims = dataspace.getSimpleExtentDims(count, NULL);
			float * buffer = new float[count[0]];



			dataspace.selectHyperslab(H5S_SELECT_SET, count, offset);

			hsize_t dim[] = {count[0]};
			H5::DataSpace memspace( rank, dim);
			memspace.selectHyperslab(H5S_SELECT_SET, dim, offset);

			dataset->read(buffer, H5::PredType::NATIVE_FLOAT, memspace, dataspace);


			//add data to vector type, and delete original array
			variableData->reserve(count[0]);
			for (int i = 0; i < count[0]; i++)
			{
				variableData->push_back(buffer[i]);
			}

			delete[] buffer;
			delete dataset;
			//std::cout << "finished reading " << variable << std::endl;
			//std::cout << "size of variable: " << variableData.size() << std::endl;
			//std::cout << "dimSizes[0]: " << dimSizes[0] << std::endl;

		}

		return variableData;
	}

	/**
	 * Returns a pointer to a std::vector<float> containing the values of the selected variable
	 * in the range specified by the startIndex and count (the number of records to read) stored
	 * in the selected file.  This allocates a new std::vector<float> pointer.  Make sure you
	 * delete the contents when you done using it, or you will have a memory leak.
	 * @param variableID
	 * @param startIndex
	 * @param count
	 * @return std::vector<float> containing the values of the selected variable.
	 */
	std::vector<float>* HDF5FileReader::getVariable(long variable, long indexOffset, long count)
	{

		std::vector<float>* variableData = new std::vector<float>();



		//get dim sizes
		H5::Group group = this->current_file->openGroup("Variables");
		//cout << "variable: " << variable << ": " << counts[0] << endl;
		H5::DataSet * dataset = new H5::DataSet(group.openDataSet(group.getObjnameByIdx(variable)));
		H5::DataSpace dataspace = dataset->getSpace();
		int rank = dataspace.getSimpleExtentNdims(); //should be 1
		hsize_t length[1] = {static_cast<hsize_t>(count)};
		hsize_t offset[1] = {static_cast<hsize_t>(indexOffset)};
		//int ndims = dataspace.getSimpleExtentDims(count, NULL);
		float * buffer = new float[length[0]];



		dataspace.selectHyperslab(H5S_SELECT_SET, length, offset);

		hsize_t dim[] = {length[0]};
		H5::DataSpace memspace( rank, dim);
		memspace.selectHyperslab(H5S_SELECT_SET, dim, offset);

		dataset->read(buffer, H5::PredType::NATIVE_FLOAT, memspace, dataspace);


		//add data to vector type, and delete original array
		variableData->reserve(length[0]);
		for (int i = 0; i < length[0]; i++)
		{
			variableData->push_back(buffer[i]);
		}

		delete[] buffer;
		delete dataset;
		//std::cout << "finished reading " << variable << std::endl;
		//std::cout << "size of variable: " << variableData.size() << std::endl;
		//std::cout << "dimSizes[0]: " << dimSizes[0] << std::endl;
		return variableData;
	}

	/**
	 * Returns a pointer to a std::vector<float> containing the values of the selected variable
	 * stored in the selected file.  This allocates a new std::vector<float> pointer.  Make sure you
	 * delete the contents when you done using it, or you will have a memory leak.
	 * @param variable
	 * @return std::vector<float> containing the values of the selected variable.
	 */
	std::vector<float>* HDF5FileReader::getVariable(long variable)
	{

		std::vector<float>* variableData = new std::vector<float>();

		//get dim sizes
		H5::Group group = this->current_file->openGroup("Variables");
		//cout << "variable: " << variable << ": " << counts[0] << endl;
		H5::DataSet * dataset = new H5::DataSet(group.openDataSet(group.getObjnameByIdx(variable)));
		H5::DataSpace dataspace = dataset->getSpace();
		int rank = dataspace.getSimpleExtentNdims(); //should be 1
		hsize_t count[1];
		hsize_t offset[1] = {0};
//		int ndims = dataspace.getSimpleExtentDims(count, NULL);
		float * buffer = new float[count[0]];



		dataspace.selectHyperslab(H5S_SELECT_SET, count, offset);

		hsize_t dim[] = {count[0]};
		H5::DataSpace memspace( rank, dim);
		memspace.selectHyperslab(H5S_SELECT_SET, dim, offset);

		dataset->read(buffer, H5::PredType::NATIVE_FLOAT, memspace, dataspace);


		//add data to vector type, and delete original array
		variableData->reserve(count[0]);
		for (int i = 0; i < count[0]; i++)
		{
			variableData->push_back(buffer[i]);
		}

		delete[] buffer;
		delete dataset;
		//std::cout << "finished reading " << variable << std::endl;
		//std::cout << "size of variable: " << variableData.size() << std::endl;
		//std::cout << "dimSizes[0]: " << dimSizes[0] << std::endl;
		return variableData;
	}


	/**
	 * @brief Returns a value in the flat array of the variable and index requested.
	 *
	 * Use this method on variables that have a type of float
	 *
	 * returns std::numeric_limits<float>::min() if missing
	 *
	 * @param variable The variable in the file
	 * @param index The index in the variable's array in the file
	 *
	 * @return float of the value in the array.
	 */
	float HDF5FileReader::getVariableAtIndex(const std::string& variable, long index)
	{

		float value = std::numeric_limits<float>::min();
		if (this->doesVariableExist(variable))
		{
			std::cout << "reading " << variable << std::endl;
			//get variable number
//			long variableNum = this->getVariableID(variable);

			//get dim sizes

			H5::Group group = this->current_file->openGroup("Variables");
			//cout << "variable: " << variable << ": " << counts[0] << endl;
			H5::DataSet * dataset = new H5::DataSet(group.openDataSet(variable));
			H5::DataSpace dataspace = dataset->getSpace();
			int rank = dataspace.getSimpleExtentNdims(); //should be 1
			hsize_t count[1];
			hsize_t offset[1] = {0};
//			int ndims = dataspace.getSimpleExtentDims(count, NULL);
			float * buffer = new float[count[0]];



			dataspace.selectHyperslab(H5S_SELECT_SET, count, offset);

			hsize_t dim[] = {count[0]};
			H5::DataSpace memspace( rank, dim);
			memspace.selectHyperslab(H5S_SELECT_SET, dim, offset);

			dataset->read(buffer, H5::PredType::NATIVE_FLOAT, memspace, dataspace);


			//add data to vector type, and delete original array
			value = buffer[0];
			delete[] buffer;
			delete dataset;
		}
		return value;
			//std::cout << "finished reading " << variable << std::endl;
			//std::cout << "size of variable: " << variableData.size() << std::endl;
			//std::cout << "dimSizes[0]: " << dimSizes[0] << std::endl;

	}

	/**
	 * This allocates a new std::vector<int> pointer.  Make sure you
	 * delete the contents when you done using it, or you will have a memory leak.
	 * @param variable
	 * @return vector<int> containing the integer values of the variable
	 */
	std::vector<int>* HDF5FileReader::getVariableInt(const std::string& variable)
	{

		std::vector<int>* variableData = new std::vector<int>();


			//std::cout << "reading " << variable << std::endl;
			//get variable number
//			long variableNum = this->getVariableID(variable);

			//get dim sizes

			H5::Group group = this->current_file->openGroup("Variables");
			//cout << "variable: " << variable << ": " << counts[0] << endl;
			H5::DataSet * dataset = new H5::DataSet(group.openDataSet(variable));
			H5::DataSpace dataspace = dataset->getSpace();
			int rank = dataspace.getSimpleExtentNdims(); //should be 1
			hsize_t count[1];
			hsize_t offset[1] = {0};
//			int ndims = dataspace.getSimpleExtentDims(count, NULL);
			float * buffer = new float[count[0]];



			dataspace.selectHyperslab(H5S_SELECT_SET, count, offset);

			hsize_t dim[] = {count[0]};
			H5::DataSpace memspace( rank, dim);
			memspace.selectHyperslab(H5S_SELECT_SET, dim, offset);

			dataset->read(buffer, H5::PredType::NATIVE_INT, memspace, dataspace);


			//add data to vector type, and delete original array
			variableData->reserve(count[0]);
			for (int i = 0; i < count[0]; i++)
			{
				variableData->push_back(buffer[i]);
			}

			delete[] buffer;
			delete dataset;
			//std::cout << "finished reading " << variable << std::endl;
			//std::cout << "size of variable: " << variableData.size() << std::endl;
			//std::cout << "dimSizes[0]: " << dimSizes[0] << std::endl;
			return variableData;

	}

	/**
	 * @param variableNum
	 * @param index
	 * @return
	 */
	float HDF5FileReader::getVariableAtIndex(long variable, long index)
	{

		float value = std::numeric_limits<float>::min();

		//get dim sizes

		H5::Group group = this->current_file->openGroup("Variables");
		//cout << "variable: " << variable << ": " << counts[0] << endl;
		H5::DataSet * dataset = new H5::DataSet(group.openDataSet(group.getObjnameByIdx(variable)));
		H5::DataSpace dataspace = dataset->getSpace();
		int rank = dataspace.getSimpleExtentNdims(); //should be 1
		hsize_t count[1] = {1};
		hsize_t offset[1] = {static_cast<hsize_t>(index)};
		//int ndims = dataspace.getSimpleExtentDims(count, NULL);
		float * buffer = new float[count[0]];



		dataspace.selectHyperslab(H5S_SELECT_SET, count, offset);

		hsize_t dim[] = {count[0]};
		H5::DataSpace memspace( rank, dim);
		memspace.selectHyperslab(H5S_SELECT_SET, dim, offset);

		dataset->read(buffer, H5::PredType::NATIVE_FLOAT, memspace, dataspace);


		//add data to vector type, and delete original array
		value = buffer[0];
		delete[] buffer;
		delete dataset;
		return value;
			//std::cout << "finished reading " << variable << std::endl;
			//std::cout << "size of variable: " << variableData.size() << std::endl;
			//std::cout << "dimSizes[0]: " << dimSizes[0] << std::endl;

	}

	/**
	 * @brief Returns a value in the flat array of the variable and index requested.
	 *
	 * Use this method on variables that have a type of int
	 *
	 * @param variable The variable in the file
	 * @param index The index in the variable's array in the file
	 *
	 * @return int of the value in the array.
	 */
	int HDF5FileReader::getVariableIntAtIndex(const std::string& variable, long index)
	{
//		long counts[1];
		int value = std::numeric_limits<int>::min();
		if (this->doesVariableExist(variable))
		{
			//std::cout << "reading " << variable << std::endl;
			//get variable number
//			long variableNum = this->getVariableID(variable);

			//get dim sizes

			H5::Group group = this->current_file->openGroup("Variables");
			//cout << "variable: " << variable << ": " << counts[0] << endl;
			H5::DataSet * dataset = new H5::DataSet(group.openDataSet(variable));
			H5::DataSpace dataspace = dataset->getSpace();
			int rank = dataspace.getSimpleExtentNdims(); //should be 1
			hsize_t count[1] = {1};
			hsize_t offset[1] = {0};
			//int ndims = dataspace.getSimpleExtentDims(count, NULL);
			float * buffer = new float[count[0]];



			dataspace.selectHyperslab(H5S_SELECT_SET, count, offset);

			hsize_t dim[] = {count[0]};
			H5::DataSpace memspace( rank, dim);
			memspace.selectHyperslab(H5S_SELECT_SET, dim, offset);

			dataset->read(buffer, H5::PredType::NATIVE_INT, memspace, dataspace);


			//add data to vector type, and delete original array
			value = buffer[0];
			delete[] buffer;
			delete dataset;
		}
		return value;
			//std::cout << "finished reading " << variable << std::endl;
			//std::cout << "size of variable: " << variableData.size() << std::endl;
			//std::cout << "dimSizes[0]: " << dimSizes[0] << std::endl;


	}

	/**
	 * @param i The attribute number
	 * @return
	 */
	Attribute HDF5FileReader::getGlobalAttribute(long i)
	{
#ifdef DEBUG
		std::cerr << "entered " << BOOST_CURRENT_FUNCTION << " i = " << (int)i << std::endl;
#endif
		H5::Group group = this->current_file->openGroup("/");
		// std::cout<< "group assigned\n";
		H5::Attribute h5attribute = group.openAttribute((unsigned int)i);
		// std::cout<< "attribute opened\n";
		H5::DataType dataType = h5attribute.getDataType();
		// std::cout<< "dataType retrieved\n";
		Attribute attribute;
		// std::cout << "checking dataType"<<std::endl;
		if (dataType.getClass() == H5T_STRING)
		{
			std::string attributeValue = "NULL";
			h5attribute.read(dataType, attributeValue);

			std::string attributeName = "";
			attributeName = h5attribute.getName();


			attribute.setAttributeName(attributeName);
			//std::cout << "name: '" << attributeName << "' string attributeBuffer: '" << attributeValue << "'"<< std::endl;
			attribute.setAttributeValue(attributeValue);
			gAttributeByID[i] = attribute;
			gAttributes[attribute.getAttributeName()] = attribute;
			//return attribute;
		} else if (dataType.getClass() == H5T_INTEGER)
		{
			//int attributeValue = 0.f;
			int attributeBuffer;// = new int[1];

			h5attribute.read(dataType, &attributeBuffer);
			std::string attributeName = "";
			attributeName = h5attribute.getName();
			attribute.setAttributeName(attributeName);
			//std::cout << "int attributeBuffer: '" << attributeBuffer << "'"<< std::endl;
			attribute.setAttributeValue(attributeBuffer);
			gAttributeByID[i] = attribute;
			gAttributes[attribute.getAttributeName()] = attribute;
			//return attribute;
		} else if (dataType.getClass() == H5T_FLOAT)//CDF_FLOAT
		{
			//int attributeValue = 0.f;
			float attributeValue;// = new int[1];

			h5attribute.read(dataType, &attributeValue);
			std::string attributeName = "";
			attributeName = h5attribute.getName();
			attribute.setAttributeName(attributeName);
			//std::cout << "float attributeBuffer: '" << attributeValue << "'"<< std::endl;

			attribute.setAttributeValue(attributeValue);
			gAttributeByID[i] = attribute;
			gAttributes[attribute.getAttributeName()] = attribute;
			//return attribute;
		}

		//std::cout << "added: " << i << " name: '" << attribute.getAttributeName() << "'" << endl;
		//std::cout << "Attribute: " << attribute.toString() << std::endl;
		return attribute;

	}

	/**
	 * @param attribute
	 * @return
	 */
	Attribute HDF5FileReader::getGlobalAttribute(const std::string& attribute)
	{
		//first, check if the attribute has already been requested. If so, return stored value
		boost::unordered_map<std::string, Attribute>::iterator iter = gAttributes.find(attribute);
		if (iter != gAttributes.end())
			return (*iter).second;

//		std::cout << "after search in getGlobalAttribute(const std::string& attribute)" << std::endl;
//		std::cout << "attribute: " << attribute << std::endl;
		H5::Group group = this->current_file->openGroup("/");
		H5::Attribute h5attribute = group.openAttribute(attribute);
		long attrNum = h5attribute.getId();
		H5::DataType dataType = h5attribute.getDataType();
//		std::cout << "attrNum after attribute: " << attrNum << std::endl;
		Attribute current_attribute;
		if (attrNum < 0)
		{
			std::cout << "attrNum: " << attrNum << " returned for " << attribute << std::endl;
		}
		else
		{
//			std::cout << "attribute: " << attribute << " attribute number: " << attrNum << std::endl;
//			std::cout << "attempting to get attribute without using attribute number\n";
			if (dataType.getClass() == H5T_STRING)
			{
				std::string attributeValue = "NULL";
				h5attribute.read(dataType, attributeValue);

				std::string attributeName = "";
				attributeName = h5attribute.getName();


				current_attribute.setAttributeName(attributeName);
				//std::cout << "name: '" << attributeName << "' string attributeBuffer: '" << attributeValue << "'"<< std::endl;
				current_attribute.setAttributeValue(attributeValue);
				gAttributeByID[(int)attrNum] = current_attribute;
				gAttributes[current_attribute.getAttributeName()] = current_attribute;
				//return attribute;
			} else if (dataType.getClass() == H5T_INTEGER)
			{
				//int attributeValue = 0.f;
				int attributeBuffer;// = new int[1];

				h5attribute.read(dataType, &attributeBuffer);
				std::string attributeName = "";
				attributeName = h5attribute.getName();
				current_attribute.setAttributeName(attributeName);
				//std::cout << "int attributeBuffer: '" << attributeBuffer << "'"<< std::endl;
				current_attribute.setAttributeValue(attributeBuffer);
				gAttributeByID[(int)attrNum] = current_attribute;
				gAttributes[current_attribute.getAttributeName()] = current_attribute;
				//return attribute;
			} else if (dataType.getClass() == H5T_FLOAT)//CDF_FLOAT
			{
				//int attributeValue = 0.f;
				float attributeValue;// = new int[1];

				h5attribute.read(dataType, &attributeValue);
				std::string attributeName = "";
				attributeName = h5attribute.getName();
				current_attribute.setAttributeName(attributeName);
				//std::cout << "float attributeBuffer: '" << attributeValue << "'"<< std::endl;

				current_attribute.setAttributeValue(attributeValue);
				gAttributeByID[(int)attrNum] = current_attribute;
				gAttributes[current_attribute.getAttributeName()] = current_attribute;
				//return attribute;
			}

		}//gAttributes[attribute] = current_attribute;
		return current_attribute;
//		return Attribute();
	}

	/**
		 * @param attribute
		 * @return
		 */
		long HDF5FileReader::getGlobalAttributeID(const std::string& attribute)
		{
			//long attrNum = CDFgetAttrNum(current_file_id, (char *) attribute.c_str());
			long attrNum = 0;
			return attrNum;
		}

	/**
	 * @param variable
	 * @param vattribute
	 * @return
	 */
	Attribute HDF5FileReader::getVariableAttribute(const std::string& variable, const std::string& vattribute)
	{


		//first, check the vAttributes map
//		std::cout<<"Checking variable attributes map\n";
		boost::unordered_map<std::string, boost::unordered_map< std::string, Attribute> >::iterator iter =
				vAttributes.find(variable);
		if (iter != vAttributes.end())
		{
			boost::unordered_map< std::string, Attribute>::iterator iter2 = vAttributes[variable].find(vattribute);
			if (iter2 != vAttributes[variable].end())
			{
				return (*iter2).second;
			}
		}
	//	std::cout<<"Attribute not loaded, opening Variables group\n";

		H5::Group group = this->current_file->openGroup("Variables");
	//	std::cout<<"Group opened. Creating memory for H5::DataSet\n";
		H5::DataSet * dataset = new H5::DataSet(group.openDataSet(variable));
	//	std::cout<<"creating h5attribute for variable\n";
		H5::Attribute h5attribute = dataset->openAttribute(vattribute); //changed from group.openAttribute(vattribute);
	//	std::cout<<"attribute opened, obtaining data type\n";
		H5::DataType dataType = h5attribute.getDataType();
		Attribute attribute;
	//	std::cerr<<"Retrieving Variable attribute info:"<<std::endl;
		if (dataType.getClass() == H5T_STRING)
		{
//			std::cout<<"String type variable attribute\n";
			std::string attributeValue = "NULL";
			h5attribute.read(dataType, &attributeValue);

			std::string attributeName = "";
			attributeName = h5attribute.getName();


			attribute.setAttributeName(attributeName);
			//std::cout << "attributeBuffer: " << attributeBuffer << endl;
			attribute.setAttributeValue(attributeValue);
			//return attribute;
		} else if (dataType.getClass() == H5T_INTEGER) //shouldn't this be H5T_INT or something?
		{
//			std::cout<<"Int type variable attribute\n";
			//int attributeValue = 0.f;
			int attributeBuffer;// = new int[1];

			h5attribute.read(dataType, &attributeBuffer);
			std::string attributeName = "";
			attributeName = h5attribute.getName();
			attribute.setAttributeName(attributeName);
			attribute.setAttributeValue(attributeBuffer);
			//return attribute;
		} else if (dataType.getClass() == H5T_FLOAT)//CDF_FLOAT
		{
//			std::cout<<"Float type variable attribute\n";
			//int attributeValue = 0.f;
			float attributeValue;// = new int[1];

			h5attribute.read(dataType, &attributeValue);
			std::string attributeName = "";
			attributeName = h5attribute.getName();
			attribute.setAttributeName(attributeName);
			attribute.setAttributeValue(attributeValue);
			//return attribute;
		}

		//cout << "added: " << i << " name: " << attribute.getAttributeName() << endl;
		//std::cout << "Attribute: " << attribute.toString() << std::endl;


		(vAttributes[variable])[vattribute] = attribute;
		return attribute;

	}

	/**
	 * TODO
	 * Retrieves the number of global attributes in the selected file.  This is useful for iterating over all
	 * available global attributes.
	 * @return The number of global attributes stored in the selected file.
	 */
	int HDF5FileReader::getNumberOfGlobalAttributes()
	{
		long num_attributes;
		H5::Group group = this->current_file->openGroup("/");
		num_attributes = group.getNumAttrs();
		return (int)num_attributes;
	}

	/**
	 * @param attribute
	 * @return
	 */
	bool HDF5FileReader::doesAttributeExist(const std::string& attribute)
	{
		bool exists = false;
		H5::Group group = this->current_file->openGroup("/");
		try
		{
			group.openAttribute(attribute);
			exists = true;
		} catch (H5::AttributeIException& e)
		{
			exists = false;
		}
		return exists;

	}

	/**
	 * Returns the variable ID as a long. Using the variable ID wherever possible is significantly faster than the
	 * equivalent methods accepting the variable string.
	 * @return Status of the file operation.
	 */
	long HDF5FileReader::getVariableID(const std::string& variable)
	{
		//first, check the current variableIDs map.  fetching the variableID from the file is expensive
		boost::unordered_map<std::string, long>::iterator iter = variableIDs.find(variable);
		if (iter != variableIDs.end())
			return (*iter).second;
		else
		{
			//since we prefetched all of the variables on open, it shouldn't exist
			return -1;
		}

		/*std::cout << "getting variable id for: " << variable << std::endl;
		long variableNumber = CDFgetVarNum((void *) current_file_id, (char *) variable.c_str());
		//std::cout << "variableNumber: " << variableNumber << std::endl;
		if (variableNumber >= 0) //we want to keep the crappy negative ids
			variableIDs[variable] = variableNumber;

		return variableNumber;*/
	}

	/**
	 * Returns the string representation of the variable referred to by variable_id
	 * @return String representation of the variable.
	 */
	std::string HDF5FileReader::getVariableName(long variable)
	{
		//first, check the current variableNames map.  fetching the variableNames from the file is expensive
		boost::unordered_map<long, std::string>::iterator iter = variableNames.find(variable);
		if (iter != variableNames.end())
			return (*iter).second;
		std::string variableName = this->variableGroup->getObjnameByIdx(variable);

		if (variableName != "")
		{
			variableNames[variable] = variableName;
		}
		return variableName;
	}

	/**
	 * @param variable
	 * @return
	 */
	bool HDF5FileReader::doesVariableExist(const std::string& variable)
	{

		//first, check the current variableIDs map.  fetching the variableID from the file is expensive
		boost::unordered_map<std::string, long>::iterator iter = variableIDs.find(variable);
		if (iter != variableIDs.end())
			return true;
/*
		try
		{
			H5::DataSet * dataset = new H5::DataSet(this->variableGroup->openDataSet(variable));
			long variableNumber = dataset->getId();
			//std::cout << "variableNumber: " << variableNumber << std::endl;
			if (variableNumber >= 0)
			{
				variableIDs[variable] = variableNumber;
				variableNames[variableNumber] = variable;
				return true;
			}
		} catch (H5::Exception const& ex)
		{

		}*/

		return false;
	}

	/**
	 * Returns the current filename.
	 * @return The current filename.
	 */
	const std::string& HDF5FileReader::getCurrentFilename()
	{
		return this->current_filename;
	}

	/**
	 *@param attribute_id
	 *@return
	 */
	std::string HDF5FileReader::getGlobalAttributeName(long attribute_id)
	{
//		CDFgetAttrName((void *) current_file_id, attribute_id, buffer);
		H5::Attribute attribute = this->rootGroup->openAttribute(attribute_id);
		std::string buffer_string = attribute.getName();
		//cout << "Attribute Name: '" << buffer_string << "'" << endl;


		return buffer_string;
	}

	/**
	 *@return
	 */
	int HDF5FileReader::getNumberOfVariables()
	{
		long numVars;
		H5::Group group = this->current_file->openGroup("Variables");
		numVars = group.getNumObjs();
		return (int)numVars;
	}



	/**
	 * Helper method to initialize a map containing variable IDs.  This solves some issues
	 * with threaded operations on CDF files.
	 */
	void HDF5FileReader::initializeVariableIDs()
	{
		int numVariables = this->getNumberOfVariables();
		//cout << "numVariables: " << numVariables << endl;
		for (int i = 0; i < numVariables; i++)
		{
			std::string variableName = this->variableGroup->getObjnameByIdx(i);
			variableIDs[variableName] = i;
//			std::cout << "variable[" << i << "]: " << variableName << std::endl;
		}
	}

	/**
	 * Helper method to initialize a variable names map. This solves some issues with
	 * threaded operations on CDF files.
	 */
	void HDF5FileReader::initializeVariableNames()
	{
		int numVariables = this->getNumberOfVariables();
		//cout << "numVariables: " << numVariables << endl;

		std::string variableName = "";
		for (int i = 0; i < numVariables; i++)
		{
			std::string variableName = this->variableGroup->getObjnameByIdx(i);
			variableNames[(long)i] = variableName;

		}
	}

	/**
	 * Gets the number of variable attributes.
	 * @return The number of variable attributes in the opened file.
	 */
	int HDF5FileReader::getNumberOfVariableAttributes()
	{
		int numVAttributes;

		//get the first variable and see how many attributes. They should all be the same.
		H5::DataSet dataset = this->variableGroup->openDataSet(this->variableGroup->getObjnameByIdx(0));
		numVAttributes = dataset.getNumAttrs();
		return numVAttributes;
	}

	/**
	 * @param attribute_id
	 * @return String representing the name of the attribute specified by attribute_id
	 */
	std::string HDF5FileReader::getVariableAttributeName(long attribute_id)
	{
		H5::DataSet dataset = this->variableGroup->openDataSet(this->variableGroup->getObjnameByIdx(0));
		H5::Attribute attribute = dataset.openAttribute(attribute_id);
		std::string buffer = attribute.getName();
		cout << "Attribute Name: '" << buffer << "'" << endl;
		return buffer;
	}


	/**
	 * @param variable
	 * @return
	 */
	long HDF5FileReader::getNumberOfRecords(const std::string& variable)
	{
		//std::cout << "reading " << variable << std::endl;
		//get variable number
		H5::DataSet dataset = this->variableGroup->openDataSet(variable);

		H5::DataSpace dataspace = dataset.getSpace();
		hsize_t count[1];
//		int ndims = dataspace.getSimpleExtentDims(count, NULL);
		return (long)count[0];
	}

	/**
	 * @param variable_id
	 * @return
	 */
	long HDF5FileReader::getNumberOfRecords(long variable)
	{
		return getNumberOfRecords(this->variableGroup->getObjnameByIdx(variable));
	}

	/**
	 * @return
	 */
	std::vector<std::string> HDF5FileReader::getVariableAttributeNames()
	{
		std::vector<std::string> attributeNames;
		int numAttributes = this->getNumberOfVariableAttributes();
		H5::DataSet dataset = this->variableGroup->openDataSet(this->variableGroup->getObjnameByIdx(0));
		for (int i = 0; i < numAttributes; i++)
		{
			std::string value = "";
			H5::Attribute attribute = dataset.openAttribute(i);
			attributeNames.push_back(attribute.getName());

		}
		return attributeNames;
	}

	/**
	 * Destructor
	 */
	HDF5FileReader::~HDF5FileReader()
	{
		if (current_file != NULL)
			close();
	}
}
#endif /* HAVE_HDF5 */
