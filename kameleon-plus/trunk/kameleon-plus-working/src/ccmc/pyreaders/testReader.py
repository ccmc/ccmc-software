#1/usr/bin/python
import unittest
import sys, os
sys.path.append('build')
sys.path.append('.')
sys.path.append('../')
import pyKameleon
from Attribute import Attribute
# import random #only used for testing
import ConfigParser
import inspect


class FileReaderFactory(object):
	def __init__(self, config_file = None):
		if config_file == None:
			print '\tFileReaderFactory called without config file, building default test reader' 
			Config = ConfigParser.ConfigParser()
			Config.add_section('Reader')
			Config.set('Reader','pyreadermodule','testReader')
			Config.set('Reader','pyfilereaderclass','pyFileReader')
			self.Config = Config
		else:
			print '\tFileReaderFactory called with', config_file 
			self.Config = getConfig(config_file)

	def createPyReader(self):
		pyReader_module_name = ConfigSectionMap(self.Config,"Reader")['pyReaderModule'.lower()]
		pyReader_class_name = ConfigSectionMap(self.Config, "Reader")['pyFileReaderClass'.lower()]
		if pyReader_module_name == "testReader":
			# print "using base pyFileReader"
			return pyFileReader()
		else:
			print "\tattempting to return subclass", pyReader_class_name
			try: 
				#TODO: check path/to/pyReader.py case, if so add path to pythonpath
				pyReader_module = __import__(pyReader_module_name)
				print "\timported", pyReader_module_name

				#TODO: check that pyReader_class is a subclass of FileReader
				pyReader = getattr(pyReader_module, pyReader_class_name)()
				assert issubclass(pyReader.__class__, pyKameleon.FileReader)				
				pyReader.Config = self.Config
				return pyReader

			except:
				raise ImportError("could not import " + pyReader_module_name)


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
		self.globalAttributes = {} #maps from attribute names or ids to global attribute objects
		self.variableAttributes = {} #maps from variable attribute names or ids to dictionaries of variable attributes


	def openFile(self, filename, readonly = True):
		""" Opens file, sets filename. To be overriden in subclass"""
		# print 'opening', filename, 'now'
		# print datapath + ' --> ' + filename
		self.current_filename = filename #invokes setCurrentFilename

		return pyKameleon.FileReader.OK

	def closeFile(self):
		"""Closes an open file """
		print '\tclosing file', self.current_filename

		return pyKameleon.FileReader.OK

	def initializeVariableIDs(self):
		"""viriableIds maps from variable string names to numbers"""
		# print '\tinitializing variableIds map!'
		i = 0
		for key in self.variables.keys():
			self.variables[long(i)] = self.variables[key]
			self.variableIDs[key] = long(i)
			self.variableNames[long(i)] = str(key)
			self.variableAttributes[key] = {} #initializes attribute dictionaries
			i += 1

	def getNumberOfVariables(self):
		return len(self.variableNames.keys())

	def getVariableID(self, variable_name):
		"""takes a variable string and returns a long integer"""
		return self.variableIDs[variable_name]

	def doesVariableExist(self,variable):
		return self.variables.has_key(variable)


	def getVariableName(self,variable_id):
		"""takes a variable id and returns a variable string name"""
		return self.variableNames[variable_id]

	def getNumberOfRecords(self, variable):
		"""returns length of the flattened variable array"""
		return len(self.variables[variable])

	def getVariableInt(self, variable):
		if self.doesVariableExist(variable):
			return self.variables[variable]
		else:
			return self.pyKameleon.FileReader.VARIABLE_DOES_NOT_EXIST


	def getVariableIntAtIndex(self, variable, index):
		if self.doesVariableExist(variable):
			return self.variables[variable][index]
		else: 
			return pyKameleon.FileReader.VARIABLE_DOES_NOT_EXIST


	def getVariable(self, variable, startIndex = None, count = None):
		if self.doesVariableExist(variable):
			return self.variables[variable]
		else:
			return pyKameleon.FileReader.VARIABLE_DOES_NOT_EXIST


	def getVariableAtIndex(self, variable, index):
		if self.doesVariableExist(variable):
			return self.variables[variable][index]
		else:
			return pyKameleon.FileReader.VARIABLE_DOES_NOT_EXIST

	def getGlobalAttribute(self, attribute):
		"""Return a global attribute defined in the reader's config file"""
		if self.globalAttributes.has_key(attribute):
			return self.globalAttributes[attribute]
		else:
			return pyKameleon.FileReader.ATTRIBUTE_DOES_NOT_EXIST

	def getGlobalAttributeName(self, attribute):
		if self.doesAttributeExist(attribute):
			return self.globalAttributes[attribute].getAttributeName()
		else:
			return pyKameleon.FileReader.ATTRIBUTE_DOES_NOT_EXIST

	def doesAttributeExist(self, attribute):
		"""checks if global attribute exists"""
		return self.globalAttributes.has_key(attribute)

	def initializeGlobalAttributeIDs(self):
		"""assign attribute ids. Note, only run this after all attribute names are loaded!"""
		self.numGAttributes = len(self.globalAttributes)
		i = 0
		for key, value in self.globalAttributes.items():
			self.globalAttributes[long(i)] = value
			i += 1

	def getNumberOfGlobalAttributes(self):
		return self.numGAttributes

	def getVariableAttribute(self, variable, attribute):
		"""Return an attribute of the specified variable"""
		if self.variableAttributes.has_key(variable):
			if self.variableAttributes[variable].has_key(attribute):
				return self.variableAttributes[variable][attribute]
			else:
				return pyKameleon.FileReader.ATTRIBUTE_DOES_NOT_EXIST
		else:
			return pyKameleon.FileReader.VARIABLE_DOES_NOT_EXIST

	def getNumberOfVariableAttributes(self):
		"""Assumes all variables have the same number of attributes! """
		variable_name = self.variableIDs.keys()[0] #get first variable name
		variable_attrs = self.variableAttributes[variable_name]
		return len(variable_attrs.keys())



class Test_pyFileReader_Global_Attributes(unittest.TestCase):
	def setUp(self):
		print 'Global Attribute test:'
		self.testReader = pyFileReader()
		self.testAttribute = Attribute('model_name', 'testModel')
		self.testReader.globalAttributes['model_name'] = self.testAttribute
		self.testReader.initializeGlobalAttributeIDs()
	
	def test_numberOfGlobalAttributes(self):
		print '\tcheck getNumberOfGlobalAttributes()' 
		self.assertEqual(self.testReader.getNumberOfGlobalAttributes(), 1)

	def test_getGlobalAttribute_by_name(self):
		print '\tchecking global attribute retrieval by name'
		check_attr = self.testReader.getGlobalAttribute('model_name')
		self.assertEqual(check_attr, self.testAttribute)

	def test_getGlobalAttributeName(self):
		print '\tchecking retrieval of global attribute name'
		self.assertEqual(self.testAttribute.getAttributeName(), 'model_name')

	def test_getGlobalAttributeValue(self):
		print '\tchecking retrieval of global attribute value'
		self.assertEqual(self.testAttribute.getAttributeValue(), 'testModel')

	def test_getGlobalAttribute_by_id(self):
		print '\tchecking global attribute retrieval by id'
		self.assertEqual(self.testReader.getGlobalAttribute(0), self.testAttribute)


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

	def test_getVariableAttribute(self):
		print '\tchecking that variable attributes can be retrieved'
		self.assertEqual(self.testReader.getVariableAttribute('power','units'), self.unitsAttr)
		self.assertEqual(self.testReader.getVariableAttribute('missing_variable','units'),pyKameleon.FileReader.VARIABLE_DOES_NOT_EXIST)
		self.assertEqual(self.testReader.getVariableAttribute('power','missing_attr'),pyKameleon.FileReader.ATTRIBUTE_DOES_NOT_EXIST)

	def test_getNumberOfVariableAttributes(self):
		print '\tchecking that number of variable attributes is right'
		self.assertEqual(self.testReader.getNumberOfVariableAttributes(), 2)


class Test_INPUT_OUTPUT(unittest.TestCase):
	def setUp(self):
		print 'io test:'
		self.testReader = pyFileReader()
		self.testReader.open('SomeDataFile.dat')

	def test_open(self):
		print '\tchecking that open() returns FileReader.OK'
		self.assertEqual(self.testReader.open('SomeDataFile.dat'), pyKameleon.FileReader.OK)
		self.assertEqual(self.testReader.current_filename, 'SomeDataFile.dat')


	def test_closeFile(self):
		print '\tcheck that closing file works'
		self.assertEqual(self.testReader.closeFile(), pyKameleon.FileReader.OK)

class Test_pyFileReader_Variable_Methods(unittest.TestCase):
	def setUp(self):
		print 'Variable test:'
		self.testReader = pyFileReader()
		self.rho = fillRandom(count=30)
		self.topology = fillRandom('int')
		self.testReader.variables['rho'] = self.rho
		self.testReader.variables['topology'] = self.topology
		self.testReader.initializeVariableIDs()

	def test_variable_exists(self):
		print '\tchecking that variable exists'
		self.assertTrue(self.testReader.doesVariableExist('rho'))
		variable_id = self.testReader.getVariableID('rho')
		self.assertTrue(self.testReader.doesVariableExist(variable_id))

	def test_variable_length(self):
		print '\tchecking variable length'
		self.assertEqual(self.testReader.getNumberOfRecords('rho'), 30)

	def test_getNumberOfVariables(self):
		print '\tchecking that number of variables is correct'
		self.assertEqual(self.testReader.getNumberOfVariables(),2)

	def test_getVariable(self):
		print '\tchecking that getVariable() returns the correct variable'
		self.assertEqual(self.testReader.getVariable('rho'), self.rho)
		self.assertEqual(self.testReader.getVariable('topology'), self.topology)
		self.assertEqual(self.testReader.getVariable('missing'), self.testReader.VARIABLE_DOES_NOT_EXIST)


	def test_getVariable_byID(self):
		print '\tchecking that getVariable() returns the correct variable if id given'
		variable_id = self.testReader.getVariableID('rho')
		self.assertEqual(self.testReader.getVariable('rho'), self.testReader.getVariable(variable_id))

	def test_getVariableID(self):
		print '\tchecking default getVariableID'
		self.assertEqual(self.testReader.getVariableID('rho'), self.testReader.variableIDs['rho'])
		self.assertEqual(type(self.testReader.getVariableID('rho')), long)

	def test_getVariableName(self):
		print '\tchecking default getVariableName'
		self.assertEqual(self.testReader.getVariableName(self.testReader.variableIDs['rho']), 'rho')
	
	def test_getVariableAtIndex(self):
		print '\tchecking that getVariableAtIndex() returns the correct value'
		self.assertEqual(self.testReader.getVariableAtIndex('rho', -1), self.rho[-1])
		self.assertEqual(self.testReader.getVariableIntAtIndex('topology',-1), self.topology[-1])

	def test_getVariableAtIndex_Fail(self):
		print '\tchecking that getVariableAtIndex() fails correctly'
		with self.assertRaises(IndexError): self.testReader.getVariableAtIndex('rho', len(self.rho))
		with self.assertRaises(IndexError):	self.testReader.getVariableIntAtIndex('topology',len(self.topology))
		self.assertEqual(self.testReader.getVariableIntAtIndex('missing',0), pyKameleon.FileReader.VARIABLE_DOES_NOT_EXIST)


class Test_Factory_methods(unittest.TestCase):
	def setUp(self):
		print 'factory method test:'

	def test_pyReader_Factory(self):
		print '\tchecking that FileReaderFactory returns default pyFileReader class'
		factory = FileReaderFactory()
		reader = factory.createPyReader()
		self.assertEqual(reader.__class__,pyFileReader)

	def test_pyReader_Facotory_with_custom(self):
		print '\tchecking that FileReaderFactory returns custom pyFileReader subclass'
		factory = FileReaderFactory('kameleon.ini')
		reader = factory.createPyReader()
		self.assertIs(issubclass(reader.__class__, pyKameleon.FileReader), True)

	def test_pyReader_Factory_with_RCM(self):
		print '\tchecking RCM.ini functionality'
		factory = FileReaderFactory('RCM.ini')
		reader = factory.createPyReader()
		self.assertIs(issubclass(reader.__class__, pyKameleon.FileReader), True)



 
def main():
	print "Performing tests for pyReader"
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
	Config.read(config_file)
	Config.sections()
	return Config

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

