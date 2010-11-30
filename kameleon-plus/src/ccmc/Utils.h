/*
 * Utils.h
 *
 *  Created on: Jul 10, 2009
 *      Author: dberrios
 */

#ifndef UTILS_H_
#define UTILS_H_

namespace ccmc
{
	template<class T>
	class Utils
	{
		public:
			Utils();
			static int binary_search(const std::vector<T>& vec, unsigned int start, unsigned int end, const T& key);
			virtual ~Utils();
	};

	/**
	 * Modified from wikibooks implementation of c++ binary search.
	 * @param vec
	 * @param start
	 * @param end
	 * @param key
	 * @return
	 */
	template<class T>
	int Utils<T>::binary_search(const std::vector<T>& vec, unsigned int start, unsigned int end, const T& key)
	{

		// Termination condition: start index greater than end index

		if (start > end)
		{
			return -1;
		}

		// Find the middle element of the vector and use that for splitting
		// the array into two pieces.
		unsigned int middle = (start + ((end - start) / 2));

		//outside valid range
		if (middle >= vec.size())
			return -1;

		if (key < vec[middle + 1] && key >= vec[middle])
		{
			return middle;
		} else if (key < vec[middle])
		{
			return binary_search(vec, start, middle - 1, key);
		}

		return binary_search(vec, middle + 1, end, key);
	}

	/**
	 * TODO:  This really needs to use a grammar and all that good stuff.  This is a dumb parser
	 */
	float Utils::calculate(const std::vector<float>& interpValues, const std::string& expression)
	{
		//assume postfix notation
		std::vector<std::string> tokens;
		int prevIndex = 0;
		for (int i = 0; i < expression.size(); i++)
		{
			if (expression[i] == '+' ||
					expression[i] == '-' ||
					expression[i] == '*' ||
					expression[i] == '/' ||
					expression[i] == '^')
			{
				//do some operation
			} else
			{
				//push onto stack
			}
		}






		return 0.;
	}




}
#endif /* UTILS_H_ */
