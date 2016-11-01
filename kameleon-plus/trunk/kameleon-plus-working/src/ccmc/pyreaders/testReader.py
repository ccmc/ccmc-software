#1/usr/bin/python
import unittest
import sys, os
sys.path.append('../../../lib/ccmc/')
sys.path.append('../../../lib/ccmc/CCMC/')
sys.path.append('.')
# sys.path.append('../')
import pyKameleon
from Attribute import Attribute
from collections import OrderedDict
# import random #only used for testing
import ConfigParser
import inspect


class FileReaderFactory(object):
	def __init__(self, config_file = None):
		self.debug = False
		if config_file is None:
			# print '\tFileReaderFactory called without config file, building default test reader' 
			config = ConfigParser.ConfigParser()
			config.add_section('Reader')
			config.set('Reader','pyreadermodule','testReader')
			config.set('Reader','pyfilereaderclass','pyFileReader')
			self._config = config
		elif config_file.split('.')[-1] == 'asdf': 
			config = ConfigParser.ConfigParser()
			config.add_section('Reader')
			config.set('Reader','pyReaderModule', 'read_asdf')
			config.set('Reader','pyFileReaderClass', 'readASDF')
			self._config = config

		else:
			# print '\tFileReaderFactory called with', config_file 
			self._config = getConfig(config_file)


	def createPyReader(self):
		pyReader_module_name = get_config_value(self._config,'Reader','pyReaderModule')
		pyReader_class_name = get_config_value(self._config, 'Reader','pyFileReaderClass')
		if pyReader_module_name == "testReader":
			# print "using base pyFileReader"
			return pyFileReader()
		else:

			#TODO: check path/to/pyReader.py case, if so add path to pythonpath
			try:
				pyReader_module = __import__(pyReader_module_name)
			except:
				print '\tcreatePyReader could not import', pyReader_module_name
				raise
			try:
				pyReader = getattr(pyReader_module, pyReader_class_name)() #an object
			except:
				print '\tcould not instantiate subclass', pyReader_class_name, 'from', pyReader_module_name, ':', pyReader_module
				raise
			try:
				assert issubclass(pyReader.__class__, pyKameleon.FileReader)
			except:
				print pyReader.__class__, "is not a subclass of pyKameleon.FileReader"
				raise

			pyReader._config = self._config
			return pyReader




class pyFileReader(pyKameleon.FileReader):
	""" General class for exposing python reader to kameleon.
		
	pyFileReader is a subclass of the Kameleon FileReader c++ class

	The following static variables of the FileReader class are available:

	static variables:			|	methods: * = Pure virtuals that needs to be implemented     ++ = Implemented in c++
	----------------------------+----------------------------------------------------------------------------------------
	ATTRIBUTE_DOES_NOT_EXIST    |   addVariableName                	getNumberOfRecords             	getVariableIntAtIndex
	FILE_DOES_NOT_EXIST         |   close ++                       	getNumberOfVariableAttributes  	getVariableName *
	LOAD_FAILED                 |   closeFile *                    	getNumberOfVariables *         	initializeGlobalAttributes ++ (does nothing)
	MODEL_NOT_SUPPORTED         |   current_filename (var)         	getVariable                    	initializeVariableAttributes ++
	NOT_A_VALID_KAMELEON_FILE   |   doesAttributeExist *           	getVariableAtIndex             	initializeVariableIDs
	OK                          |   doesVariableExist *            	getVariableAttribute           	initializeVariableNames
	OPEN_ERROR                  |   getGlobalAttribute             	getVariableAttributeName       	open - calls openFile
	UNABLE_TO_ALLOCATE_MEMORY   |   getGlobalAttributeID           	getVariableAttributeNames       openFile *           
	VARIABLE_DOES_NOT_EXIST     |   getGlobalAttributeName         	getVariableID *                	setCurrentFilename ++
	VARIABLE_NOT_IN_MEMORY      |   getNumberOfGlobalAttributes    	getVariableInt                 	

	
	"""

	def __init__(self):
		"""default pyFileReader constructor

		This initializer calls the base class initializer.
		The variables dictionary is initialized here and is expected to be populated by the subclassing reader.
		""" 
		super(pyFileReader, self).__init__()
		self.variables = {} #created in subclass
		self.variableIDs = {} #maps from variable string names to long ids
		self.variableNames = {} #maps from long ids to string names
		self.globalAttributes = OrderedDict() #maps from attribute str names to global attribute objects
		self.globalAttributeNames = {} #maps from ids to str names
		self.globalAttributeIDs = {} #maps from str names to ids
		self.variableAttributes = {} #maps from variable str names to dictionaries of variable attributes
		self.variableAttributeNames = {} #maps from attribute IDs to name
		self.variableAttributeIDs = {} #maps from attribute names to IDs
		self.globalAttributes['model_name']  = Attribute('model_name', 'python_base_model')
		self.globalAttributes['python_model'] = Attribute('python_model',1)
		self.debug = False
		self.missing_value = -256.*-256.*-256.*-256.*-256. #same as ccmc::defaults::missingValue
		self.dummy_variable = pyKameleon.vectorFloat()
		self.dummy_variable.append(0)

	def getMissingValue(self):
		"""This function is ignored by Model class when Model.getMissingValue is called"""
		return self.missing_value

	def openFile(self, filename, readonly = True):
		""" Dummy method to be overriden in subclass.
			Method should open file and set current_filename. 
			If open fails, please return self.OPEN_ERROR or self.FILE_DOES_NOT_EXIST"""

		print 'dummy method opening file'
		self.current_filename = filename #automatically invokes setCurrentFilename in c++

		return pyKameleon.FileReader.OK


	def getCurrentFilename(self):
		return self.current_filename

	def closeFile(self):
		"""Closes an open file """
		print '\tclosing file', self.current_filename

		return pyKameleon.FileReader.OK

	def initializeVariableIDs(self):
		"""viriableIds maps from variable string names to numbers. This assumes variables has already been populated.
			If variable names exists, nothing happens"""
		if self.debug: print '\t\tpyFileReader.initializeVariableIDs initializing variableIds map!'
		if len(self.variableNames) == 0:
			if self.debug: print '\t\tinitializing variable names from variables dict'
			i = 0
			for key in self.variables.keys():
				# self.variables[long(i)] = self.variables[key] #removed redundant id key. Let's avoid copies and only query on names
				self.variableIDs[key] = long(i)
				self.variableNames[long(i)] = str(key)
				self.variableAttributes[key] = {} #initializes attribute dictionaries
				i += 1
		else:
			if self.debug: print '\t\tpyFileReader.initializeVariableIDs variableNames already loaded'

	def initializeVariableAttributeIDs(self):
		"""assign variable attribute ids. Note, only run this after all variablesNames are loaded!
			All variables have to have the same attributes...
		"""
		i = 0
		for key in self.variableAttributes[self.variableNames[0]]:
			self.variableAttributeIDs[key] = long(i)
			self.variableAttributeNames[long(i)] = key
			i+=1
		pass

	def addVariableName(self, variable_name, variable_id, default_array = None):
		# set default array
		if (type(default_array) == pyKameleon.vectorFloat) \
			or (type(default_array) == pyKameleon.vectorFloat) \
			or (type(default_array) == pyKameleon.vectorString):
			pass
		else:
			default_array = self.dummy_variable

		# see if variable_name and variable_id exist in variables keys
		if self.variables.has_key(variable_name):
			pass
		else: # initialize variables with new key
			self.variables[variable_name] = default_array

		if self.variableAttributes.has_key(variable_name): #in name only
			pass
		else:
			self.variableAttributes[variable_name] = {}

		# see if variable_name and variable_id exist in variableNames and variableIDs maps
		if self.variableNames.has_key(long(variable_id)):
			old_variable_name = self.variableNames[long(variable_id)]
			if old_variable_name != variable_name:
				raise NameError('variable name \'' + str(variable_name) \
					+ '\' conflicts with ' + old_variable_name +' found in map')
			else: 
				pass 
		elif self.variableIDs.has_key(variable_name):
			old_id = self.variableIDs[variable_name]
			if old_id != variable_id:
				raise NameError('variable id \'' + str(variable_id) \
					+ '\' conflicts with ' + str(old_id) + ' found in map' )
			else:
				pass
		else:
			self.variableNames[long(variable_id)] = variable_name
			self.variableIDs[variable_name] = long(variable_id)


	def doesVariableExist(self,variable):
		if self.debug: print '\t\tpyFileReader.doesVariableExist looking for', variable

		if type(variable) == str:
			return self.variables.has_key(variable)
		else:
			if self.variableNames.has_key(long(variable)):
				return self.variables.has_key(self.variableNames[variable])
			else:
				return False
		
	def getNumberOfVariables(self):
		if self.debug: print '\t\tgetting number of variables from variableNames'
		return len(self.variableNames.keys())


	def getVariableID(self, variable_name):
		"""takes a variable string and returns a long integer"""
		if self.doesVariableExist(variable_name):
			return self.variableIDs[variable_name]
		else:
			if self.debug: print '\t\tpyFileReader.getVariableID', 'variable +\'' + variable_name + '\' does not exist!'
			raise NameError('variable +\'' + variable_name + '\' does not exist!')

	def getVariableName(self,variable_id):
		"""takes a variable id and returns a variable string name. 
		if the input is a str it returns the str."""
		if self.doesVariableExist(variable_id):
			if type(variable_id) == str:
				return variable_id
			else:
				return self.variableNames[variable_id]
		else:
			raise NameError('variableID +\'' + str(variable_id) + '\' does not exist!')

	def getVariable(self, variable, startIndex = None, count = None):
		var_name = self.getVariableName(variable)
		if self.debug: print "\tpyFileReader.getVariable returning variable", var_name
		return self.variables[var_name]

	def getNumberOfRecords(self, variable):
		"""returns length of the flattened variable array"""
		var_name = self.getVariableName(variable)
		return len(self.variables[var_name])

	def getVariableInt(self, variable):
		return self.variables[self.getVariableName(variable)]


	def getVariableIntAtIndex(self, variable, index):
		var_name = self.getVariableName(variable)
		return self.variables[var_name][index]


	def getVariableAtIndex(self, variable, index):
		var_name = self.getVariableName(variable)
		return self.variables[var_name][index]

	def getGlobalAttribute(self, attribute):
		"""Return a global attribute defined in the reader's config file"""
		try:
			if type(attribute) == str:
				return self.globalAttributes[attribute]
			else:
				return self.globalAttributes[self.globalAttributeNames[attribute]]
		except KeyError:
			raise NameError('Attribute \'' + attribute + '\' does not exist')

	def getGlobalAttributeName(self, attribute_id):
		if self.doesAttributeExist(attribute_id):
			return self.globalAttributes[self.globalAttributeNames[attribute_id]].getAttributeName()
		else:
			raise NameError('Attribute \'' + str(attribute_id) + '\' does not exist')

	def doesAttributeExist(self, attribute_name):
		"""checks if global attribute exists"""
		# print '\tchecking if ', attribute_name, 'exists:', self.globalAttributes.has_key(attribute_name)
		if type(attribute_name) == str:
			return self.globalAttributes.has_key(attribute_name)
		else:
			return self.globalAttributeNames.has_key(attribute_name)

	def initializeGlobalAttributeIDs(self):
		"""assign attribute ids. Note, only run this after all attribute names are loaded!"""
		self.numGAttributes = len(self.globalAttributes)
		i = 0
		for attr_name in self.globalAttributes:
			self.globalAttributeIDs[attr_name] = long(i)
			self.globalAttributeNames[long(i)] = attr_name
			i += 1


	def getNumberOfGlobalAttributes(self):
		if self.debug: print '\t\tgetting number of global attributes'
		return self.numGAttributes

	def getVariableAttribute(self, variable, attribute):
		"""Return an attribute of the specified variable"""
		if self.variableAttributes.has_key(variable):
			if self.variableAttributes[variable].has_key(attribute):
				return self.variableAttributes[variable][attribute]
			else:
				raise NameError('Attribute \'' + str(attribute) + '\' does not exist')
		else:
			raise NameError('Variable \'' + variable + '\' does not exist')

	def getVariableAttributeID(self, attribute_name):
		if self.variableAttributeIDs.has_key(attribute_name):
			return self.variableAttributeIDs[attribute_name]
		else:
			raise NameError('variable attribute \''+str(attribute_name)+'\' does not exist')

	def getVariableAttributeName(self, attribute_id):
		"""Return name of specified attribute_id. Note: All variables share the same attribute names."""
		if self.variableAttributeNames.has_key(attribute_id):
			return self.variableAttributeNames[attribute_id]
		else:
			raise NameError('variable attribute id \''+str(attribute_id)+'\' does not exist')

	def getVariableAttributeNames(self):
		"""Return std::vector<string> of Attribute names"""
		attr_names = pyKameleon.vectorString()
		for attr_id, name in self.variableAttributeNames.items():
			attr_names.append(name)
		return attr_names


	def getNumberOfVariableAttributes(self):
		"""Assumes all variables have the same number of attributes! """
		variable_name = self.variableIDs.keys()[0] #get first variable name
		variable_attrs = self.variableAttributes[variable_name]
		return len(variable_attrs.keys())


class Test_pyFileReader_is_subclass(unittest.TestCase):
	def setUp(self):
		print 'subclassing test:'
		self.testReader = pyFileReader()

	def test_pyFileReader_subclass(self):
		print '\tchecking if pyFileReader is actually a subclass of ccmc::FileReader'
		self.assertTrue(issubclass(self.testReader.__class__, pyKameleon.FileReader))

class Test_pyFileReader_Global_Attributes(unittest.TestCase):
	def setUp(self):
		print 'Global Attribute test:'
		self.testReader = pyFileReader()
		self.testReader.initializeGlobalAttributeIDs()
	
	def test_numberOfGlobalAttributes(self):
		print '\tcheck getNumberOfGlobalAttributes()' 
		self.assertEqual(self.testReader.getNumberOfGlobalAttributes(), 2)

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
		self.assertEqual(glob_attr.getAttributeValue(), 'python_base_model')

	def test_getGlobalAttribute_by_id(self):
		print '\tchecking global attribute retrieval by id'
		self.assertEqual(self.testReader.getGlobalAttribute(1).getAttributeValue(), 1)


class Test_pyFileReader_Variable_Attributes(unittest.TestCase):
	def setUp(self):
		print 'Variable Attribute test:'
		self.testReader = pyFileReader()
		self.testReader.variables['power'] = fillRandom()
		self.testReader.variables['money'] = fillRandom('int')
		self.testReader.initializeVariableIDs()
		self.unitsAttr = Attribute('unit', '[J/s]')
		self.testReader.variableAttributes['power']['units'] = self.unitsAttr
		self.testReader.variableAttributes['power']['amount'] = Attribute('value', 9001.)
		self.testReader.variableAttributes['money']['units'] = Attribute('unit', 'pesos')
		self.testReader.variableAttributes['money']['amount'] = Attribute('value', 'over 9000!!')
		self.testReader.initializeVariableAttributeIDs()

	def test_getVariableAttribute(self):
		print '\tchecking that variable attributes can be retrieved'
		self.assertEqual(self.testReader.getVariableAttribute('power','units'), self.unitsAttr)
		with self.assertRaises(NameError): self.testReader.getVariableAttribute('missing_variable','units')
		with self.assertRaises(NameError): self.testReader.getVariableAttribute('power','missing_attr')

	def test_getNumberOfVariableAttributes(self):
		print '\tchecking that number of variable attributes is right'
		self.assertEqual(self.testReader.getNumberOfVariableAttributes(), 2)

	def test_getVariableAttributeName(self):
		print '\tchecking that variable attribute names can be retrived by ID'
		attr_id = self.testReader.getVariableAttributeID('units')
		self.assertEqual(self.testReader.getVariableAttributeName(attr_id), 'units')
		with self.assertRaises(NameError): self.testReader.getVariableAttributeName(3)

	def test_getVariableAttributeNames(self):
		print '\tchecking that variable attribute names return string vector'
		self.assertEqual(self.testReader.getVariableAttributeNames()[0], 'units')
		self.assertEqual(type(self.testReader.getVariableAttributeNames()), pyKameleon.vectorString)


class Test_INPUT_OUTPUT(unittest.TestCase):
	def setUp(self):
		print 'io test:'
		self.testReader = pyFileReader()
		self.testReader.open('SomeDataFile.dat')

	def test_open(self):
		print '\tchecking that open() returns FileReader.OK'
		self.assertEqual(self.testReader.open('SomeDataFile.dat'), pyKameleon.FileReader.OK)

	def test_getCurrentFilename(self):
		print '\tchecking that current filename can be retrieved'
		self.assertEqual(self.testReader.getCurrentFilename(), 'SomeDataFile.dat')

	def test_closeFile(self):
		print '\tcheck that closing file works'
		self.assertEqual(self.testReader.closeFile(), pyKameleon.FileReader.OK)

class Test_pyFileReader_Variable_Methods(unittest.TestCase):
	def setUp(self):
		print 'Variable test:'
		self.testReader = pyFileReader()
		self.rho = fillRandom(type = 'float', count=30)
		self.topology = fillRandom('int')
		self.testReader.variables['rho'] = self.rho
		self.testReader.variables['topology'] = self.topology
		self.testReader.initializeVariableIDs()

	def test_variable_exists(self):
		print '\tchecking that variable exists'
		self.assertTrue(self.testReader.doesVariableExist('rho'))
		variable_id = self.testReader.getVariableID('rho')
		self.assertTrue(self.testReader.doesVariableExist(variable_id))

	def test_add_variable_name(self):
		print '\tchecking that addVariableName works'
		self.testReader.addVariableName('density', 5)
		with self.assertRaises(NameError): self.testReader.addVariableName('density', 3)
		self.assertTrue(self.testReader.doesVariableExist('density'))
		self.assertTrue(self.testReader.doesVariableExist(5))

	def test_getNumberOfRecords(self):
		print '\tchecking variable length'
		self.assertEqual(self.testReader.getNumberOfRecords('rho'), 30)
		self.assertEqual(self.testReader.getNumberOfRecords(self.testReader.getVariableID('rho')), 30)
		with self.assertRaises(NameError):self.testReader.getNumberOfRecords('missing')

	def test_getNumberOfVariables(self):
		print '\tchecking that number of variables is correct'
		self.assertEqual(self.testReader.getNumberOfVariables(),2)

	def test_getVariable(self):
		print '\tchecking that getVariable() returns the correct variable'
		self.assertEqual(self.testReader.getVariable('rho'), self.rho)
		self.assertEqual(self.testReader.getVariableInt('topology'), self.topology)
		self.assertEqual(type(self.testReader.getVariable('topology')), pyKameleon.vectorInt)
		with self.assertRaises(NameError):self.testReader.getVariable('missing')

	def test_getVariable_byID(self):
		print '\tchecking that getVariable() returns the correct variable if id given'
		variable_id = self.testReader.getVariableID('rho')
		self.assertEqual(self.testReader.getVariable('rho'), self.testReader.getVariable(variable_id))

	def test_getVariableID(self):
		print '\tchecking default getVariableID'
		self.assertEqual(self.testReader.getVariableID('rho'), self.testReader.variableIDs['rho'])
		self.assertEqual(type(self.testReader.getVariableID('rho')), long)
		with self.assertRaises(NameError): self.testReader.getVariableID('missing')

	def test_getVariableName(self):
		print '\tchecking default getVariableName'
		self.assertEqual(self.testReader.getVariableName(self.testReader.variableIDs['rho']), 'rho')
		with self.assertRaises(NameError): self.testReader.getVariableName(5)
	
	def test_getVariableAtIndex(self):
		print '\tchecking that getVariableAtIndex() returns the correct value'
		self.assertEqual(self.testReader.getVariableAtIndex('rho', -1), self.rho[-1])
		self.assertEqual(self.testReader.getVariableIntAtIndex('topology',-1), self.topology[-1])

	def test_getVariableAtIndex_Fail(self):
		print '\tchecking that getVariableAtIndex() fails correctly'
		with self.assertRaises(IndexError): self.testReader.getVariableAtIndex('rho', len(self.rho))
		with self.assertRaises(IndexError):	self.testReader.getVariableIntAtIndex('topology',len(self.topology))
		with self.assertRaises(NameError):	self.testReader.getVariableIntAtIndex('missing',len(self.topology))


class Test_Factory_methods(unittest.TestCase):
	def setUp(self):
		print 'factory method test:'

	def test_pyReader_Factory(self):
		print '\tchecking that FileReaderFactory returns default pyFileReader class'
		factory = FileReaderFactory()
		reader = factory.createPyReader()
		self.assertEqual(reader.__class__,pyFileReader)

	def test_pyReader_Facotory_with_custom(self):
		print '\tchecking that FileReaderFactory can return custom pyFileReader subclass'
		factory = FileReaderFactory('ARMS.ini')
		reader = factory.createPyReader()
		self.assertIs(issubclass(reader.__class__, pyKameleon.FileReader), True)

	# def test_pyReader_Factory_with_RCM(self):
	# 	print '\tchecking RCM.ini functionality'
	# 	factory = FileReaderFactory('RCM.ini')
	# 	reader = factory.createPyReader()
	# 	self.assertIs(issubclass(reader.__class__, pyKameleon.FileReader), True)


 
def main():
	print "Performing tests for pyReader"
	writeConfig('ARMS.ini', dict(Reader=dict(	pyReaderModule='ARMS',
												pyFileReaderClass='readARMS',
	 											ModelName='HelioSpaceV10')))
	unittest.main()

def fillRandom(type = 'float', count = 30):
	import random
	if type == 'float':
		vector = pyKameleon.vectorFloat()
		for i in range(count):
			vector.append(random.random())
		return vector
	if type == 'int':
		vector = pyKameleon.vectorInt()
		for i in range(count):
			vector.append(int(5*random.random()))
		return vector

def getConfig(config_file):
	Config = ConfigParser.ConfigParser()
	try:
		Config.read(config_file)
	except:
		print 'could not read as config file. checking directory for config.ini'
		dir_ = os.path.dirname(os.path.realpath(config_file))
		try: 
			Config.read(dir_+'/config.ini')
		except:
			print 'could not find config file'
			raise
	Config.sections()
	return Config

def writeConfig(filename, dict_):
	print 'writing', filename
	config = ConfigParser.ConfigParser()
	for section in dict_:
		config.add_section(section)
		for k, v in dict_[section].items():
			config.set(section, k, v)
	with open(filename, 'w') as configfile:
	    config.write(configfile)

def ConfigSectionMap(Config, section):
    dict1 = {}
    options = Config.options(section)
    for option in options:
        try:
            dict1[option] = Config.get(section, option)
            if dict1[option] == -1:
                print("skip: %s" % option)
        except:
            print("exception on %s!" % option)
            dict1[option] = None
    return dict1

def get_config_value(config, section, name):
	return ConfigSectionMap(config,section)[name.lower()]

def itersubclasses(cls, _seen=None):
    """
    itersubclasses(cls)

    Generator over all subclasses of a given class, in depth first order.

    >>> list(itersubclasses(int)) == [bool]
    True
    >>> class A(object): pass
    >>> class B(A): pass
    >>> class C(A): pass
    >>> class D(B,C): pass
    >>> class E(D): pass
    >>> 
    >>> for cls in itersubclasses(A):
    ...     print(cls.__name__)
    B
    D
    E
    C
    >>> # get ALL (new-style) classes currently defined
    >>> [cls.__name__ for cls in itersubclasses(object)] #doctest: +ELLIPSIS
    ['type', ...'tuple', ...]
    """
    
    if not isinstance(cls, type):
        raise TypeError('itersubclasses must be called with '
                        'new-style classes, not %.100r' % cls)
    if _seen is None: _seen = set()
    try:
        subs = cls.__subclasses__()
    except TypeError: # fails only when cls is type
        subs = cls.__subclasses__(cls)
    for sub in subs:
        if sub not in _seen:
            _seen.add(sub)
            yield sub
            for sub in itersubclasses(sub, _seen):
                yield sub


if __name__ == '__main__':
	main()

