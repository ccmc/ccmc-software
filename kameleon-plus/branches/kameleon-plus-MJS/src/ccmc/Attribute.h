/*
 * Attribute.h
 *
 *  Created on: June 1, 2009
 *      Author: David Berrios
 */

#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_
#include <string>
#include <ostream>

namespace ccmc
{
	/**
	 * @class Attribute Attribute.h ccmc/Attribute.h
	 * @brief TODO: brief description of Attribute class
	 *
	 * TODO: full description of Attribute class
	 */
	class Attribute
	{

		public:

			enum AttributeType
			{
				FLOAT, INT, STRING
			};

			std::string getAttributeName();
			void setAttributeName(std::string attributeName);
			void setAttributeValue(std::string& value);
			void setAttributeValue(int& value);
			void setAttributeValue(float& value);
			AttributeType getAttributeType();
			float getAttributeFloat();
			std::string getAttributeString();
			int getAttributeInt();
			Attribute();
			std::string toString() const;

			virtual ~Attribute();
			friend std::ostream& operator<<(std::ostream& out, const Attribute attribute);

		private:
			std::string attributeName;
			std::string sValue;
			int iValue;
			float fValue;
			AttributeType type;

	};
}

#endif /* ATTRIBUTE_H_ */
