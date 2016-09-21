"""Illustrates basic use of pyFileReader class"""
import testReader
import pyKameleon
import random
import unittest
import asdf
import numpy as np


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
		
		if self.debug: print 'data file:', filename, 'readonly:', readonly

		asdf_file =  asdf.AsdfFile.open(filename)
		variables = asdf_file.tree['variables']
		self.variables = variables
		self.initializeVariableIDs() #after all variables loaded
		self.initializeVariableAttributeIDs() #after all variable attributes loaded
		self.globalAttributes['model_name'] = 'asdf model'
		self.initializeGlobalAttributeIDs()  #after all global attributes set
		#ToDO: check variable names attributes (units) and global attributes

		return pyKameleon.FileReader.OK

	def interpolate(self,variable, *point):
		if type(variable) != str:
			variable = self.variableNames[variable]
		return self.missing_value

	def interpolate_dc(self,variable,*point):
		val = self.interpolate(variable,*point)
		return val, self.missing_value, self.missing_value, self.missing_value

def write_asdf_test(filename, variable_dict):
	"""Writes an asdf file to be tested"""
	print 'writing to', filename, variable_dict.keys()
	tree = dict(variables=variable_dict)
	ff = asdf.AsdfFile(tree)
	ff.write_to(filename)

class Test_pyReader(unittest.TestCase):
	def test_open_check(self):
		variables = dict( density = np.random.rand(8,8),
	                              pressure = np.random.rand(8,8)
	                            )
		filename = '/tmp/sample_data.asdf'
		write_asdf_test(filename, variables)
		test = readASDF()
		test.debug = True
		print '\tchecking that readASDF.open() returns FileReader.OK'
		self.assertEqual(test.open(filename), pyKameleon.FileReader.OK)
		print '\topen worked..'
		self.assertEqual(test.current_filename, filename)


	def test_subclasses(self):
		print 'checking subclasses'
		self.assertTrue(issubclass(readASDF,testReader.pyFileReader))
		self.assertTrue(issubclass(readASDF,pyKameleon.FileReader))

def main():
	unittest.main()


if __name__ == '__main__':
	main()

