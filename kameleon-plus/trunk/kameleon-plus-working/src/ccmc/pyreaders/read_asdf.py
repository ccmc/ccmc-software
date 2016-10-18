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
		self.asdf_file.tree['variables']
		self.file_tree = self.asdf_file.tree
		self.variables = self.file_tree['variables']
		self.initializeVariableIDs() #after all variables loaded
		self.initializeVariableAttributeIDs() #after all variable attributes loaded

		#ToDO: check variable names attributes (units) and global attributes

		return pyKameleon.FileReader.OK

	def closeFile(self): 
		if self.debug: print '\tclosing file', self.current_filename

		return pyKameleon.FileReader.OK

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
		self.testReader.setGlobalAttributes()
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

def write_asdf_test(filename, variable_dict, global_attributes):
	"""Writes an asdf file to be tested"""
	print 'writing to', filename, variable_dict.keys()
	tree = dict(variables=variable_dict, global_attributes = global_attributes)
	ff = asdf.AsdfFile(tree)
	ff.write_to(filename)

def main():
	from datetime import datetime
	variables = dict( 	density = np.random.rand(8,8),
						pressure = np.random.rand(8,8)
                            )
	global_attributes = dict(	model_name = 'asdf_model',
								timestep_time = datetime.now().isoformat())
	filename = '/tmp/sample_data.asdf'
	write_asdf_test(filename, variables, global_attributes)
	unittest.main()


if __name__ == '__main__':
	main()

