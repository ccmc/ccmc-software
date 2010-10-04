/*
 * Fieldline.h
 *
 *  Created on: Jul 16, 2009
 *      Author: dberrios
 */

#ifndef CCMC_FIELDLINE_H_
#define CCMC_FIELDLINE_H_

#include "Point3f.h"
#include <vector>

namespace ccmc
{
	class Fieldline
	{
		public:
			Fieldline();
			Fieldline(int initialSize);
			~Fieldline();
			void insertPointData(const Point3f& p, const float& d);
			void removePoint(int index);
			Fieldline reverseOrder();
			void reverseOrderInPlace();
			const vector< Point3f >& getPositions();
			const vector< float >& getData();
			int size();
			const Point3f& getPosition(int i);
			float getData(int i);
			Point3f getStartPoint();
			void reserve(int size);
			void setStartPoint(Point3f p);
		// A. Pembroke added below (Fall 2010)
			const vector< float>& getDs(); // requires positions
			const vector< Point3f >& getElements(); // requires elements
			const Point3f& getElement(int i); // requires elements
			const vector< float>& integrate();// requires elementsMagnitudes, values
			const vector< float>& measure(); // requires elementsMagnitudes
			float getLength(int i); // requires length
			float getIntegral(int i); // requires integral
			Fieldline interpolate(int option, int Npoints); // requires positions, length, integral
			const vector< int >& getNearest();


		private:
			vector < Point3f > positions;
			vector < float > values;
			Point3f startPoint;
			// A. Pembroke added below (Fall 2010)
			vector < Point3f > elements; // created by getDs()
			vector < float> elementsMagnitudes; // created by getDs()
			vector < float> integral; // created by integrate()
			vector < float> length; // created by measure()
			vector < int> nearest; // created by interpolate
	};
}

#endif /* CCMC_FIELDLINE_H_ */
