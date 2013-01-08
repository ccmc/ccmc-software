/*
 * Point.h
 *
 *  Created on: Apr 21, 2009
 *      Author: David Berrios
 */

#ifndef POINT_H_
#define POINT_H_
#include <iostream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>

namespace ccmc
{
	/**
	 * @class Point Point.h ccmc/Point.h
	 * @brief TODO: Brief description of Point class
	 *
	 * TODO: Full description of Point class
	 */
	template<class T>
	class Point
	{
		public:
			Point();
			Point(T& c0, T& c1, T& c2);
			void setComponents(T& c1, T& c2, T& c3);
			const T& c0();
			const T& c1();
			const T& c2();
			std::string toString();
			virtual ~Point();

		protected:
			T components[3];

	};

	/**
	 * Default constructor
	 */
	template<class T>
	Point<T>::Point()
	{

	}

	template<class T>
	Point<T>::Point(T& c0, T& c1, T& c2)
	{
		components[0] = c0;
		components[1] = c1;
		components[2] = c2;
	}

	template<class T>
	const T& Point<T>::c0()
	{
		return components[0];
	}

	template<class T>
	const T& Point<T>::c1()
	{
		return components[1];
	}

	template<class T>
	const T& Point<T>::c2()
	{
		return components[2];
	}

	template<class T>
	void Point<T>::setComponents(T& c0, T& c1, T& c2)
	{
		components[0] = c0;
		components[1] = c1;
		components[2] = c2;
	}

	template<class T>
	Point<T>::~Point()
	{

	}

	template<class T>
	std::string Point<T>::toString()
	{
		std::string temp = "(" + boost::lexical_cast<std::string>(components[0]) + "," + boost::lexical_cast<
				std::string>(components[1]) + "," + boost::lexical_cast<std::string>(components[2]) + ")";
		return temp;

	}

}

#endif /* POINT_H_ */
