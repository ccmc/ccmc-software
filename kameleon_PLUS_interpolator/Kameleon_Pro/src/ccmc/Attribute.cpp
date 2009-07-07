#include "Attribute.h"
#include <iostream>

/**
 *
 */
Attribute::Attribute()
{
	attributeName = "";
	sValue = "";
	iValue = 0;
	fValue = 0.f;

}

/**
 * @return
 */
std::string Attribute::getAttributeName()
{
	return attributeName;
}

/**
 * @param name The attribute name requested.
 */
void Attribute::setAttributeName(std::string name)
{
	attributeName = name;
}

/**
 * Copies the contents of value and stores them.
 * @param value the new attribute value
 */
void Attribute::setAttributeValue(std::string& value)
{
	type = Attribute::STRING;
	sValue = value;
}

/**
 * Copies the contents of value and stores them.
 * @param value the new attribute value
 */
void Attribute::setAttributeValue(int& value)
{
	type = Attribute::INT;
	iValue = value;
}

/**
 * Copies the contents of value and stores them.
 * @param value the new attribute value
 */
void Attribute::setAttributeValue(float& value)
{
	type = Attribute::FLOAT;
	fValue = value;
}


/**
 * @return AttributeType of the Attribute object
 */
Attribute::AttributeType Attribute::getAttributeType()
{
	return type;
}

/**
 * Returns the attribute value as a float, if applicable.
 * @return The float value of the attribute.  This value returned will be 0.f
 * if the AttributeType of the Attribute object is not Attribute::FLOAT
 */
float Attribute::getAttributeFloat()
{
	return fValue;
}

/**
 * @return
 */
std::string Attribute::getAttributeString()
{
	return sValue;
}

/**
 * @return
 */
int Attribute::getAttributeInt()
{
	return iValue;
}

/**
 * Destructor
 */
Attribute::~Attribute()
{
	/*
	if (attributeValue != NULL)
	{
		std::cout << "deleting attributeValue in Attribute.cpp" << std::endl;
		if (type == Attribute::STRING)
		{
			delete [] (char *)attributeValue;
			attributeValue = NULL;
		}else if(type == Attribute::INT)
		{
			delete (int *)attributeValue;
			attributeValue = NULL;
		}
		else
		{
			delete (float *)attributeValue;
			attributeValue = NULL;
		}
	}*/
}
