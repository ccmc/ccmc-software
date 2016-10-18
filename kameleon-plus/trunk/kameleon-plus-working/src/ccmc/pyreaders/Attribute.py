import unittest
import sys, os
sys.path.append('build')
sys.path.append('.')
sys.path.append('../')
import pyKameleon


class Attribute(pyKameleon.Attribute):
	"""
	python Attribute class accessible from C++

	AttributeType       	attributeName       	getAttributeName    	           		        setAttrStr
	FLOAT               	fValue              	getAttributeString  	sValue              	setAttributeName
	INT                 	getAttributeFloat   	getAttributeType    	setAttrFloat        	toString
	STRING              	getAttributeInt     	iValue              	setAttrInt          	type

	"""
	def __init__(self, name = None, value = None):
		"""default Attribute constructor
		"""
		super(Attribute, self).__init__()
		if name!= None:
			self.setAttributeName(name)
		if type(value) == int:
			self.setAttrInt(value)
		elif type(value) == float:
			self.setAttrFloat(value)
		elif type(value) == str:
			self.setAttrStr(value)
		elif type(value) == unicode:
			self.setAttrStr(str(value))

	def getAttributeValue(self):
		if self.getAttributeType() == pyKameleon.Attribute.STRING:
			return self.getAttributeString()
		elif self.getAttributeType() == pyKameleon.Attribute.FLOAT:
			return self.getAttributeFloat()
		elif self.getAttributeType() == pyKameleon.Attribute.INT:
			return self.getAttributeInt()


class Test_pyReader(unittest.TestCase):
	def setUp(self):
		self.fruit_attr = Attribute('fruit','apple')
		self.size_attr = Attribute('radius', 3.0)
		self.int_attr = Attribute('count', 5)

	def test_attribute_name(self):
		print '\tchecking that attribute name matches'
		self.assertEqual(self.fruit_attr.getAttributeName(), 'fruit')

	def test_attribute_str(self):
		print '\tchecking that string value matches'
		self.assertEqual(self.fruit_attr.getAttributeType(), pyKameleon.Attribute.STRING)
		self.assertEqual(self.fruit_attr.getAttributeString(), 'apple')

	def test_attribute_float(self):
		print '\tchecking that float value matches'
		self.assertEqual(self.size_attr.getAttributeType(), pyKameleon.Attribute.FLOAT)
		self.assertEqual(self.size_attr.getAttributeFloat(), 3.0)

	def test_attribute_int(self):
		print '\tchecking that int value matches', self.int_attr.iValue
		self.assertEqual(self.int_attr.getAttributeType(), pyKameleon.Attribute.INT)
		self.assertEqual(self.int_attr.getAttributeInt(), 5)

	def test_getAttributeValue(self):
		print '\tchecking that arbitrary value can be returned'
		self.assertEqual(self.fruit_attr.getAttributeValue(), 'apple')
		self.assertEqual(self.size_attr.getAttributeValue(), 3.0)
		self.assertEqual(self.int_attr.getAttributeValue(), 5)

	def test_attribute_is_subclass(self):
		print '\tchecking that attribute is a subclass of pyKameleon.Attribute'
		self.assertTrue(issubclass(self.fruit_attr.__class__, pyKameleon.Attribute))


def main():
	print "Performing tests for Attribute"
	unittest.main()


if __name__ == '__main__':
	main()
