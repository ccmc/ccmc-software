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

		self.set_required_attributes()

	def set_required_attributes(self):
		self.required_global_attributes = ['model_name', 'timestep_time', 'python_model']
		self.required_variable_attributes = ['units', 'actual_min', 'actual_max']

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
			return pyKameleon.FileReader.NOT_A_VALID_KAMELEON_FILE
		else:
			return pyKameleon.FileReader.OK

	def check_global_attributes(self):
		if self.debug: 
			print '\tchecking all required global attributes present and accounted for'
		
		for attr_name in self.required_global_attributes:
			check_attr = self.getGlobalAttribute(attr_name)

	def check_variable_attributes(self):
		if self.debug: 
			print '\tchecking all required variable attributes present and accounted for'
		
		for var_name in self.variableIDs.keys():
			for attr_name in self.required_variable_attributes:
				check_attr = self.getVariableAttribute(var_name,attr_name)

	def closeFile(self): 
		if self.debug: print '\tclosing file', self.current_filename

		return pyKameleon.FileReader.OK

	def getNumberOfRecords(self, variable):
		"""returns length of the flattened variable array"""
		var_name = self.getVariableName(variable)
		try:
			return self.file_tree['variables'][var_name].size
		except:
			return len(self.file_tree['variables'][var_name])

	def setVariables(self):
		'''Set up variable names but don't copy arrays to memory yet'''
		for var_name in self.file_tree['variables']:
			self.variables[var_name] = None
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

	def getVariable(self, variable, startIndex = None, count = None):
		'''copy float variable into memory'''
		var_name = self.getVariableName(variable)
		if self.debug: print "\tpyFileReader.getVariable returning variable", var_name
		if self.variables[var_name] is None:
			self.variables[var_name] = pyKameleon.vectorFloat()
			self.variables[var_name].extend(self.file_tree['variables'][var_name].flatten())
		return self.variables[var_name]

	def getVariableAtIndex(self, variable, index):
		return self.getVariable(variable)[index]

	def getVariableInt(self, variable, startIndex = None, count = None):
		'''returns copy of flattened int array into memory'''
		var_name = self.getVariableName(variable)
		if self.debug: 
			print "\tpyFileReader.getVariableInt returning variable", var_name
		if self.variables[var_name] is None:
			self.variables[var_name] = pyKameleon.vectorInt()
			self.variables[var_name].extend(self.file_tree['variables'][var_name].flatten())
		return self.variables[var_name]

	def getVariableIntAtIndex(self, variable, index):
		return self.getVariableInt(variable)[index]


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
		self.assertEqual(self.testReader.openFile('sample_data.asdf'), pyKameleon.FileReader.OK)
		
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

class Test_fail_invalid_file(unittest.TestCase):
	def setUp(self):
		print 'Test invalid file:'
		self.testReader = readASDF()
		# self.testReader.debug = True
		
	def tearDown(self):
		self.testReader.close()
		print ' ... success!\n'

	def test_invalid_file(self):
		filename = 'fail_data.asdf'
		print '\tchecking that file ' + filename + ' is invalid (missing attributes)'
		self.assertEqual(self.testReader.openFile(filename), pyKameleon.FileReader.NOT_A_VALID_KAMELEON_FILE)

class Test_read_asdf_Variable_Attributes(unittest.TestCase):
	def setUp(self):
		print 'Variable Attribute test:'
		self.testReader = readASDF()
		self.assertEqual(self.testReader.openFile('sample_data.asdf'), pyKameleon.FileReader.OK)


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

class Test_pyFileReader_Variable_Methods(unittest.TestCase):
	def setUp(self):
		print 'Variable method test:'
		self.testReader = readASDF()
		# self.testReader.debug = True
		self.assertEqual(self.testReader.openFile('sample_data.asdf'), pyKameleon.FileReader.OK)

	def tearDown(self):
		for var_name, var_array in self.testReader.variables.items():
			if var_array is not None:
				print '\t\t', var_name, 'loaded'
		self.testReader.close()
		print ' ... success!\n'

	def test_variable_exists(self):
		print '\tchecking that variable exists'
		self.assertTrue(self.testReader.doesVariableExist('density'))
		variable_id = self.testReader.getVariableID('density')
		self.assertTrue(self.testReader.doesVariableExist(variable_id))

	# def test_add_variable_name(self):
	# 	print '\tchecking that addVariableName works'
	# 	self.testReader.addVariableName('density', 5)
	# 	with self.assertRaises(NameError): self.testReader.addVariableName('density', 3)
	# 	self.assertTrue(self.testReader.doesVariableExist('density'))
	# 	self.assertTrue(self.testReader.doesVariableExist(5))

	def test_getNumberOfRecords(self):
		print '\tchecking variable length'
		self.assertEqual(self.testReader.getNumberOfRecords('density'), 64)
		self.assertEqual(self.testReader.getNumberOfRecords(self.testReader.getVariableID('density')), 64)
		with self.assertRaises(NameError):self.testReader.getNumberOfRecords('missing')

	def test_getNumberOfVariables(self):
		print '\tchecking that number of variables is correct'
		self.assertEqual(self.testReader.getNumberOfVariables(),3)

	def test_getVariable(self):
		print '\tchecking that getVariable() returns the correct type'
		self.assertEqual(type(self.testReader.getVariable('density')), pyKameleon.vectorFloat)
		self.assertEqual(type(self.testReader.getVariableInt('topology')), pyKameleon.vectorInt)
		with self.assertRaises(NameError):self.testReader.getVariable('missing')

	def test_getVariable_byID(self):
		print '\tchecking that getVariable() returns the correct variable if id given'
		self.assertEqual(	self.testReader.getVariable('density'), 
							self.testReader.getVariable(self.testReader.getVariableID('density')))
		self.assertEqual(	self.testReader.getVariableInt('topology'), 
							self.testReader.getVariableInt(self.testReader.getVariableID('topology')))

	def test_getVariableID(self):
		print '\tchecking default getVariableID'
		self.assertEqual(self.testReader.getVariableID('density'), self.testReader.variableIDs['density'])
		self.assertEqual(type(self.testReader.getVariableID('density')), long)
		with self.assertRaises(NameError): self.testReader.getVariableID('missing')

	def test_getVariableName(self):
		print '\tchecking default getVariableName'
		self.assertEqual(self.testReader.getVariableName(self.testReader.variableIDs['density']), 'density')
		with self.assertRaises(NameError): self.testReader.getVariableName(5)
	
	def test_getVariableAtIndex(self):
		print '\tchecking that getVariableAtIndex() returns the correct value'
		self.assertEqual(self.testReader.getVariableAtIndex('density', -1), 10.)
		self.assertEqual(self.testReader.getVariableIntAtIndex('topology',-1), 63)

	def test_getVariableAtIndex_Fail(self):
		print '\tchecking that getVariableAtIndex() fails correctly'
		with self.assertRaises(IndexError): self.testReader.getVariableAtIndex('density', 64)
		with self.assertRaises(IndexError):	self.testReader.getVariableIntAtIndex('topology',64)
		with self.assertRaises(NameError):	self.testReader.getVariableIntAtIndex('missing',64)

def write_asdf_test(filename, **data):
	"""Writes an asdf file to be tested"""
	print 'writing to', filename
	for d in data:
		print '\t', d

	ff = asdf.AsdfFile(data)
	ff.write_to(filename)

def create_success_data(filename = 'sample_data.asdf'):
	"""Writes a kameleon-compatible asdf file"""
	from datetime import datetime

	variables= dict( density = np.linspace(0,10,64).reshape((8,8)),
					 pressure = np.random.rand(8,8),
					 topology = np.arange(64),
                    )

	variable_attributes = dict(	density = dict(units = 'amu/cc', actual_min = variables['density'].min(), actual_max = variables['density'].max()),
								pressure= dict(units = 'nPa', actual_min = variables['pressure'].min(), actual_max = variables['pressure'].min()),
								topology=dict(units = '1', actual_min = variables['topology'].min(), actual_max = variables['topology'].min())
								)

	global_attributes = dict(	model_name = 'asdf_model',
								timestep_time = datetime.now().isoformat(),
								python_model = 1)
	
	write_asdf_test(filename, 	variables = variables, 
								global_attributes = global_attributes, 
								variable_attributes = variable_attributes)

def create_fail_data(filename = 'fail_data.asdf'):
	"""Writes a kameleon incompatible asdf file - i.e. missing certain global or variable attributes"""
	from datetime import datetime

	variables= dict( density = np.random.rand(8,8),
					 pressure = np.random.rand(8,8)
                    )

	variable_attributes = dict(	density = dict(units = 'amu/cc', actual_min = 0, actual_max = 7.0),
								pressure= dict(units = 'nPa', actual_min = 0),
								)

	global_attributes = dict(	model_ = 'asdf_model',
								timestep_time = datetime.now().isoformat())
	
	write_asdf_test(filename, 	variables = variables, 
								global_attributes = global_attributes, 
								variable_attributes = variable_attributes)

def main():
	import os
	create_success_data()
	create_fail_data()
	unittest.main()

	# os.remove('fail_data.asdf') #clean up
	# os.remove('sample_data.asdf')

if __name__ == '__main__':
	main()

