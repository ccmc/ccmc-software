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

	/**
	 * @param initialSize
	 */
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
	const std::vector<float>& Fieldline::getData()
	{
		return values;
	}

	/**
	 * TODO: finish documentation
	 * @return
	 */
	const std::vector<Point3f>& Fieldline::getPositions()
	{
		return positions;
	}

	/**
	 * TODO: finish documentation
	 * @return
	 */
	Fieldline Fieldline::reverseOrder()
	{
		std::vector<float> originalVectorValues = getData();
		std::vector<Point3f> originalVectorPositions = getPositions();
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

	/**
	 * @param size
	 */
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
		std::vector<float> originalVectorValues = getData();
		std::vector<Point3f> originalVectorPositions = getPositions();
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
	const std::vector<Point3f>& Fieldline::getElements()
	{
		return elements;
	}
	const Point3f& Fieldline::getElement(int i)
	{
		return elements[i];
	}

	/**
	 * Calculate the forward difference elements for a field line with ordered
	 * positions. Output has length fieldline.size()-1
	 * TODO: Add backward and higher-order differencing
	 */
	const std::vector<float>& Fieldline::getDs()
	{
		std::vector<Point3f> VectorPositions = getPositions();
		int size = getPositions().size();
		//vector<Point3f> elements;
		for (int i = 0; i < size-1; i++)
		{
			elements.push_back(this->positions[i+1]-positions[i]);
			elementsMagnitudes.push_back(elements[i].magnitude());
		}
		return elementsMagnitudes;


	}


	/**
	 * Calculate the integral of ds*values over the length of the field line
	 */
	const std::vector<float>& Fieldline::integrate()
	{
		int size = this->positions.size();
		// length.push_back(0);
		integral.push_back(0);
		// First get the curve elements ds
		for (int i = 0; i < size-1; i++)
		{
			/**
			 * TODO: Change integration so that it multiplies the element
			 * lengths by the average of the data on either side of the element
			 */
		integral.push_back(elementsMagnitudes[i]*(values[i]+values[i+1])/2+integral[i]);
		}
		return integral;

	}

	/**
	 * Get the integral up to position i
	 *
	 */
	float Fieldline::getIntegral(int i)
	{
		return integral[i];
	}


	/**
	 * Measure the length of the field line up to point i
	 */
	const std::vector<float>& Fieldline::measure()
		{
			int size = this->positions.size();
			// length.push_back(0);
			length.push_back(0);
			// First get the curve elements ds
			for (int i = 0; i < size-1; i++)
			{
			length.push_back(elementsMagnitudes[i]+length[i]);
			}
			return length;

		}
	/**
	 * Get the length up to position i
	 *
	 */
	float Fieldline::getLength(int i)
	{
		return length[i];
	}

	Fieldline Fieldline::interpolate(int option, int Npoints)
	{
		// Use i = 1 for fixed distance interpolation
		// Use i = 2 for integral-weighted interpolation

		Fieldline interpolated;
		int size = this->positions.size();
		float n = 1; //initialize index for interpolated fieldline
		interpolated.insertPointData(positions[0], values[0]);
		interpolated.nearest.push_back(0);

		if (option == 1)
		{
			//First get the length of the field line. Total length should be the default.
			/*
			 * TODO: Create an optional (public?) length variable. This would allow for
			 * custom maxlength, useful for open fields.
			 *
			 *                   ts=totalLength/(Npoints-1)
			 * |0 -------------------------|ts---------------------------Length|
			 * |P0----P1----P2--...---a----|tlocal---b------...------------Pend|
			 * |0-----t1----t2--...---ta------------tb------...------------tend|
			 *
			 */

			float totalLength = this->length[size-1];

			for  (int j = 0; j < size; j++)
			{
				float tb = length[j]/totalLength;
				float ts = n/(Npoints-1);
				if (tb > ts)
				{
					float ta = length[j-1]/totalLength;
					float dt = tb-ta;
					float tlocal = (ts-ta)/dt;

					float value = values[j-1]*(1-tlocal)+values[j]*(tlocal);
					Point3f point = getPositions()[j-1]*(1-tlocal)+getPositions()[j]*tlocal;
					interpolated.insertPointData(point, value);
					interpolated.nearest.push_back(j-1);
					n = n+1.0;
				}
			}

		}
		else
		{
			/*
			 * TODO: Handle case for integral weighting
			 */
		}

		interpolated.insertPointData(positions[size-1], values[size-1]);
		interpolated.nearest.push_back(size-1);
		return interpolated;
	}

	void Fieldline::setVariable(std::string variable)
	{
		this->variable = variable;
	}

	const std::string& Fieldline::getVariable()
	{
		return this->variable;
	}

	/*
	 * Gets the indices of the original field line nearest the interpolated ones
	 */

	const std::vector<int>& Fieldline::getNearest()
	{
		return nearest;

	}



}
