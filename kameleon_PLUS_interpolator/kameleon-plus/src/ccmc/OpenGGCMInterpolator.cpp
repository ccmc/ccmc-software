/*
 * OpenGGCMInterpolator.cpp
 *
 *  Created on: Jun 30, 2009
 *      Author: dberrios
 */

#include "OpenGGCMInterpolator.h"
#include "OpenGGCM.h"
#include "Utils.h"
#include "StringConstants.h"

namespace ccmc
{

	/**
	 * Constructor
	 * @param modelReader
	 */
	OpenGGCMInterpolator::OpenGGCMInterpolator(Model * modelReader)
	{
		// TODO Auto-generated constructor stub
		this->modelReader = modelReader;
		conversionFactors["x"] = -1.0f;
		conversionFactors["y"] = -1.0f;
		conversionFactors["bx"] = -1.0f;
		conversionFactors["by"] = -1.0f;
		conversionFactors["bx1"] = -1.0f;
		conversionFactors["by1"] = -1.0f;
		conversionFactors["ux"] = -1.0f;
		conversionFactors["uy"] = -1.0f;
		conversionFactors["jx"] = -1.0f;
		conversionFactors["jy"] = -1.0f;

		conversionFactorsByID[modelReader->getVariableID("x")] = -1.0f;
		conversionFactorsByID[modelReader->getVariableID("y")] = -1.0f;
		conversionFactorsByID[modelReader->getVariableID("bx")] = -1.0f;
		conversionFactorsByID[modelReader->getVariableID("by")] = -1.0f;
		conversionFactorsByID[modelReader->getVariableID("bx1")] = -1.0f;
		conversionFactorsByID[modelReader->getVariableID("by1")] = -1.0f;
		conversionFactorsByID[modelReader->getVariableID("ux")] = -1.0f;
		conversionFactorsByID[modelReader->getVariableID("uy")] = -1.0f;
		conversionFactorsByID[modelReader->getVariableID("jx")] = -1.0f;
		conversionFactorsByID[modelReader->getVariableID("jy")] = -1.0f;

		/*for (int i = 0; i < variableDataX.length; i++)
		 {
		 variableDataX[i] = -variableDataX[i];

		 variableDataY[i] = -variableDataY[i];

		 }*/
		float missingValue = this->modelReader->getMissingValue();

		previousVariable = "NULL";
		previousVariableID = -1L;
		previousValue = missingValue;
		previousConversionFactor = 1.0f;
		previous_x = missingValue;
		previous_y = missingValue;
		previous_z = missingValue;
		previous_ix = 0;
		previous_iy = 0;
		previous_iz = 0;

		x_array = NULL;
		y_array = NULL;
		z_array = NULL;

	}

	/**
	 * @param variable_id
	 * @param c0
	 * @param c1
	 * @param c2
	 * @return
	 */
	float OpenGGCMInterpolator::interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2)
	{

		float dc0, dc1, dc2;
		return interpolate(variable_id, c0, c1, c2, dc0, dc1, dc2);

	}

	/**
	 * @param variable
	 * @param c0
	 * @param c1
	 * @param c2
	 * @return
	 */
	float OpenGGCMInterpolator::interpolate(const std::string& variable, const float& c0, const float& c1,
			const float& c2)
	{

		float dc0, dc1, dc2;
		long variable_id = modelReader->getVariableID(variable);
		return interpolate(variable_id, c0, c1, c2, dc0, dc1, dc2);

	}

	/**
	 * @param variable
	 * @param c0
	 * @param c1
	 * @param c2
	 * @param dc0 Reference to a variable to store the delta for component 0
	 * @param dc1 Reference to a variable to store the delta for component 1
	 * @param dc2 Reference to a variable to store the delta for component 2
	 * @return The interpolated value at position (c0,c1,c2) with deltas (dc0,dc1,dc2)
	 */
	float OpenGGCMInterpolator::interpolate(const std::string& variable, const float& c0, const float& c1,
			const float& c2, float& dc0, float& dc1, float& dc2)
	{
		//Point<float> point;

		long variable_id = modelReader->getVariableID(variable);
		return interpolate(variable_id, c0, c1, c2, dc0, dc1, dc2);

	}

	/**
	 * @param variable_id
	 * @param c0
	 * @param c1
	 * @param c2
	 * @param dc0 Reference to a variable to store the delta for component 0
	 * @param dc1 Reference to a variable to store the delta for component 1
	 * @param dc2 Reference to a variable to store the delta for component 2
	 * @return The interpolated value at position (c0,c1,c2) with deltas (dc0,dc1,dc2)
	 */
	float OpenGGCMInterpolator::interpolate(const long& variable_id, const float& c0, const float& c1, const float& c2,
			float& dc0, float& dc1, float& dc2)
	{
		//Point<float> point;


		//get cell containing point
		//getCell(variable, c0,c1,c2);
		//interpolate within cube at position c0,c1,c2

		//Point<float> point;

		//Cell3D<float, float> cell = getCell(variable, -c0,-c1,c2);

		/*float m_x = (point.c0() - positions[0].c0())/dx;
		 float m_y = (point.c1() - positions[0].c1())/dy;
		 float m_z = (point.c2() - positions[0].c2())/dz;
		 //System.out.println("m_x: " + m_x + " m_y: " + m_y + " m_z: " + m_z);

		 float vc1 = (1-m_x)*vectors[0].c0() + m_x*vectors[1].c0();
		 float vc2 = (1-m_x)*vectors[4].c0() + m_x*vectors[5].c0();
		 float vc3 = (1-m_x)*vectors[2].c0() + m_x*vectors[3].c0();
		 float vc4 = (1-m_x)*vectors[6].c0() + m_x*vectors[7].c0();
		 float vc5 = (1-m_z)*vc1+m_z*vc3;
		 float vc6 = (1-m_z)*vc2+m_z*vc4;
		 float xc = (1-m_y)*vc5 + m_y*vc6;

		 vc1 = (1-m_x)*vectors[0].c1() + m_x*vectors[1].c1();
		 vc2 = (1-m_x)*vectors[4].c1() + m_x*vectors[5].c1();
		 vc3 = (1-m_x)*vectors[2].c1() + m_x*vectors[3].c1();
		 vc4 = (1-m_x)*vectors[6].c1() + m_x*vectors[7].c1();
		 vc5 = (1-m_z)*vc1+m_z*vc3;
		 vc6 = (1-m_z)*vc2+m_z*vc4;
		 float yc = (1-m_y)*vc5 + m_y*vc6;

		 vc1 = (1-m_x)*vectors[0].c2() + m_x*vectors[1].c2();
		 vc2 = (1-m_x)*vectors[4].c2() + m_x*vectors[5].c2();
		 vc3 = (1-m_x)*vectors[2].c2() + m_x*vectors[3].c2();
		 vc4 = (1-m_x)*vectors[6].c2() + m_x*vectors[7].c2();
		 vc5 = (1-m_z)*vc1+m_z*vc3;
		 vc6 = (1-m_z)*vc2+m_z*vc4;
		 float zc = (1-m_y)*vc5 + m_y*vc6;
		 */

		//c0 = -c0;
		//c1 = -c1;
		//Point<float> p(c0,c1,c2);
		//flip c0 and c1;
		float flipped_c0 = -c0;
		float flipped_c1 = -c1;
		float missingValue = this->modelReader->getMissingValue();

		int ix, iy, iz;
		if (previous_x == c0 && previous_y == c1 && previous_z == c2)
		{
			if (previousValue == missingValue)
				return missingValue;
			ix = previous_ix;
			iy = previous_iy;
			iz = previous_iz;
		} else
		{
			//first, find the cell
			x_array = ((OpenGGCM*)this->modelReader)->getXGrid(variable_id);
			y_array = ((OpenGGCM*)this->modelReader)->getYGrid(variable_id);
			z_array = ((OpenGGCM*)this->modelReader)->getZGrid(variable_id);
			ix = Utils<float>::binary_search(*x_array, 0, (*x_array).size() - 1, flipped_c0);
			iy = Utils<float>::binary_search(*y_array, 0, (*y_array).size() - 1, flipped_c1);
			iz = Utils<float>::binary_search(*z_array, 0, (*z_array).size() - 1, c2);

			if (ix < 0 || iy < 0 || iz < 0)
				return missingValue;

		}

		float ix_value = (*x_array)[ix];
		float ixp1_value = (*x_array)[ix + 1];
		float iy_value = (*y_array)[iy];
		float iyp1_value = (*y_array)[iy + 1];
		float iz_value = (*z_array)[iz];
		float izp1_value = (*z_array)[iz + 1];

		/*Point<float> positions[8] = {
		 Point<float>(ix_value, iy_value, iz_value),
		 Point<float>(ixp1_value, iy_value, iz_value),
		 Point<float>(ix_value, iy_value, izp1_value),
		 Point<float>(ixp1_value, iy_value, izp1_value),
		 Point<float>(ix_value, iyp1_value, iz_value),
		 Point<float>(ixp1_value, iyp1_value, iz_value),
		 Point<float>(ix_value, iyp1_value, izp1_value),
		 Point<float>(ixp1_value, iyp1_value, izp1_value)
		 };*/

		/*dc0 = cell.getPositions()[1].c0() - cell.getPositions()[0].c0();
		 dc1 = cell.getPositions()[4].c1() - cell.getPositions()[0].c1();
		 dc2 = cell.getPositions()[2].c2() - cell.getPositions()[0].c2();
		 */
		dc0 = ixp1_value - ix_value;
		dc1 = iyp1_value - iy_value;
		dc2 = izp1_value - iz_value;

		int nx = (*x_array).size();
		int ny = (*y_array).size();
		int nz = (*z_array).size();
		int NV_blk = (ny) * (nx);
		//ix + iy*nx + iz*NV_blk
		int i = iz;
		int j = iy;
		int k = ix;
		int jp1 = j + 1;
		int kp1 = k + 1;
		int ip1 = i + 1;
		int itNV_blk = i * NV_blk;
		int ip1tNV_blk = ip1 * NV_blk;
		int i0 = k + (j) * (nx) + itNV_blk;
		int i1 = (kp1) + (j) * (nx) + itNV_blk;
		int i2 = k + (j) * (nx) + ip1tNV_blk;
		int i3 = (kp1) + (j) * (nx) + ip1tNV_blk;
		int i4 = k + (jp1) * (nx) + itNV_blk;
		int i5 = (kp1) + (jp1) * (nx) + itNV_blk;
		int i6 = k + (jp1) * (nx) + ip1tNV_blk;
		int i7 = (kp1) + (jp1) * (nx) + ip1tNV_blk;

		const std::vector<float> * vData = modelReader->getVariableFromMap(variable_id);
		float data[8];
		if (vData == NULL)
		{
			data[0] = modelReader->getVariableAtIndex(variable_id,i0);
			data[1] = modelReader->getVariableAtIndex(variable_id,i1);
			data[2] = modelReader->getVariableAtIndex(variable_id,i2);
			data[3] = modelReader->getVariableAtIndex(variable_id,i3);
			data[4] = modelReader->getVariableAtIndex(variable_id,i4);
			data[5] = modelReader->getVariableAtIndex(variable_id,i5);
			data[6] = modelReader->getVariableAtIndex(variable_id,i6);
			data[7] = modelReader->getVariableAtIndex(variable_id,i7);
		} else
		{
			data[0] = (*vData)[i0];
			data[1] = (*vData)[i1];
			data[2] = (*vData)[i2];
			data[3] = (*vData)[i3];
			data[4] = (*vData)[i4];
			data[5] = (*vData)[i5];
			data[6] = (*vData)[i6];
			data[7] = (*vData)[i7];
		}

		float m_x = (flipped_c0 - ix_value) / dc0;
		float m_y = (flipped_c1 - iy_value) / dc1;
		float m_z = (c2 - iz_value) / dc2;
		//std::cout << "m_x: " <<  m_x << " m_y: " << m_y << " m_z: " << m_z << std::endl;

		float vc1 = (1.f - m_x) * data[0] + m_x * data[1];
		float vc2 = (1.f - m_x) * data[4] + m_x * data[5];
		float vc3 = (1.f - m_x) * data[2] + m_x * data[3];
		float vc4 = (1.f - m_x) * data[6] + m_x * data[7];
		float vc5 = (1.f - m_z) * vc1 + m_z * vc3;
		float vc6 = (1.f - m_z) * vc2 + m_z * vc4;
		float xc = (1.f - m_y) * vc5 + m_y * vc6;
		//return cell.interpolateData(-c0,-c1,c2) * getConversionFactor(variable);

		if (previousVariableID != variable_id)
		{
			previousVariableID = variable_id;
			previousVariable = modelReader->getVariableName(variable_id);
			previousConversionFactor = getConversionFactor(variable_id);
			//std::cout << "different " << std::endl;

		}
		previous_ix = ix;
		previous_iy = iy;
		previous_iz = iz;
		previous_x = c0;
		previous_y = c1;
		previous_z = c2;

		//float value = xc*previousConversionFactor;
		previousValue = xc;
		return xc * previousConversionFactor ;

	}

	/**
	 * Creates the cell with the necessary values to interpolate the correct value at
	 * position (c0,c1,c2).
	 @verbatim
	 6_____7
	 /|    /|
	 4_|___5 |
	 | |   | |
	 | 2 - - 3
	 |/    |/
	 0 - - 1
	 @endverbatim
	 * @param variable
	 * @param c0
	 * @param c1
	 * @param c2
	 * @return The cell values necessary for inteprolating the value at position (c0,c1,c2).
	 */
	/*Cell3D<float, float> OpenGGCMInterpolator::getCell(std::string& variable, float c0, float c1, float c2)
	 {
	 Cell3D<float, float> cell;



	 //first, find the cell
	 int ix = binary_search(*x_array, 0, (*x_array).size()-1, c0);
	 int iy = binary_search(*y_array, 0, (*y_array).size()-1, c1);
	 int iz = binary_search(*z_array, 0, (*z_array).size()-1, c2);


	 float ix_value = (*x_array)[ix];
	 float ixp1_value = (*x_array)[ix+1];
	 float iy_value = (*y_array)[iy];
	 float iyp1_value = (*y_array)[iy+1];
	 float iz_value = (*z_array)[iz];
	 float izp1_value = (*z_array)[iz+1];

	 Point<float> positions[8] = {
	 Point<float>(ix_value, iy_value, iz_value),
	 Point<float>(ixp1_value, iy_value, iz_value),
	 Point<float>(ix_value, iy_value, izp1_value),
	 Point<float>(ixp1_value, iy_value, izp1_value),
	 Point<float>(ix_value, iyp1_value, iz_value),
	 Point<float>(ixp1_value, iyp1_value, iz_value),
	 Point<float>(ix_value, iyp1_value, izp1_value),
	 Point<float>(ixp1_value, iyp1_value, izp1_value)
	 };




	 cell.setPositions(positions);

	 int nx = (*x_array).size();
	 int ny = (*y_array).size();
	 int nz = (*z_array).size();
	 int NV_blk = (ny) * (nx);
	 //ix + iy*nx + iz*NV_blk
	 int i = iz;
	 int j = iy;
	 int k = ix;
	 int jp1 = j+1;
	 int kp1 = k+1;
	 int ip1 = i+1;
	 int itNV_blk = i*NV_blk;
	 int ip1tNV_blk = ip1*NV_blk;
	 int i0 = k + (j) * (nx) + itNV_blk;
	 int i1 = (kp1) + (j) * (nx) + itNV_blk;
	 int i2 = k + (j) * (nx) + ip1tNV_blk;
	 int i3 = (kp1) + (j) * (nx) + ip1tNV_blk;
	 int i4 = k + (jp1) * (nx) + itNV_blk;
	 int i5 = (kp1) + (jp1) * (nx) + itNV_blk;
	 int i6 = k + (jp1) * (nx) + ip1tNV_blk;
	 int i7 = (kp1) + (jp1) * (nx) + ip1tNV_blk;

	 std::vector<float> * vData = variableData[variable];
	 float data[9] = {
	 (*vData)[i0],
	 (*vData)[i1],
	 (*vData)[i2],
	 (*vData)[i3],
	 (*vData)[i4],
	 (*vData)[i5],
	 (*vData)[i6],
	 (*vData)[i7]

	 };
	 cell.setData(data);

	 return cell;
	 }*/

	/**
	 * @param variable
	 * @return
	 */
	float OpenGGCMInterpolator::getConversionFactor(const std::string& variable)
	{
		float conversionFactor = 1.0f;
		boost::unordered_map<std::string, float>::iterator cf = conversionFactors.find(variable);

		if (cf != conversionFactors.end())
			conversionFactor = (*cf).second;
		return conversionFactor;
	}

	/**
	 * @param variable_id
	 * @return
	 */
	float OpenGGCMInterpolator::getConversionFactor(const long& variable_id)
	{
		float conversionFactor = 1.0f;
		boost::unordered_map<long, float>::iterator cf = conversionFactorsByID.find(variable_id);

		if (cf != conversionFactorsByID.end())
			conversionFactor = (*cf).second;
		return conversionFactor;
	}

	/**
	 * Destructor
	 */
	OpenGGCMInterpolator::~OpenGGCMInterpolator()
	{
		// TODO Auto-generated destructor stub
	}

}
