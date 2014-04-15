/*
 * Fieldline.h
 *
 *  Created on: Jul 16, 2009
 *      Author: dberrios
 */

#ifndef CCMC_FIELDLINE_H_
#define CCMC_FIELDLINE_H_

#include "Point3f.h"
#include "Vector.h"
#include <vector>

namespace ccmc
{
	/**
	 * @class Fieldline Fieldline.h ccmc/Fieldline.h
	 */
	class Fieldline
	{
		public:
			Fieldline();
			Fieldline(int initialSize);
			~Fieldline();
			void insertPointData(const Point3f& p, const float& d);
			void insertVectorData(const Point3f& vector);
			void removePoint(int index);
			Fieldline reverseOrder();
			void reverseOrderInPlace();
			const std::vector< Point3f >& getPositions();
			const std::vector< float >& getData();
			int size();
			const Point3f& getPosition(int i);
			float getData(int i);
			Point3f getStartPoint();
			int getStartIndex();
			void reserve(int size);
			void setStartPoint(Point3f p);
			void setStartIndex(int index);
			void setVariable(std::string variable);
			const std::string& getVariable();
			const std::vector< float>& getDs(); // requires positions
			const std::vector< Point3f >& getElements(); // requires elements
			const Point3f& getElement(int i); // requires elements
			const std::vector< float>& integrate();// requires elementsMagnitudes, values
			const std::vector< float>& integrateVector();
			const std::vector< float>& derivative();// requires elementsMagnitudes, values
			const std::vector< float>& measure(); // requires elementsMagnitudes
			float getLength(int i); // requires length
			float getIntegral(int i); // requires integral
			Fieldline interpolate(int option, int Npoints); // requires positions, length, integral
			const std::vector< int >& getNearest();
			const std::vector< float>& getTlocal();
			void minmax();
            int mincount; // set by minmax()
            int maxcount; //
            std::vector < int> minima; //filled by minmax()
            std::vector < int> maxima;
			int GlobMinIndex; // see minmax
			int GlobMaxIndex;


		private:
			std::vector < Point3f > positions;
			std::vector < float > values;
			std::vector < Point3f > vectorValues;
			Point3f startPoint;
			int startIndex;
			std::string variable;
			std::vector < Point3f > elements; // created by getDs()
			std::vector < float> elementsMagnitudes; // created by getDs()
			std::vector < float> integral; // created by integrate()
			std::vector < float> vectorIntegral;
			std::vector < float> dds; // created by derivative()
			std::vector < float> length; // created by measure()
			std::vector < int> nearest; // created by interpolate
			std::vector < float> tlocal; // created by interpolate

	};
}

#endif /* CCMC_FIELDLINE_H_ */
