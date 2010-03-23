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

		private:
			vector < Point3f > positions;
			vector < float > values;
			Point3f startPoint;

	};
}

#endif /* CCMC_FIELDLINE_H_ */
