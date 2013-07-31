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

namespace ccmc
{
	/*
	 *
	 */
	template<class T>
	class Vector: public Point<T>
	{
		public:
			Vector();
			Vector(T c0, T c1, T c2);
			T length();
			Vector<T> operator+(const Vector<T>& v) const;
			Vector<T> operator-(const Vector<T>& v) const;
			Vector<T> operator*(const T& value) const;
			Vector<T> operator/(const T& value) const;
			void operator+=(const Vector<T>& v);
			void operator-=(const Vector<T>& v);
			void operator*=(const T& value);
			void operator/=(const T& value);
			static T dot(Vector<T>& a, Vector<T>& b);
			static T dot(Vector<T>* a, Vector<T>* b);
			static Vector<T> cross(Vector<T>& a, Vector<T>& b);
			static Vector<T> cross(Vector<T>* a, Vector<T>* b);
			static void cross(Vector<T>& c, Vector<T>& a, Vector<T>& b);
			static T triple(Vector<T>& a,Vector<T>& b,Vector<T>& c);
			static Vector<T> norm(Vector<T>& a);
			static Vector<T> norm(Vector<T>* a);
			void norm();
			static T angle(Vector<T>& a, Vector<T>& b);
			static void angle(T& angle, Vector<T>& a, Vector<T>& b);

			static Vector<T> dummy;

			virtual ~Vector();
	};

	template<class T>
	Vector<T>::Vector() :
		Point<T> ()
	{

	}

	template<class T>
	Vector<T> Vector<T>::dummy = Vector(0,0,0);

	template<class T>
	Vector<T>::~Vector()
	{

	}

	template<class T>
	Vector<T>::Vector(T c0, T c1, T c2) :
		Point<T> (c0, c1, c2)
	{

	}

	template<class T>
	T Vector<T>::length()
	{
		T c0 = this->components[0];
		T c1 = this->components[1];
		T c2 = this->components[2];
		return sqrt(c0 * c0 + c1 * c1 + c2 * c2);
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
		return Vector<T> (a.c1() * b.c2() - a.c2() * b.c1(), a.c2() * b.c0() - a.c0() * b.c2(), a.c0() * b.c1()
				- a.c1() * b.c0());
	}

	template<class T>
	Vector<T> Vector<T>::cross(Vector<T>* a, Vector<T>* b)
	{
		return Vector<T> (a->c1() * b->c2() - a->c2() * b->c1(), a->c2() * b->c0() - a->c0() * b->c2(), a->c0()
				* b->c1() - a->c1() * b->c0());
	}

	template<class T>
	void Vector<T>::cross(Vector<T>& c, Vector<T> &a, Vector<T> &b) //should be faster than cross(a,b)
	{ // c = a cross b
		c.components[0] = a.c1() * b.c2() - a.c2() * b.c1();
		c.components[1] = a.c2() * b.c0() - a.c0() * b.c2();
		c.components[2] = a.c0() * b.c1() - a.c1() * b.c0();
	}

	template<class T>
	T Vector<T>::triple(Vector<T>& a,Vector<T>& b,Vector<T>& c){
		Vector<T> temp = cross(b,c);
		return dot(a,temp);
	}

	template<class T>
	T Vector<T>::angle(Vector<T>& a, Vector<T>& b)
	{
		return T(atan2(Vector<T>::cross(a,b).length(),Vector<T>::dot(a,b)));
	}

	template<class T>
	void Vector<T>::angle(T& angle, Vector<T>& a, Vector<T>& b) //should be faster than angle(a,b)
	{
		Vector<T>::cross(Vector<T>::dummy, a, b); //dummy stores cross product of a and b
		angle = atan2(Vector<T>::dummy.length(), Vector<T>::dot(a,b));
		return;
	}

	template<class T>
	Vector<T> Vector<T>::norm(Vector<T>& a)
	{
		T length = a.length();
		return Vector<T> (a.c0() / length, a.c1() / length, a.c2()/length);
	}

	template<class T>
	Vector<T> Vector<T>::norm(Vector<T>* a)
	{
		T length = a->length();
		return Vector<T> (a->c0() / length, a->c1() / length, a->c2()/length);
	}

	template<class T>
	void Vector<T>::norm()
	{
		//call the overloaded /= operator and normalize the components of this object
		(*this)/=this->length();
	}

	template<class T>
	Vector<T> Vector<T>::operator+(const Vector<T>& v) const
	{
		return Vector<T> (this->components[0]+v.components[0],this->components[1]+v.components[1],
				this->components[2]+v.components[2]);
	}

	template<class T>
	void Vector<T>::operator+=(const Vector<T>& v)
	{
		this->components[0]+=v.components[0];
		this->components[1]+=v.components[1];
		this->components[2]+=v.components[2];
	}

	template<class T>
	Vector<T> Vector<T>::operator-(const Vector<T>& v) const
	{
		return Vector<T> (this->components[0]-v.components[0],this->components[1]-v.components[1],
				this->components[2]-v.components[2]);
	}

	template<class T>
	void Vector<T>::operator-=(const Vector<T>& v)
	{
		this->components[0]-=v.components[0];
		this->components[1]-=v.components[1];
		this->components[2]-=v.components[2];
	}

	template<class T>
	Vector<T> Vector<T>::operator*(const T& value) const
	{
		return Vector<T> (this->components[0]*value, this->components[1]*value,this->components[2]*value);

	}

	template<class T>
	void Vector<T>::operator*=(const T& value)
	{
		this->components[0]*=value;
		this->components[1]*=value;
		this->components[2]*=value;
	}

	template<class T>
	Vector<T> Vector<T>::operator/(const T& value) const
	{
		return Vector<T> (this->components[0]/value, this->components[1]/value, this->components[2]/value);
	}

	template<class T>
	void Vector<T>::operator/=(const T& value)
	{
		this->components[0]/=value;
		this->components[1]/=value;
		this->components[2]/=value;
	}

}

#endif /* VECTOR_H_ */
