/*
 * Vector.h
 *
 *  Created on: Apr 21, 2009
 *      Author: David Berrios
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include <iostream>
#include <sstream>
#include <cmath>
#include "Point.h"
/*
 *
 */
template<class T>
class Vector:public Point<T>
{
	public:
		Vector();
		Vector(T c0, T c1, T c2);
		T length();
		static T dot(Vector<T>& a, Vector<T>& b);
		static T dot(Vector<T>* a, Vector<T>* b);
		static Vector<T> cross(Vector<T>& a, Vector<T>& b);
		static Vector<T> cross(Vector<T>* a, Vector<T>* b);

		virtual ~Vector();
};

template<class T>
Vector<T>::Vector():Point<T>()
{

}

template<class T>
Vector<T>::~Vector()
{

}

template<class T>
Vector<T>::Vector(T c0, T c1, T c2):Point<T>(c0,c1,c2)
{

}

template<class T>
T Vector<T>::length()
{
	T c0 = this->components[0];
	T c1 = this->components[1];
	T c2 = this->components[2];
	return sqrt(c0*c0 + c1*c1 + c2*c2);
}

template<class T>
T Vector<T>::dot(Vector<T>& a, Vector<T>& b)
{
	return a.c0() * b.c0() + a.c1() * b.c1() + a.c2() * b.c2();
}

template<class T>
T Vector<T>::dot(Vector<T>* a, Vector<T>* b)
{
	return a->c0() * b->c0() + a->c1() * b->c1() + a->c2() * b->c2();
}

template<class T>
Vector<T> Vector<T>::cross(Vector<T>& a, Vector<T>& b)
{
	return Vector<T>(a.c1()*b.c2() - a.c2()*b.c1(),
			a.c2()*b.c0() - a.c0()*b.c2(),
			a.c0()*b.c1() - a.c1()*b.c0());
}

template<class T>
Vector<T> Vector<T>::cross(Vector<T>* a, Vector<T>* b)
{
	return Vector<T>(a->c1()*b->c2() - a->c2()*b->c1(),
			a->c2()*b->c0() - a->c0()*b->c2(),
			a->c0()*b->c1() - a->c1()*b->c0());
}






#endif /* VECTOR_H_ */
