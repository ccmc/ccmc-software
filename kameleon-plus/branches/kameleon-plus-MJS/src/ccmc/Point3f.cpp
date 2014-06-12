#include "Point3f.h"
#include <cmath>
#include <sstream>
#include <ostream>
#include <cmath>
#include "Constants.h"
#include <boost/lexical_cast.hpp>

namespace ccmc
{
	/**
	 * @param out
	 * @param point
	 * @return
	 */
	std::ostream& operator<<(std::ostream& out, const Point3f& point)
	{
		out << point.toString();
		return out;
	}

	/**
	 * TODO: finish documentation
	 * @param component1
	 * @param component2
	 * @param component3
	 */
	Point3f::Point3f(const float& component1, const float& component2, const float& component3)
	{
		this->component1 = component1;
		this->component2 = component2;
		this->component3 = component3;
		coordinates = Point3f::CARTESIAN;
	}

	/**
	 * TODO: finish documentation
	 * @param component1
	 * @param component2
	 * @param component3
	 */
	Point3f::Point3f(const float& component1, const float& component2, const float& component3, Coordinates c)
	{
		this->component1 = component1;
		this->component2 = component2;
		this->component3 = component3;
		coordinates = c;
	}

	Point3f::Point3f(const Point3f& p)
	{
		this->component1 = p.component1;
		this->component2 = p.component2;
		this->component3 = p.component3;
		coordinates = p.coordinates;
	}

	/**
	 * TODO: finish documentation
	 */
	Point3f::Point3f()
	{
		this->component1 = 0.0;
		this->component2 = 0.0;
		this->component3 = 0.0;
	}

	/**
	 * TODO: finish documentation
	 */
	Point3f::Point3f(Coordinates c)
	{
		this->component1 = 0.0;
		this->component2 = 0.0;
		this->component3 = 0.0;
		coordinates = c;
	}

	/**
	 * TODO: finish documentation
	 */
	Point3f::~Point3f()
	{

	}

	/**
	 * TODO: finish documentation
	 */
	Point3f Point3f::operator+(const Point3f& p) const
	{
		Point3f point;
		point.component1 = component1 + p.component1;
		point.component2 = component2 + p.component2;
		point.component3 = component3 + p.component3;
		point.coordinates = p.coordinates;

		return point;
	}
	/**
	 * Minus operator where sender is on the right
	 * TODO: test this
	 */
	Point3f Point3f::operator-(const Point3f& p) const
	{
		Point3f point;
		point.component1 = component1 - p.component1;
		point.component2 = component2 - p.component2;
		point.component3 = component3 - p.component3;
		point.coordinates = p.coordinates;

		return point;
	}


	/**
	 * TODO: finish documentation
	 */
	Point3f Point3f::operator*(float value) const
	{
		Point3f point;
		point.component1 = component1 * value;
		point.component2 = component2 * value;
		point.component3 = component3 * value;
		point.coordinates = coordinates;
		return point;
	}

	/**
	 * TODO: finish documentation
	 */
	Point3f Point3f::operator*(double value) const
	{
		Point3f point;
		point.component1 = component1 * value;
		point.component2 = component2 * value;
		point.component3 = component3 * value;
		point.coordinates = coordinates;
		return point;
	}

	/** compute the distance between this point, and Point p **/
	/**
	 * TODO: finish documentation
	 */
	float Point3f::distance(const Point3f& p) const
	{
		float dist;

		if (coordinates == Point3f::SPHERICAL)
		{
			//r p t
			float sinTheta1 = std::sin(ccmc::constants::DegreesToRadians*(90.f-component2));
			float cartesianComponent1 = component1 * sinTheta1 * std::cos(ccmc::constants::DegreesToRadians * component3);
			float cartesianComponent2 = component1 * sinTheta1 * std::sin(ccmc::constants::DegreesToRadians * component3);
			float cartesianComponent3 = component1 * std::cos(ccmc::constants::DegreesToRadians * (90.f - component2));

			float sinTheta2 = std::sin(ccmc::constants::DegreesToRadians*(90.f-p.component2));
			float pCartesianComponent1 = p.component1 * sinTheta1 * std::cos(ccmc::constants::DegreesToRadians * p.component3);
			float pCartesianComponent2 = p.component1 * sinTheta1 * std::sin(ccmc::constants::DegreesToRadians * p.component3);
			float pCartesianComponent3 = p.component1 * std::cos(ccmc::constants::DegreesToRadians * (90.f - p.component2));


			float diff1 = cartesianComponent1 - pCartesianComponent1;
			float diff2 = cartesianComponent2 - pCartesianComponent2;
			float diff3 = cartesianComponent3 - pCartesianComponent3;

			dist = std::sqrt(diff1 * diff1 + diff2 * diff2 + diff3 * diff3);
		} else
		{
			dist = std::sqrt((component1 - p.component1) * (component1 - p.component1) + (component2 - p.component2)
					* (component2 - p.component2) + (component3 - p.component3) * (component3 - p.component3));
		}
		return dist;
	}

	/**
	 * TODO: finish documentation
	 */
	void Point3f::normalize()
	{

		float mag = magnitude();
		if (std::abs(mag - 0.0) > .0000001)
		{
			this->component1 /= mag;
			this->component2 /= mag;
			this->component3 /= mag;
		}
	}

	/**
	 * Computes the magnitude of the cartesian vector
	 */
	float Point3f::magnitude()
	{
		return sqrt(component1 * component1 + component2 * component2 + component3 * component3);
	}

	/**
	 * TODO: finish documentation
	 */
	std::string Point3f::toString() const
	{
/*		std::string beg = "(";
		std::string mid = ",";
		std::string end = ")";
		std::ostringstream oss1;
		oss1 << component1;
		std::string scomponent1 = oss1.str();
		std::ostringstream oss2;
		oss2 << component2;
		std::string scomponent2 = oss2.str();
		std::ostringstream oss3;
		oss3 << component3;
		std::string scomponent3 = oss3.str();
		std::string str = beg + scomponent1 + mid + scomponent2 + mid + scomponent3 + end;
		return str;
		*/
		std::string temp_string = "";
		temp_string = "(" + boost::lexical_cast<std::string>(component1) + ",";
		temp_string += boost::lexical_cast<std::string>(component2) + ",";
		temp_string += boost::lexical_cast<std::string>(component3) + ")";
		return temp_string;
	}

	/**
	 * TODO: finish documentation
	 */
	void Point3f::setCoordinates(Point3f::Coordinates c)
	{
		coordinates = c;
	}

	/**
	 * TODO: finish documentation
	 */
	Point3f::Coordinates Point3f::getCoordinates()
	{
		return coordinates;
	}

	Point3f Point3f::getCartesian()
	{
		Point3f cartesian;
		float sinTheta1 = std::sin(ccmc::constants::DegreesToRadians*(90.f-component2));
		cartesian.component1 = component1 * sinTheta1 * std::cos(ccmc::constants::DegreesToRadians * component3);
		cartesian.component2 = component1 * sinTheta1 * std::sin(ccmc::constants::DegreesToRadians * component3);
		cartesian.component3 = component1 * std::cos(ccmc::constants::DegreesToRadians * (90.f - component2));
		return cartesian;

	}

}
