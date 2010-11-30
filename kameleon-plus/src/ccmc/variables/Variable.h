/*
 * Variable.h
 *
 *  Created on: Nov 24, 2010
 *      Author: David Berrios
 */

#ifndef VARIABLE_H_
#define VARIABLE_H_

namespace ccmc
{

	/*
	 *
	 */
	class Variable
	{
		public:
			Variable(Model * model);
			virtual void getVisUnits();
			virtual void getSIUnits();
			virtual float calculate(const float& c0, const float& c1, const float& c2);
			virtual float calculate(const float& c0, const float& c1, const float& c2, float& d0,
					float& d1, float& d2);

			virtual ~Variable();
	};
}

#endif /* VARIABLE_H_ */
