/*
 * Fieldline.cpp
 *
 *  Created on: Jul 16, 2009
 *      Author: dberrios
 */

#include "Fieldline.h"

namespace ccmc
{
	/**
	 * TODO: finish documentation
	 */
	Fieldline::Fieldline()
	{
		//cout << "Fieldline object created" << endl;
		//positions.reserve(10000);
		//values.reserve(10000);
	}

	Fieldline::Fieldline(int initialSize)
	{
		positions.reserve(initialSize);
		values.reserve(initialSize);
	}

	/**
	 * TODO: finish documentation
	 */
	Fieldline::~Fieldline()
	{
		//cout << "Fieldline object destroyed" << endl;
	}

	/**
	 * TODO: finish documentation
	 * @param p
	 * @param d
	 */
	void Fieldline::insertPointData(const Point3f& p, const float& d)
	{
		positions.push_back(p);
		values.push_back(d);
	}

	/**
	 * TODO: finish documentation
	 * @return
	 */
	const vector<float>& Fieldline::getData()
	{
		return values;
	}

	/**
	 * TODO: finish documentation
	 * @return
	 */
	const vector<Point3f>& Fieldline::getPositions()
	{
		return positions;
	}

	/**
	 * TODO: finish documentation
	 * @return
	 */
	Fieldline Fieldline::reverseOrder()
	{
		vector<float> originalVectorValues = getData();
		vector<Point3f> originalVectorPositions = getPositions();
		Fieldline reversedLine;
		//reversedLine.reserve()
		int size = originalVectorValues.size();
		for (int i = 0; i < size; i++)
		{
			Point3f point = originalVectorPositions[size - i - 1];
			float value = originalVectorValues[size - i - 1];
			reversedLine.insertPointData(point, value);

		}
		return reversedLine;
	}

	void Fieldline::reserve(int size)
	{
		this->positions.resize(size);
		this->values.resize(size);
	}

	/**
	 * TODO: finish documentation
	 * @return
	 */
	void Fieldline::reverseOrderInPlace()
	{
		vector<float> originalVectorValues = getData();
		vector<Point3f> originalVectorPositions = getPositions();
		Fieldline reversedLine;
		int size = originalVectorValues.size();
		for (int i = 0; i < size; i++)
		{
			Point3f point = originalVectorPositions[size - i - 1];
			float value = originalVectorValues[size - i - 1];
			//reversedLine.insertPointData(point, value);
			this->positions[i] = point;
			this->values[i] = value;

		}
		//return reversedLine;
	}

	/**
	 * TODO: finish documentation
	 * @return
	 */
	int Fieldline::size()
	{
		return positions.size();
	}

	/**
	 * TODO: finish documentation
	 * @param i
	 * @return
	 */
	float Fieldline::getData(int i)
	{
		return values[i];
	}

	/**
	 * TODO: finish documentation
	 * @param i
	 * @return
	 */
	const Point3f& Fieldline::getPosition(int i)
	{
		return positions[i];
	}

	/**
	 * TODO: finish documentation
	 * @param index
	 */
	void Fieldline::removePoint(int index)
	{

		positions.erase(positions.begin() + index);
		values.erase(values.begin() + index);
	}
}
