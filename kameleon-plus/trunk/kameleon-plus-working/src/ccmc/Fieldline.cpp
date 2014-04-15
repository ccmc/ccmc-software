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
        mincount = 0;
        maxcount = 0;
		GlobMinIndex = 0;
		GlobMaxIndex = 0;
		startIndex = 0;
	}

	/**
	 * @param initialSize
	 */
	Fieldline::Fieldline(int initialSize)
	{
		positions.reserve(initialSize);
		values.reserve(initialSize);
        mincount = 0;
        maxcount = 0;
		GlobMinIndex = 0;
		GlobMaxIndex = 0;
		startIndex = 0;
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

	void Fieldline::insertVectorData(const Point3f& vector) //vectorValue must match positions
	{
		vectorValues.push_back(vector);
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
		if (index < positions.size()){
			positions.erase(positions.begin() + index);
		}
		if (index < values.size()){
			values.erase(values.begin() + index);
		}
		if (index < vectorValues.size()){
			vectorValues.erase(vectorValues.begin() + index);
		}

		if (index < length.size()){ //arc length will be unmodified
			length.erase(length.begin()+index);
		}

		/*
			p0    p1   p2   p3            p0       p1   p2                 p0   p1   p2 
			 |----|----|----|     -p1 =>  |---------|----|   -p0|-p3 => ---|----|----|--- 
			   e0   e1   e2                   e0      e1                   e0   e1   e2 
		*/
		if (elements.size() > 0){ 
			if ((index > 0)&&(index < elements.size())){
				elements[index-1] = elements[index-1]+elements[index];
				elementsMagnitudes[index-1] = elements[index-1].magnitude();
				elements.erase(elements.begin() + index);
				elementsMagnitudes.erase(elementsMagnitudes.begin() + index);
			}
			else{
				if (index == elements.size()){ 
					elements.pop_back();
					elementsMagnitudes.pop_back();
				}
				if (index == 0){
					elements.erase(elements.begin());
					elementsMagnitudes.erase(elementsMagnitudes.begin());
				}
			}
		}
		
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
	 * Calculate the derivative of d values/ ds over the length of the field line
	*/
	const std::vector<float>& Fieldline::derivative()
	{
		int size = this->positions.size();
		for (int i = 0; i< size-1; i++)
		{
		dds.push_back((values[i+1]-values[i])/elementsMagnitudes[i]);
		}
		return dds;
	}


	/**
	 * Calculate the integral of ds*values over the length of the field line
	 */
	const std::vector<float>& Fieldline::integrate()
	{
		int size = this->positions.size();
		if (elements.size() == 0)
		{
			getDs();
		}
		// length.push_back(0);
		integral.push_back(0);
		// First get the curve elements ds
		for (int i = 0; i < size-1; i++)
		{
			/**
			 * Integration uses trapezoidal rule: multiplies the element
			 * lengths by the average of the data on either side of the segment
			 */
		integral.push_back(elementsMagnitudes[i]*(values[i]+values[i+1])/2+integral[i]);
		}
		return integral;

	}
	/**
	 * Calculate the integral of (vector) dot dl over the length of the field line
	 */
	const std::vector<float>& Fieldline::integrateVector()
	{
//		std::cout<<"entered integrateVector()\n";
		float integrand = 0.0;
		vectorIntegral.push_back(integrand);
		if (elements.size() == 0)
		{
			getDs();
		}

		if ((elements.size() != 0) && (vectorValues.size() == elements.size()+1))
		{
//			std::cout<<"integrating vectors along fieldline. Number of elements:"<<elements.size()<<"\n";
			for (int i = 0; i < elements.size(); i++)
			{
				Point3f vAverage = (vectorValues[i] + vectorValues[i+1])*.5;
				integrand += elements[i].component1 * vAverage.component1 +
						elements[i].component2 * vAverage.component2 +
						elements[i].component3 * vAverage.component3;
				vectorIntegral.push_back(integrand);
			}
//			std::cout<<"done with vector integration.\n";
		}
		else{
			std::cout<<"vectors and elements don't match; elements must be 1 less than vectors\n" 
			<< "size of elements:" << elements.size() << "vectors:"<< vectorValues.size() << std::endl;
		}
		return vectorIntegral;

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
		if (elements.size() == 0)
		{
			getDs(); //sets elementsMagnitudes
		}
		length.push_back(0);
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
		if (length.size() == 0)
		{
			measure();
		}
		return length[i];
	}

	Fieldline Fieldline::interpolate(int option, int Npoints)
	{
		// Field line will be interpolated to fixed number given by Npoints
		/*
		 * If Npoints = 3, then the first and third interpolated points (and corresponding
		 *  data) will be set equal to the first and last points of the original field line,
		 *  and the second point will correspond to the "half-way" mark along the field line as
		 *  determined by the following weighting scheme
		 *  	option=1: interpolate to fixed arc length
		 *  	option=2: interpolate to fixed integral
		 *  	option=3: interpolate to fixed index space
		 */

		Fieldline interpolated;
		int size = this->positions.size();
		float n = 1; //initialize index for interpolated fieldline
		interpolated.insertPointData(positions[0], values[0]);
		interpolated.nearest.push_back(0);
		interpolated.tlocal.push_back(0);
		std::cout<<"interpolation option = "<< option<<"\n";


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
		float totalLength = 0;
		if(option==1)
		{
			totalLength = this->length[size-1];
			std::cout<<"opt=1; total length = "<< totalLength << "\n"<<std::endl;
		}
		else if(option==2)
		{
			totalLength = this->integral[size-1];
			std::cout<<"opt=2; integral tot: "<< totalLength << "\n"<<std::endl;
		}
		else if(option==3)
		{
			totalLength = size-1;
			std::cout<<"opt=3; points in original: "<< totalLength <<"\n" << std::endl;
		}
		else
		{
			std::cout<<"interpolation option not supported!";
		}

		float ta = 0; // initialize ta,tb
		float tb = 0;
		int flinedex = 0;
		float ts = 0;
		while(n < Npoints-1)
		{
            ts = n/(Npoints-1); // ts in [0, 1.0)

			if (option == 1)
			{
				ta = length[flinedex]/totalLength;
                tb = length[flinedex+1]/totalLength;
			}
			else if (option == 2)
			{
                ta = integral[flinedex]/totalLength;
                tb = integral[flinedex+1]/totalLength;
			}
			else if (option == 3)
			{
                ta = flinedex;
				ta = ta/totalLength;
                tb = flinedex+1;
				tb = tb/totalLength;
			}
			else
			{
				std::cout<<"interpolation option not supported";
			}

			if ((ts > ta)&&(ts <= tb)) // interpolating parameter between current and next points of original field line
			{
             float dt = tb-ta; // get size of parametric field line step
             float tloc = (ts-ta)/dt; // get local interpolation step (0,1)
             // linear interpolation
             float value = values[flinedex]*(1-tloc)+values[flinedex+1]*(tloc); //interpolate data
             Point3f point = getPositions()[flinedex]*(1-tloc)+getPositions()[flinedex+1]*tloc; //interpolate positions
             interpolated.insertPointData(point, value);
             interpolated.nearest.push_back(flinedex); //save closest point
             interpolated.tlocal.push_back(tloc); //save tlocal
             n = n+1.0;
				// std::cout<<"ta, tb, ts "<<ta<<", "<<tb<<" "<<ts<<" n = "<<n<<"\n";
			}
			else
			{
             flinedex++; //interpolant not between current and next points of original field line, increment
			}
		} // End loop

		// Insert end positions, data, index, interpolating parameter
		interpolated.insertPointData(positions[size-1], values[size-1]);
		interpolated.nearest.push_back(size-1);
		interpolated.tlocal.push_back(1.0);
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

	const std::vector<float>& Fieldline::getTlocal()
	{
		return tlocal;

	}
	// const std::vector<int >& minmax();


	void Fieldline::setStartIndex(int startIndex)
	{
		this->startIndex = startIndex;
	}
	int Fieldline::getStartIndex()
	{
		return startIndex;
	}

	/**
	 * TODO: finish documentation
	 */
	Point3f Fieldline::getStartPoint()
	{
		return startPoint;
	}

	/**
	 * TODO: finish documentation
	 * @param startPoint
	 */
	void Fieldline::setStartPoint(Point3f startPoint)
	{
		this->startPoint = startPoint;
	}

	void Fieldline::minmax()
	{
        bool minimum = true;
        bool maximum = true;
		float globmin = 0;
		float globmax = 0;
 		int size = this->positions.size();

		// first check properties of 0 endpoint
		if (values[0] >= values[1])
		{
			maxima.push_back(0);
			maxcount ++;
			std::cout << "max encountered at 0 point\n";
			globmax = values[0];
		}
		if (values[0] <= values[1])
		{
			minima.push_back(0);
			mincount ++;
			std::cout << "min encountered at 0 point\n";
			globmin = values[0];
		}

		for (int i = 1; i < size-1; i++) // interior domain of fieldline
		{
            if ((values[i] < values[i-1])&&(values[i] < values[i+1])) // i is a local minimum
               {
                 mincount ++;
                 minima.push_back(i); // store index
                 if (values[i] < globmin)
                 {
                	 GlobMinIndex = mincount-1;
                 }
                }
            if ((values[i] > values[i-1])&&(values[i] > values[i+1])) // i is a local maximum
               {
                 maxcount ++;
                 maxima.push_back(i);
                 if (values[i] > globmax)
                 {
                    GlobMaxIndex = maxcount-1;
                 }

               }
		}
		// ToDO: Need to check endpoint for global max/min?
		// check properties of last endpoint
                if (values[size-1] >= values[size-2])
                {
                        maxima.push_back(size-1);
                        maxcount ++;
                        std::cout <<"max encountered at last point\n";
                }
                if (values[size-1] <= values[size-2])
                {
                        minima.push_back(size-1);
                        mincount ++;
                        std::cout<<"min encountered at last point\n";
                }


	}


}
