"""Illustrates basic use of pyFileReader class"""
import testReader
from testReader import FileReaderFactory
import pyKameleon
import unittest
import asdf
from Attribute import Attribute
from datetime import datetime
import numpy as np
from scipy.interpolate import RegularGridInterpolator


class readASDF(testReader.pyFileReader):
	"""ASDF reader class"""

	def __init__(self):
		# This line is required to call the base class constructor
		testReader.pyFileReader.__init__(self)
		
		#all variables will be stored here
		self.interpolators = {}
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
		self.fields = self.file_tree['fields']

		try:
			self.setVariables()
			self.setVariableAttributes() #after all variables loaded
			self.setGlobalAttributes()
			self.check_global_attributes()
			self.check_variable_attributes()
			self.set_interpolators()
			return pyKameleon.FileReader.OK
		except NameError:
			return pyKameleon.FileReader.NOT_A_VALID_KAMELEON_FILE

	def check_global_attributes(self):
		if self.debug: 
			print '\tchecking all required global attributes present and accounted for'
		for attr_name in self.required_global_attributes:
			check_attr = self.getGlobalAttribute(attr_name)
			if self.debug:
				print '\t\t', attr_name, check_attr.getAttributeValue()

	def check_variable_attributes(self):
		if self.debug: 
			print '\tchecking all required variable attributes present and accounted for'
		
		for var_name in self.variableIDs.keys():
			for attr_name in self.required_variable_attributes:
				self.getVariableAttribute(var_name,attr_name)

	def closeFile(self): 
		if self.debug: print '\tclosing file', self.current_filename

		return pyKameleon.FileReader.OK

	def getNumberOfRecords(self, variable):
		"""returns length of the flattened variable array"""
		var_name = self.getVariableName(variable)
		try:
			return self.file_tree['variables'][var_name]['data'].size
		except:
			return len(self.file_tree['variables'][var_name]['data'])

	def setVariables(self):
		'''Set up variable names but don't copy arrays to memory yet'''
		try:
			for var_name in self.file_tree['variables']:
				self.variables[var_name] = None
			self.initializeVariableIDs() #after all variables loaded
		except KeyError:
			error_str = "'variables' not found in asdf file"
			if self.debug: print error_str
			raise NameError(error_str)

	def setVariableAttributes(self):
		for variable, variable_dict in self.file_tree['variables'].items():
			if variable_dict.has_key('attributes'):
				for attr_name, v in variable_dict['attributes'].items():
					self.variableAttributes[variable][str(attr_name)] = Attribute(str(attr_name),v)
			else:
				error_str = "Could not find variable attributes for", variable
				if self.debug: print error_str
				raise NameError(error_str)
		
		self.initializeVariableAttributeIDs() #after all variable attributes loaded

	def setGlobalAttributes(self):
		if self.file_tree.has_key('global_attributes'):
			for k,v in self.file_tree['global_attributes'].items():
				self.globalAttributes[str(k)] = Attribute(str(k),v)
		else:
			error_str = "Could not find 'global_attributes' in asdf file"
			if self.debug: print error_str
			raise NameError(error_str)
		self.initializeGlobalAttributeIDs()  #after all global attributes set


	def getVariable(self, variable, startIndex = None, count = None):
		'''copy float variable into memory'''
		var_name = self.getVariableName(variable)
		if self.debug: print "\tpyFileReader.getVariable returning variable", var_name
		if self.variables[var_name] is None:
			self.variables[var_name] = pyKameleon.vectorFloat()
			self.variables[var_name].extend(self.file_tree['variables'][var_name]['data'].flatten())
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
			self.variables[var_name].extend(self.file_tree['variables'][var_name]['data'].flatten())
		return self.variables[var_name]

	def getVariableIntAtIndex(self, variable, index):
		return self.getVariableInt(variable)[index]

	def set_interpolators(self):
		for field_name, field in self.fields.items():
			if field['interpolation_type'] == 'REGULAR_ND_GRID':
			    positions = [np.array(self.file_tree['variables'][c]['data']) for c in field['positions']]
			    for var_name in field['variables']:
			    	variable = self.file_tree['variables'][var_name]['data']
			    	try:
			    		self.interpolators[var_name] = RegularGridInterpolator(	positions, 
					    													variable,
					    													bounds_error = False, 
					    													fill_value = self.missing_value)
			    	except IndexError:
						print var_name, variable.shape, field['positions'], [p.shape for p in positions]
						print positions
						raise
			else:
				raise NotImplementedError

	def interpolate(self,variable, *point):
		if type(variable) != str:
			variable = self.variableNames[variable]

		return self.interpolators[variable](point)

	def interpolate_dc(self,variable,*point):
		val = self.interpolate(variable,*point)
		return val, self.missing_value, self.missing_value, self.missing_value

class Test_read_asdf_interpolator(unittest.TestCase):
	def setUp(self):
		self.verbose = False
		if self.verbose: print 'Interpolator test:'
		write_success_data('test/Valid_Kameleon_File.asdf')
		self.testReader = readASDF()
		# self.testReader.debug = True
		self.testReader.openFile('test/Valid_Kameleon_File.asdf')

	def tearDown(self):
		self.testReader.close()
		if self.verbose: print ' ... success!\n'

	def test_interpolate(self):
		if self.verbose: print '\tchecking that grid interpolation works'
		p_cart = 1.5, 5., .5
		p_sph = np.pi/4, -np.pi/5
	
		maxima = {}
		for v in ['rho', 'Bx', 'By', 'potential']:
			maxima[v] = self.testReader.getVariableAttribute(v,'actual_max').getAttributeValue()
		
		self.assertAlmostEqual(get_success_data.rho(*p_cart)/maxima['rho'], self.testReader.interpolate('rho', *p_cart)/maxima['rho'], places=3)
		self.assertAlmostEqual(get_success_data.b(*p_cart)[0]/maxima['Bx'], self.testReader.interpolate('Bx', *p_cart)/maxima['Bx'], places=3)
		self.assertAlmostEqual(get_success_data.b(*p_cart)[1]/maxima['By'], self.testReader.interpolate('By', *p_cart)/maxima['By'], places=3)
		self.assertAlmostEqual(get_success_data.pot(*p_sph)/maxima['potential'], self.testReader.interpolate('potential', *p_sph)/maxima['potential'], places =3)


class Test_read_asdf_is_subclass(unittest.TestCase):
	def setUp(self):
		self.verbose = False
		if self.verbose: print 'subclassing test:'
		self.testReader = readASDF()
	def tearDown(self):
		if self.verbose: print ' ... success!\n'

	def test_pyFileReader_subclass(self):
		if self.verbose: print '\tchecking if pyFileReader is actually a subclass of ccmc::FileReader'
		self.assertTrue(issubclass(self.testReader.__class__, pyKameleon.FileReader))
		self.assertTrue(issubclass(self.testReader.__class__,testReader.pyFileReader))

class Test_read_asdf_Global_Attributes(unittest.TestCase):
	def setUp(self):
		write_success_data('test/Valid_Kameleon_File.asdf')
		self.testReader = readASDF()
		# self.testReader.debug = True
		self.verbose = False
		if self.verbose: print 'Global Attribute test:'
		try:
			self.assertEqual(self.testReader.openFile('test/Valid_Kameleon_File.asdf'), pyKameleon.FileReader.OK)
		except:
			self.testReader.check_global_attributes()
			self.testReader.check_variable_attributes()
			self.testReader.setVariables()
			self.testReader.setGlobalAttributes() 
			self.testReader.setVariableAttributes()

	def tearDown(self):
		self.testReader.close()
		if self.verbose: print ' ... success!\n'
	
	def test_numberOfGlobalAttributes(self):
		if self.verbose: print '\tcheck getNumberOfGlobalAttributes()' 
		self.assertEqual(self.testReader.getNumberOfGlobalAttributes(), 3)

	def test_getGlobalAttribute_by_name(self):
		if self.verbose: print '\tchecking global attribute retrieval by name'
		check_attr = self.testReader.getGlobalAttribute('model_name')
		self.assertEqual(check_attr, self.testReader.globalAttributes['model_name'])

	def test_getGlobalAttributeName(self):
		if self.verbose: print '\tchecking retrieval of global attribute name'
		self.assertEqual(self.testReader.getGlobalAttributeName(0), 'model_name')

	def test_getGlobalAttributeValue(self):
		if self.verbose: print '\tchecking retrieval of global attribute value'
		glob_attr = self.testReader.getGlobalAttribute('model_name')
		self.assertEqual(glob_attr.getAttributeValue(), 'asdf_model')

	def test_getGlobalAttribute_by_id(self):
		if self.verbose: print '\tchecking global attribute retrieval by id'
		self.assertEqual(self.testReader.getGlobalAttribute(1).getAttributeValue(), 1)

class Test_fail_invalid_file(unittest.TestCase):
	def setUp(self):
		self.testReader = readASDF()
		self.file_prefix = 'test/Invalid_Kameleon_File_'
		self.verbose = False
		if self.verbose: print 'Test invalid file:'
		# self.testReader.debug = True
		
	def tearDown(self):
		self.testReader.close()
		if self.verbose: print ' ... success!\n'

	def test_missing_attributes(self):
		filename =  self.file_prefix + 'attributeError.asdf' 
		variables, global_attributes, fields = get_fail_data()
		explode_variables = [variables[var]['data'] for var in variables]
		write_asdf_test(filename, explode_variables = explode_variables,
						variables = variables, 
						global_attributes = global_attributes, #missing 'model_name'
						fields = fields
						)
		if self.verbose: print '\tchecking that file ' + filename + ' is invalid (missing certain attributes)'
		self.testReader = FileReaderFactory(filename).createPyReader()
		self.assertEqual(self.testReader.openFile(filename), pyKameleon.FileReader.NOT_A_VALID_KAMELEON_FILE)
		with self.assertRaises(NameError): self.testReader.check_global_attributes()
		with self.assertRaises(NameError): self.testReader.check_variable_attributes()

	def test_missing_variables(self):
		filename =  self.file_prefix + 'variables_NameError.asdf' 
		variables, global_attributes, fields = get_fail_data()
		explode_variables = [variables[var]['data'] for var in variables]
		write_asdf_test(filename, explode_variables = explode_variables,
						variables_NameError = variables, 
						global_attributes = global_attributes, 
						fields = fields)
		if self.verbose: print '\tchecking that file ' + filename + " is missing 'variables'"
		self.assertEqual(self.testReader.openFile(filename), pyKameleon.FileReader.NOT_A_VALID_KAMELEON_FILE)
		with self.assertRaises(NameError): self.testReader.setVariables()

	def test_missing_global_attributes(self):
		filename = self.file_prefix + 'globals_NameError.asdf'
		variables, global_attributes, fields = get_fail_data()
		explode_variables = [variables[var]['data'] for var in variables]
		write_asdf_test(filename, explode_variables = explode_variables,
						variables = variables, 
						global_attributes_NameError = global_attributes, 
						fields = fields)
		if self.verbose: print '\tchecking that file ' + filename + " is missing 'global_attributes'"
		self.assertEqual(self.testReader.openFile(filename), pyKameleon.FileReader.NOT_A_VALID_KAMELEON_FILE)
		with self.assertRaises(NameError): self.testReader.setGlobalAttributes() 


class Test_read_asdf_Variable_Attributes(unittest.TestCase):
	def setUp(self):
		write_success_data('test/Valid_Kameleon_File.asdf')
		self.testReader = readASDF()
		self.verbose = False
		if self.verbose: print 'Variable Attribute test:'
		# self.testReader.debug = True
		try:
			self.assertEqual(self.testReader.openFile('test/Valid_Kameleon_File.asdf'), pyKameleon.FileReader.OK)
		except:
			self.testReader.check_global_attributes()
			self.testReader.check_variable_attributes()
			self.testReader.setVariables()
			self.testReader.setGlobalAttributes() 
			self.testReader.setVariableAttributes()

	def tearDown(self):
		self.testReader.close()
		if self.verbose: print ' ... success!\n'

	def test_getVariableAttribute(self):
		if self.verbose: print '\tchecking that variable attributes can be retrieved'
		self.assertEqual(self.testReader.getVariableAttribute('rho','units').getAttributeValue(), 'amu/cc')
		self.assertEqual(self.testReader.getVariableAttribute('topology','units').getAttributeValue(), '1')
		with self.assertRaises(NameError): self.testReader.getVariableAttribute('missing_variable','units')
		with self.assertRaises(NameError): self.testReader.getVariableAttribute('rho','missing_attr')

	def test_getNumberOfVariableAttributes(self):
		if self.verbose: print '\tchecking that number of variable attributes is right'
		self.assertEqual(self.testReader.getNumberOfVariableAttributes(), 3)

	def test_getVariableAttributeName(self):
		if self.verbose: print '\tchecking that variable attribute names can be retrived by ID'
		attr_id = self.testReader.getVariableAttributeID('units')
		self.assertEqual(self.testReader.getVariableAttributeName(attr_id), 'units')
		with self.assertRaises(NameError): self.testReader.getVariableAttributeName(3)

	def test_getVariableAttributeNames(self):
		if self.verbose: print '\tchecking that variable attribute names return string vector'
		self.assertEqual(self.testReader.getVariableAttributeNames()[0], 'units')
		self.assertEqual(type(self.testReader.getVariableAttributeNames()), pyKameleon.vectorString)

class Test_pyFileReader_Variable_Methods(unittest.TestCase):
	def setUp(self):
		write_success_data('test/Valid_Kameleon_File.asdf')
		self.testReader = readASDF()
		# self.testReader.debug = True
		self.verbose = False
		if self.verbose: print 'Variable method test:'
		try:
			self.assertEqual(self.testReader.openFile('test/Valid_Kameleon_File.asdf'), pyKameleon.FileReader.OK)
		except:
			self.testReader.check_global_attributes()
			self.testReader.check_variable_attributes()
			self.testReader.setVariables()
			self.testReader.setGlobalAttributes() 
			self.testReader.setVariableAttributes()

	def tearDown(self):
		for var_name, var_array in self.testReader.variables.items():
			if var_array is not None:
				if self.verbose: print '\t\t', var_name, 'loaded'
		self.testReader.close()
		if self.verbose: print ' ... success!\n'

	def test_variable_exists(self):
		if self.verbose: print '\tchecking that variable exists'
		self.assertTrue(self.testReader.doesVariableExist('rho'))
		variable_id = self.testReader.getVariableID('rho')
		self.assertTrue(self.testReader.doesVariableExist(variable_id))


	def test_getNumberOfRecords(self):
		if self.verbose: print '\tchecking variable length'
		self.assertEqual(self.testReader.getNumberOfRecords('rho'), get_success_data.density.flatten().size)
		self.assertEqual(self.testReader.getNumberOfRecords(self.testReader.getVariableID('rho')), get_success_data.density.flatten().size)
		with self.assertRaises(NameError):self.testReader.getNumberOfRecords('missing')

	def test_getNumberOfVariables(self):
		if self.verbose: print '\tchecking that number of variables is correct'
		self.assertEqual(self.testReader.getNumberOfVariables(),len(get_success_data.variables))

	def test_getVariable(self):
		if self.verbose: print '\tchecking that getVariable() returns the correct type'
		self.assertEqual(type(self.testReader.getVariable('rho')), pyKameleon.vectorFloat)
		self.assertEqual(type(self.testReader.getVariableInt('topology')), pyKameleon.vectorInt)
		with self.assertRaises(NameError):self.testReader.getVariable('missing')

	def test_getVariable_byID(self):
		if self.verbose: print '\tchecking that getVariable() returns the correct variable if id given'
		self.assertEqual(	self.testReader.getVariable('rho'), 
							self.testReader.getVariable(self.testReader.getVariableID('rho')))
		self.assertEqual(	self.testReader.getVariableInt('topology'), 
							self.testReader.getVariableInt(self.testReader.getVariableID('topology')))

	def test_getVariableID(self):
		if self.verbose: print '\tchecking default getVariableID'
		self.assertEqual(self.testReader.getVariableID('rho'), self.testReader.variableIDs['rho'])
		self.assertEqual(type(self.testReader.getVariableID('rho')), long)
		with self.assertRaises(NameError): self.testReader.getVariableID('missing')

	def test_getVariableName(self):
		if self.verbose: print '\tchecking default getVariableName'
		self.assertEqual(self.testReader.getVariableName(self.testReader.variableIDs['rho']), 'rho')
		with self.assertRaises(NameError): self.testReader.getVariableName(25)
	
	def test_getVariableAtIndex(self):
		if self.verbose: print '\tchecking that getVariableAtIndex() returns the correct value'
		self.assertEqual(self.testReader.getVariableAtIndex('rho', -1), get_success_data.density.flatten()[-1])
		self.assertEqual(self.testReader.getVariableIntAtIndex('topology',-1), get_success_data.topology.flatten()[-1])

	def test_getVariableAtIndex_Fail(self):
		if self.verbose: print '\tchecking that getVariableAtIndex() fails correctly'
		with self.assertRaises(IndexError): self.testReader.getVariableAtIndex('rho', get_success_data.density.flatten().size)
		with self.assertRaises(IndexError):	self.testReader.getVariableIntAtIndex('topology',get_success_data.topology.flatten().size)
		with self.assertRaises(NameError):	self.testReader.getVariableIntAtIndex('missing',64)

class Test_Factory_methods(unittest.TestCase):
	def setUp(self):
		write_success_data('test/Valid_Kameleon_File.asdf')
		# print 'factory method test:'

	def test_pyReader_Factory(self):
		# print '\tchecking that FileReaderFactory returns default readASDF class'
		reader = FileReaderFactory('test/Valid_Kameleon_File.asdf').createPyReader()
		# self.assertEqual(type(reader),readASDF.__class__) #this doesn't work
		self.assertIs(issubclass(reader.__class__, pyKameleon.FileReader), True) # as long as this works, we should be good


def write_asdf_test(filename, explode_variables = None, inline_variables = None, **data):
	"""Writes an asdf file to be tested"""
	# print '\nwriting to', filename
	ff = asdf.AsdfFile(data)
	if explode_variables is None:
		pass
	else:
		for v in explode_variables:
			ff.set_array_storage(v, 'external')

	if inline_variables is None:
		pass
	else:
		for v in inline_variables:
			ff.set_array_storage(v, 'inline')
	ff.write_to(filename)
	
def write_success_data(filename = 'test/Valid_Kameleon_File.asdf'):
	# print 'writing to', filename
	variables, global_attributes, fields = get_success_data()
	position_variables = ['x', 'y', 'z', 'theta', 'phi']
	explode_variables = [variables[var]['data'] for var in variables if var not in position_variables]
	inline_variables = [variables[var]['data'] for var in variables if var in position_variables]
	write_asdf_test(filename,	
					explode_variables = explode_variables,
					inline_variables = inline_variables,
				 	variables = variables, 
					global_attributes = global_attributes, 
					fields = fields)

def get_success_data():
	"""Writes a kameleon-compatible asdf file"""
	x = np.linspace(-30, 10, 50)
	y = np.linspace(-10, 10, 50)
	z = np.linspace(-10, 10, 50)
	X, Y, Z = np.meshgrid(x,y,z, indexing='ij', sparse =False)

	theta = np.linspace(-np.pi/2, np.pi/2)
	phi = np.linspace(-np.pi,np.pi)
	THETA, PHI = np.meshgrid(theta, phi, indexing='ij', sparse=False)

	
	def pot(theta,phi):
		return np.cos(theta) + np.sin(phi)
	get_success_data.pot = pot

	def b(x,y,z):
		return x*x, y*y, z*z
	get_success_data.b = b

	def rho(x,y,z):
		return x**2 + y**2 + z**2
	get_success_data.rho = rho

	def top(x,y,z):
		return (x**2 + y**2 + z**2).astype(int)

	potential = pot(THETA,PHI)
	density = rho(X,Y,Z); get_success_data.density = density #for tests
	topology = top(X,Y,Z); get_success_data.topology = topology
	Bx, By, Bz = b(X,Y,Z)

	# fields reference variables and include supporting attributes for interpolation
	fields = dict(	potential	= dict(	positions=['theta', 'phi'], 
										variables = ['potential'],
										coordinate_system = 'GSM',
										coordinate_type = 'SPHERICAL',
										interpolation_type = 'REGULAR_ND_GRID'
										),
          			B 			= dict( positions=['x', 'y', 'z'], 
          								variables = ['Bx','By', 'Bz'], 
          								coordinate_system = 'GSM',
          								coordinate_type = 'CARTESIAN',
          								interpolation_type = 'REGULAR_ND_GRID'
          								),
          			density 	= dict( positions=['x','y','z'],
          								variables = ['rho'],
          								coordinate_system = 'GSM',
          								coordinate_type = 'CARTESIAN',
          								interpolation_type = 'REGULAR_ND_GRID'
          								),
          			topology	= dict( positions=['x','y','z'],
          								variables = ['topology'],
          								coordinate_system = 'GSM',
          								coordinate_type = 'CARTESIAN',
          								interpolation_type = 'REGULAR_ND_GRID'
          								),
          )

	# variables attributes have to match in name and number
	variables = dict(
		rho = 	dict(data = density, 	attributes = dict(units = 'amu/cc', actual_min = density.min(),actual_max = density.max())),
		potential = dict(data = potential, 	attributes = dict(units = 'V', actual_min = potential.min(), actual_max = potential.max())),
		topology = 	dict(data = topology, 	attributes = dict(units = '1', actual_min = topology.min(), actual_max = topology.max())),
		Bx = 		dict(data = Bx, 		attributes = dict(units = 'nT', actual_min = Bx.min(), actual_max = Bx.max())),
		By = 		dict(data = By, 		attributes = dict(units = 'nT', actual_min = By.min(), actual_max = By.max())),
		Bz = 		dict(data = Bz, 		attributes = dict(units = 'nT', actual_min = Bz.min(), actual_max = Bz.max())),
		x = 		dict(data = x, 			attributes = dict(units = 'Re', actual_min = x.min(), actual_max = x.max())),
		y = 		dict(data = y, 			attributes = dict(units = 'Re', actual_min = y.min(), actual_max = y.max())),
		z = 		dict(data = z, 			attributes = dict(units = 'Re', actual_min = z.min(), actual_max = z.max())),
		theta = 	dict(data = theta, 		attributes = dict(units = 'radians', actual_min = theta.min(), 	actual_max = theta.max())),
		phi = 		dict(data = phi, 		attributes = dict(units = 'radians', actual_min = phi.min(), 	actual_max = phi.max())),
	)
	get_success_data.variables = variables


	global_attributes = dict(	model_name = 'asdf_model',
								timestep_time = datetime.now().isoformat(),
								)
	return variables, global_attributes, fields
	


def get_fail_data():
	variables, global_attributes, fields = get_success_data()
	variables['rho']['attributes'].pop('actual_max')
	global_attributes.pop('timestep_time')
	return variables, global_attributes, fields


def main():
	unittest.main()

if __name__ == '__main__':
	main()

