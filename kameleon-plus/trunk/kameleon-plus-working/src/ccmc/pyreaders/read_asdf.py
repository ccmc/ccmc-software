"""Illustrates basic use of pyFileReader class"""
import testReader
import pyKameleon
import random
import unittest
import asdf
import numpy as np
from Attribute import Attribute


class readASDF(testReader.pyFileReader):
	"""ASDF reader class"""

	def __init__(self):
		# This line is required to call the base class constructor
		testReader.pyFileReader.__init__(self)
		
		#all variables will be stored here
		testReader.pyFileReader.variables = {}

	def openFile(self, filename, readonly = True):
		if self.debug: print '\topening with asdf reader'
		self.current_filename = filename
		# self.setCurrentFilename(filename) # alternatively
		
		if self.debug: print '\tdata file:', filename, 'readonly:', readonly

		self.asdf_file =  asdf.AsdfFile.open(filename)
		self.file_tree = self.asdf_file.tree

		self.setVariables()
		self.setVariableAttributes() #after all variables loaded
		self.setGlobalAttributes()
		#ToDO: check variable names attributes (units) and global attributes

		return self.verify()

	def verify(self):
		# how to handle failure?
		try:
			self.check_global_attributes()
			self.check_variable_attributes()
		except NameError:
			self.close()
			print 'sorry, not a valid kameleon file!'
			return pyKameleon.FileReader.NOT_A_VALID_KAMELEON_FILE
		else:
			return pyKameleon.FileReader.OK

	def check_global_attributes(self):
		if self.debug: print '\tchecking all required global attributes present and accounted for'
		required_attributes = ['model_name', 'timestep_time']
		for attr_name in required_attributes:
			check_attr = self.getGlobalAttribute(attr_name)

	def check_variable_attributes(self):
		if self.debug: print '\tchecking all required variable attributes present and accounted for'
		required_attributes = ['units', 'actual_min', 'actual_max']
		for var_name in self.variableIDs.keys():
			for attr_name in required_attributes:
				check_attr = self.getVariableAttribute(var_name,attr_name)

	def closeFile(self): 
		if self.debug: print '\tclosing file', self.current_filename

		return pyKameleon.FileReader.OK

	def setVariables(self):
		self.variables = self.file_tree['variables']
		self.initializeVariableIDs() #after all variables loaded

	def setVariableAttributes(self):
		for variable, attr_dict in self.file_tree['variable_attributes'].items():
			for attr_name, v in attr_dict.items():
				self.variableAttributes[variable][str(attr_name)] = Attribute(str(attr_name),v)

		self.initializeVariableAttributeIDs() #after all variable attributes loaded

	def setGlobalAttributes(self):
		for k,v in self.file_tree['global_attributes'].items():
			self.globalAttributes[str(k)] = Attribute(str(k),v)

		self.initializeGlobalAttributeIDs()  #after all global attributes set

	def interpolate(self,variable, *point):
		if type(variable) != str:
			variable = self.variableNames[variable]
		return self.missing_value

	def interpolate_dc(self,variable,*point):
		val = self.interpolate(variable,*point)
		return val, self.missing_value, self.missing_value, self.missing_value


class Test_read_asdf_is_subclass(unittest.TestCase):
	def setUp(self):
		print 'subclassing test:'
		self.testReader = readASDF()
	def tearDown(self):
		print ' ... success!\n'

	def test_pyFileReader_subclass(self):
		print '\tchecking if pyFileReader is actually a subclass of ccmc::FileReader'
		self.assertTrue(issubclass(self.testReader.__class__, pyKameleon.FileReader))
		self.assertTrue(issubclass(self.testReader.__class__,testReader.pyFileReader))

class Test_read_asdf_Global_Attributes(unittest.TestCase):
	def setUp(self):
		print 'Global Attribute test:'
		self.testReader = readASDF()
		# self.testReader.debug = True
		self.testReader.openFile('/tmp/sample_data.asdf')
		
	def tearDown(self):
		self.testReader.close()
		print ' ... success!\n'
	
	def test_numberOfGlobalAttributes(self):
		print '\tcheck getNumberOfGlobalAttributes()' 
		self.assertEqual(self.testReader.getNumberOfGlobalAttributes(), 3)

	def test_getGlobalAttribute_by_name(self):
		print '\tchecking global attribute retrieval by name'
		check_attr = self.testReader.getGlobalAttribute('model_name')
		self.assertEqual(check_attr, self.testReader.globalAttributes['model_name'])

	def test_getGlobalAttributeName(self):
		print '\tchecking retrieval of global attribute name'
		self.assertEqual(self.testReader.getGlobalAttributeName(0), 'model_name')

	def test_getGlobalAttributeValue(self):
		print '\tchecking retrieval of global attribute value'
		glob_attr = self.testReader.getGlobalAttribute('model_name')
		self.assertEqual(glob_attr.getAttributeValue(), 'asdf_model')

	def test_getGlobalAttribute_by_id(self):
		print '\tchecking global attribute retrieval by id'
		self.assertEqual(self.testReader.getGlobalAttribute(1).getAttributeValue(), 1)

class Test_read_asdf_Variable_Attributes(unittest.TestCase):
	def setUp(self):
		print 'Variable Attribute test:'
		self.testReader = readASDF()
		self.testReader.openFile('/tmp/sample_data.asdf')

	def tearDown(self):
		self.testReader.close()
		print ' ... success!\n'

	def test_getVariableAttribute(self):
		print '\tchecking that variable attributes can be retrieved'
		self.assertEqual(self.testReader.getVariableAttribute('density','units').getAttributeValue(), 'amu/cc')
		self.assertEqual(self.testReader.getVariableAttribute('pressure','units').getAttributeValue(), 'nPa')
		with self.assertRaises(NameError): self.testReader.getVariableAttribute('missing_variable','units')
		with self.assertRaises(NameError): self.testReader.getVariableAttribute('density','missing_attr')

	def test_getNumberOfVariableAttributes(self):
		print '\tchecking that number of variable attributes is right'
		self.assertEqual(self.testReader.getNumberOfVariableAttributes(), 3)

	def test_getVariableAttributeName(self):
		print '\tchecking that variable attribute names can be retrived by ID'
		attr_id = self.testReader.getVariableAttributeID('units')
		self.assertEqual(self.testReader.getVariableAttributeName(attr_id), 'units')
		with self.assertRaises(NameError): self.testReader.getVariableAttributeName(3)

	def test_getVariableAttributeNames(self):
		print '\tchecking that variable attribute names return string vector'
		self.assertEqual(self.testReader.getVariableAttributeNames()[0], 'units')
		self.assertEqual(type(self.testReader.getVariableAttributeNames()), pyKameleon.vectorString)


def write_asdf_test(filename, **data):
	"""Writes an asdf file to be tested"""
	print 'writing to', filename
	for d in data:
		print '\t', d

	ff = asdf.AsdfFile(data)
	ff.write_to(filename)

def create_test_data():
	from datetime import datetime

	filename = '/tmp/sample_data.asdf' #how to pass this to tests?

	variables= dict( density = np.random.rand(8,8),
					 pressure = np.random.rand(8,8)
                    )

	variable_attributes = dict(	density = dict(units = 'amu/cc', actual_min = 0, actual_max = 7.0),
								pressure= dict(units = 'nPa', actual_min = 0, actual_max = 30.1),
								)

	global_attributes = dict(	model_name = 'asdf_model',
								timestep_time = datetime.now().isoformat())
	
	write_asdf_test(filename, 	variables = variables, 
								global_attributes = global_attributes, 
								variable_attributes = variable_attributes)

def main():
	create_test_data()
	unittest.main()


if __name__ == '__main__':
	main()

