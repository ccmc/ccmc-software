#ifndef CCMC_POINT3F_H_
#define CCMC_POINT3F_H_
#include <iostream>
#include <string>

namespace ccmc
{
	class Point3f
	{
			friend std::ostream& operator<<(std::ostream& out, const Point3f& point);
		public:
			//Point3f(const Point3f&);
			enum Coordinates
			{
				SPHERICAL, CARTESIAN
			};

			Point3f(const float& component1, const float& component2, const float& component3);
			Point3f(const float& component1, const float& component2, const float& component3, Coordinates c);
			Point3f(const Point3f& p);
			Point3f();
			Point3f(Coordinates c);
			void normalize();
			std::string toString() const;
			float magnitude();
			float distance(const Point3f& p) const;
			float component1;
			float component2;
			float component3;

			Point3f operator+(const Point3f& p) const;
			Point3f operator-(const Point3f& p) const;
			Point3f operator*(float value) const;
			Point3f operator*(double value) const;

			void setCoordinates(Coordinates c);
			Coordinates getCoordinates();
			Point3f getCartesian();

			virtual ~Point3f();

		private:
			Coordinates coordinates;
	};

}
#endif /*CCMC_POINT3F_H_*/
