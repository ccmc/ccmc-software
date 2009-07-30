#include "Attribute.h"
#include <iostream>

namespace ccmc
{
	/**
	 *	Default constructor.  Initializes the attributeName to "", the string value to "", and the integer and
	 *	float values to 0.
	 */
	Attribute::Attribute()
	{
		attributeName = "";
		sValue = "";
		iValue = 0;
		fValue = 0.f;

	}

	/**Returns the attribute's name
	 * @return The attribute's name
	 */
	std::string Attribute::getAttributeName()
	{
		return attributeName;
	}

	/**
	 * Sets the attribute name
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
	 * @return The float value of the attribute.  The value returned will be 0.f
	 * if the AttributeType of the Attribute object is not Attribute::FLOAT
	 */
	float Attribute::getAttributeFloat()
	{
		return fValue;
	}

	/**
	 * Returns the string representation of the attribute, if applicable.
	 * @return The string value of the attribute.  This value will be an empty string
	 * if the AttributeType of the Attribute object is not Attribute::STRING
	 */
	std::string Attribute::getAttributeString()
	{
		return sValue;
	}

	/**
	 * Returns the attribute value as an int, if applicable.
	 * @return The int value of the attribute.  The value returned will be 0
	 * if the AttributeType of the Attribute object is not Attribute::FLOAT
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
}
